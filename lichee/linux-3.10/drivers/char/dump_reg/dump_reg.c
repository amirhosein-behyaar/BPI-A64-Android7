/*
 * drivers/char/dump_reg/dump_reg.c
 *
 * Copyright(c) 2015-2018 Allwinnertech Co., Ltd.
 *      http://www.allwinnertech.com
 *
 * Author: Liugang <liugang@allwinnertech.com>
 *         Xiafeng <xiafeng@allwinnertech.com>
 *
 * dump registers sysfs driver
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/kdev_t.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/miscdevice.h>
#include <linux/seq_file.h>
#include <asm/memory.h>
#include "dump_reg.h"



#ifdef CONFIG_ARM64
#define SUNXI_IO_PHYS_BASE	0x01000000
#if defined(CONFIG_ARCH_SUN50IW3) \
	|| defined(CONFIG_ARCH_SUN50IW6)
#define SUNXI_IO_SIZE		(SZ_128M + SZ_16M)
#else
#define SUNXI_IO_SIZE		SZ_16M          /* 16MB(Max) */
#endif
#define PLAT_PHYS_OFFSET	0x40000000UL
#define SUNXI_IOMEM_VASE	0xffffff8000000000UL
#define SUNXI_IOMEM_SIZE	SZ_2G
#define SUNXI_MEM_PHYS_VASE	0xffffffc000000000UL
#define PRINT_ADDR_FMT		"0x%016lx"
#else
#define SUNXI_IO_PHYS_BASE	SUNXI_IO_PBASE
#define SUNXI_IOMEM_VASE	IO_ADDRESS(SUNXI_IO_PBASE)
#define SUNXI_IOMEM_SIZE	SUNXI_IO_SIZE
#define SUNXI_MEM_PHYS_VASE	PAGE_OFFSET
#define PRINT_ADDR_FMT		"0x%08lx"
#endif



typedef struct dump_reg {
	unsigned long pst_addr;  /* start reg addr */
	unsigned long ped_addr;  /* end reg addr   */
	void __iomem *vaddr;
} dump_reg_t;

typedef struct dump_struct {
	struct dump_reg dump;
	/* some registers' operate method maybe different */
	void __iomem *(*remap)(phys_addr_t phys_addr, size_t size);
	void (*unmap)(void __iomem *addr);
	void __iomem *(*phys2virt)(struct dump_reg *dump, unsigned long addr);
	u32 (*read)(void __iomem *addr);
	void (*write)(u32 val, void __iomem *addr);
} dump_struct_t;

/* for read and write in byte/word mode */
static unsigned int rw_byte_mode = 0;

/* for dump_reg class */
static struct dump_reg dump_para;
static struct write_group *wt_group;
static struct compare_group *cmp_group;

static u32 READ(void __iomem *addr)
{
	if (rw_byte_mode)
		return readb(addr);
	else
		return readl(addr);
}

static void WRITE(u32 val, void __iomem *addr)
{
	if (rw_byte_mode)
		writeb(val, addr);
	else
		writel(val, addr);
}

static void __iomem *REMAPIO(phys_addr_t phys_addr, size_t size)
{
	pr_debug("%s,%d, addr:%p, size:%zx\n", __func__, __LINE__, \
	         (void *)phys_addr, size);

	return ioremap(phys_addr, size);
}

static void UNMAPIO(void __iomem *addr)
{
	pr_debug("%s,%d, addr:%p\n", __func__, __LINE__, addr);

	iounmap(addr);
}

static void __iomem *REMAPMEM(phys_addr_t phys_addr, size_t size)
{
	pr_debug("%s,%d, addr:%p, size:%zx\n", __func__, __LINE__, \
	         (void *)phys_addr, size);

	return (void __iomem *)phys_to_virt(phys_addr);
}

static void __iomem *GET_VADDR(struct dump_reg *dump, unsigned long addr)
{
	unsigned long offset;
	unsigned long raddr;

	raddr = (unsigned long)(dump->vaddr);
	offset = addr - dump->pst_addr;

	raddr = raddr + offset;

	return (void __iomem *)raddr;
}

static const struct dump_struct dump_table[] = {
	{
		.dump = {
			.pst_addr = SUNXI_IO_PHYS_BASE,
			.ped_addr = SUNXI_IO_PHYS_BASE + SUNXI_IO_SIZE,
			.vaddr = NULL,
		},
		.remap = REMAPIO,
		.unmap = UNMAPIO,
		.phys2virt = GET_VADDR,
		.read = READ,
		.write = WRITE,
	},
	{
		.dump = {
			.pst_addr = PLAT_PHYS_OFFSET,
			.ped_addr = PLAT_PHYS_OFFSET + SZ_1G,
			.vaddr = NULL,
		},
		.remap = REMAPMEM,
		.unmap = NULL,
		.phys2virt = GET_VADDR,
		.read = READ,
		.write = WRITE,
	},
	{
		.dump = {
			.pst_addr = (unsigned long)SUNXI_IOMEM_VASE,
			.ped_addr = (unsigned long)SUNXI_IOMEM_VASE + SUNXI_IOMEM_SIZE,
			.vaddr = NULL,
		},
		.remap = NULL,
		.unmap = NULL,
		.phys2virt = GET_VADDR,
		.read = READ,
		.write = WRITE,
	},
	{
		.dump = {
			.pst_addr = SUNXI_MEM_PHYS_VASE,
			.ped_addr = SUNXI_MEM_PHYS_VASE + SZ_2G,
			.vaddr = NULL,
		},
		.remap = NULL,
		.unmap = NULL,
		.phys2virt = GET_VADDR,
		.read = READ,
		.write = WRITE,
	},
};

/**
 * __addr_valid - check if the addr is valid.
 * @addr: addr to judge.
 *
 * return index if the addr is register addr, -ENXIO if not.
 */
static int __addr_valid(unsigned long addr)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(dump_table); i++)
		if (addr >= dump_table[i].dump.pst_addr && \
		    addr < dump_table[i].dump.ped_addr)
			return i;

	return -ENXIO;
}

/**
 * __dump_regs_ex - dump a range of registers' value, copy to buf.
 * @reg: start and end address of registers.
 * @buf: store the dump info.
 *
 * return bytes written to buf, <=0 indicate err
 */
static ssize_t __dump_regs_ex(struct dump_reg *reg, char *buf, ssize_t len)
{
	int index;
	ssize_t cnt = 0;
	unsigned long paddr;
	const struct dump_struct *dump;

	reg->pst_addr &= (~0x3UL);
	reg->ped_addr &= (~0x3UL);

	index = __addr_valid(reg->pst_addr);
	if ((index < 0) || (index != __addr_valid(reg->ped_addr)) || \
	    (NULL == buf)) {
		pr_err("%s,%d err, invalid para, index:%d, start:0x%lx, " \
		       "end:0x%lx, buf:0x%p\n", __func__, __LINE__, index, \
		       reg->pst_addr, reg->ped_addr, buf);
		return -EIO;
	}

	dump = &dump_table[index];
	if (dump->remap)
		reg->vaddr = dump->remap(reg->pst_addr, reg->ped_addr - \
		                         reg->pst_addr + sizeof(unsigned long));
	else
		reg->vaddr = (void __iomem *)reg->pst_addr;

	/* only one to dump, so, we did not print address for
	 * open("/sys/class/...") app call
	 */
	if (reg->pst_addr == reg->ped_addr) {
		cnt = sprintf(buf, "0x%08x\n", dump->read(reg->vaddr));
		goto out;
	}

	for (paddr = (reg->pst_addr & ~0x0F); paddr <= reg->ped_addr; paddr += 4) {
		if (!(paddr & 0x0F))
			cnt += snprintf(buf + cnt, len - cnt, "\n"PRINT_ADDR_FMT":", paddr);

		if (cnt >= len) {
			pr_warn("Range too large, strings buffer overflow\n");
			cnt = len;
			goto out;
		}

		if (paddr < reg->pst_addr)
			/* "0x12345678 ", 11 space */
			cnt += snprintf(buf + cnt, len - cnt,"           ");
		else
			cnt += snprintf(buf + cnt, len - cnt, " 0x%08x", \
			               dump->read(dump->phys2virt(reg, paddr)));
	}
	cnt += snprintf(buf + cnt, len - cnt, "\n");
	pr_debug("%s,%d, start:0x%lx, end:0x%lx, return:%zd\n", __func__, \
	         __LINE__, reg->pst_addr, reg->ped_addr, cnt);

out:
	if (dump->unmap)
		dump->unmap(reg->vaddr);

	return cnt;
}

/**
 * __parse_dump_str - parse the input string for dump attri.
 * @buf: the input string, eg: "0x01c20000,0x01c20300".
 * @size: buf size.
 * @start: store the start reg's addr parsed from buf, eg 0x01c20000.
 * @end: store the end reg's addr parsed from buf, eg 0x01c20300.
 *
 * return 0 if success, otherwise failed.
 */
static int __parse_dump_str(const char *buf, size_t size, \
                            unsigned long *start, unsigned long *end)
{
	char *ptr = NULL;
	char *ptr2 = (char *)buf;
	int ret = 0, times = 0;

	/* Support single address mode, some time it haven't ',' */
next:
	/*
	 * Default dump only one register(*start =*end).
	 * If ptr is not NULL, we will cover the default value of end.
	 */
	if (times == 1)
		*start = *end;

	if (!ptr2 || (ptr2 - buf) >= size)
		goto out;

	ptr = ptr2;
	ptr2 = strnchr(ptr, size - (ptr - buf), ',');
	if (ptr2) {
		*ptr2 = '\0';
		ptr2++;
	}

	ptr = strim(ptr);
	if (!strlen(ptr))
		goto next;

	ret = strict_strtoul(ptr, 16, end);
	if (!ret){
		times++;
		goto next;
	} else
		pr_warn("String syntax errors: \"%s\"\n", ptr);

out:
	return ret;
}

/**
 * __write_show - dump a register's value, copy to buf.
 * @pgroup: the addresses to read.
 * @buf: store the dump info.
 *
 * return bytes written to buf, <=0 indicate err.
 */
static ssize_t __write_show(struct write_group *pgroup, char *buf, ssize_t len)
{
#ifdef CONFIG_ARM64
#define WR_PRINT_FMT "reg                 to_write    after_write \n"
#else
#define WR_PRINT_FMT "reg         to_write    after_write \n"
#endif
#define WR_DATA_FMT PRINT_ADDR_FMT"  0x%08x  %s"

	int i = 0;
	ssize_t cnt = 0;
	unsigned long reg = 0;
	u32 val;
	u8 rval_buf[16];
	struct dump_reg dump_reg;

	if (!pgroup) {
		pr_err("%s,%d err, pgroup is NULL!\n", __func__, __LINE__);
		goto end;
	}

	cnt += snprintf(buf, len - cnt, WR_PRINT_FMT);
	if (cnt > len) {
		cnt = -EINVAL;
		goto end;
	}

	for (i = 0; i < pgroup->num; i++) {
		reg = pgroup->pitem[i].reg_addr;
		val = pgroup->pitem[i].val;
		dump_reg.pst_addr = reg;
		dump_reg.ped_addr = reg;
		if (__dump_regs_ex(&dump_reg, rval_buf, sizeof(rval_buf)) < 0)
			return -EINVAL;

		cnt += snprintf(buf + cnt, len - cnt, WR_DATA_FMT, reg, val, rval_buf);
		if (cnt > len) {
			cnt = len;
			goto end;
		}
	}

end:
	return cnt;
}

/**
 * __parse_write_str - parse the input string for write attri.
 * @str: string to be parsed, eg: "0x01c20818 0x55555555".
 * @reg_addr: store the reg address. eg: 0x01c20818.
 * @val: store the expect value. eg: 0x55555555.
 *
 * return 0 if success, otherwise failed.
 */
static int __parse_write_str(char *str, unsigned long *reg_addr, u32 *val)
{
	char *ptr = str;
	char *tstr = NULL;
	int ret = 0;

	/*
	 * Skip the leading whitespace, find the true split symbol.
	 * And it must be 'address value'.
	 */
	tstr = strim(str);
	ptr = strchr(tstr, ' ');
	if (!ptr)
		return -EINVAL;

	/*
	 * Replaced split symbol with a %NUL-terminator temporary.
	 * Will be fixed at end.
	 */
	*ptr = '\0';
	ret = strict_strtoul(tstr, 16, reg_addr);
	if (ret)
		goto out;

	ret = kstrtou32(skip_spaces(ptr + 1), 16, val);

out:
	return ret;
}

/**
 * __write_item_init - init for write attri. parse input string,
 *                     and construct write struct.
 * @ppgroup: store the struct allocated, the struct contains items parsed from
 *           input buf.
 * @buf: input string, eg: "0x01c20800 0x00000031,0x01c20818 0x55555555,...".
 * @size: buf size.
 *
 * return 0 if success, otherwise failed.
 */
static int __write_item_init(struct write_group **ppgroup, const char *buf, \
                             size_t size)
{
	char *ptr, *ptr2;
	unsigned long addr = 0;
	u32 val;
	struct write_group *pgroup;

	/* alloc item buffer */
	pgroup = kmalloc(sizeof(struct write_group), GFP_KERNEL);
	if (!pgroup)
		return -ENOMEM;

	pgroup->pitem = kmalloc(sizeof(struct write_item) * MAX_WRITE_ITEM, \
	                        GFP_KERNEL);
	if (!pgroup->pitem) {
		kfree(pgroup);
		return -ENOMEM;
	}

	pgroup->num = 0;
	ptr = (char *)buf;
	do {
		ptr2 = strchr(ptr, ',');
		if (ptr2)
			*ptr2 = '\0';

		if (!__parse_write_str(ptr, &addr, &val)) {
			pgroup->pitem[pgroup->num].reg_addr = addr;
			pgroup->pitem[pgroup->num].val = val;
			pgroup->num++;
		} else
			pr_err("%s: Failed to parse string: %s\n", __func__, ptr);

		if (!ptr2)
			break;

		ptr = ptr2 + 1;
		*ptr2 = ',';

	} while(pgroup->num <= MAX_WRITE_ITEM);

	/* free buffer if no valid item */
	if (0 == pgroup->num) {
		kfree(pgroup->pitem);
		kfree(pgroup);
		return -EINVAL;
	}

	*ppgroup = pgroup;
	return 0;
}

/**
 * __write_item_deinit - reled_addrse memory that cred_addrted by
 *                       __write_item_init.
 * @pgroup: the write struct allocated in __write_item_init.
 */
static void __write_item_deinit(struct write_group *pgroup)
{
	if (NULL != pgroup) {
		if(NULL != pgroup->pitem)
			kfree(pgroup->pitem);
		kfree(pgroup);
	}
}

/**
 * __compare_regs_ex - dump a range of registers' value, copy to buf.
 * @pgroup: addresses of registers.
 * @buf: store the dump info.
 *
 * return bytes written to buf, <= 0 indicate err.
 */
static ssize_t __compare_regs_ex(struct compare_group *pgroup, char *buf, ssize_t len)
{
#ifdef CONFIG_ARM64
#define CMP_PRINT_FMT "reg                 expect      actual      mask        result\n"
#else
#define CMP_PRINT_FMT "reg         expect      actual      mask        result\n"
#endif
#define CMP_DATAO_FMT PRINT_ADDR_FMT"  0x%08x  0x%08x  0x%08x  OK\n"
#define CMP_DATAE_FMT PRINT_ADDR_FMT"  0x%08x  0x%08x  0x%08x  ERR\n"

	int i;
	ssize_t cnt = 0;
	unsigned long reg;
	u32 expect, actual, mask;
	u8 actualb[16];
	struct dump_reg dump_reg;

	if (!pgroup) {
		pr_err("%s,%d err, pgroup is NULL!\n", __func__, __LINE__);
		goto end;
	}

	cnt += snprintf(buf, len - cnt, CMP_PRINT_FMT);
	if (cnt > len) {
		cnt = -EINVAL;
		goto end;
	}

	for (i = 0; i < pgroup->num; i++) {
		reg = pgroup->pitem[i].reg_addr;
		expect = pgroup->pitem[i].val_expect;
		dump_reg.pst_addr = reg;
		dump_reg.ped_addr = reg;
		if (__dump_regs_ex(&dump_reg, actualb, sizeof(actualb)) < 0)
			return -EINVAL;

		if (kstrtou32(actualb, 16, &actual))
			return -EINVAL;

		mask = pgroup->pitem[i].val_mask;
		if ((actual & mask) == (expect & mask))
			cnt += snprintf(buf + cnt, len - cnt, CMP_DATAO_FMT, reg, \
				       expect, actual, mask);
		else
			cnt += snprintf(buf + cnt, len - cnt, CMP_DATAE_FMT, reg, \
			               expect, actual, mask);
		if (cnt > len) {
			cnt = -EINVAL;
			goto end;
		}
	}

end:
	return cnt;
}

/**
 * __compare_item_deinit - reled_addrse memory that cred_addrted by
 *                         __compare_item_init.
 * @pgroup: the compare struct allocated in __compare_item_init.
 */
static void __compare_item_deinit(struct compare_group *pgroup)
{
	if (pgroup) {
		if(pgroup->pitem)
			kfree(pgroup->pitem);
		kfree(pgroup);
	}
}

/**
 * __parse_compare_str - parse the input string for compare attri.
 * @str: string to be parsed, eg: "0x01c20000 0x80000011 0x00000011".
 * @reg_addr: store the reg address. eg: 0x01c20000.
 * @val_expect: store the expect value. eg: 0x80000011.
 * @val_mask: store the mask value. eg: 0x00000011.
 *
 * return 0 if success, otherwise failed.
 */
static int __parse_compare_str(char *str, unsigned long *reg_addr, \
                               u32 *val_expect, u32 *val_mask)
{
	unsigned long result_addr[3] = {0};
	char *ptr = str;
	char *ptr2 = NULL;
	int i, ret = 0;

	for (i = 0; i < ARRAY_SIZE(result_addr); i++) {
		ptr = skip_spaces(ptr);
		ptr2 = strchr(ptr, ' ');
		if (ptr2)
			*ptr2 = '\0';

		ret = strict_strtoul(ptr, 16, &result_addr[i]);
		if (!ptr2)
			break;

		*ptr2 = ' ';

		if (ret)
			break;

		ptr = ptr2 + 1;
	}

	*reg_addr = result_addr[0];
	*val_expect = (u32)result_addr[1];
	*val_mask = (u32)result_addr[2];

	return ret;
}

/**
 * __compare_item_init - init for compare attri. parse input string,
 *                       and construct compare struct.
 * @ppgroup: store the struct allocated, the struct contains items parsed from
 *           input buf.
 * @buf: input string,
 *       eg: "0x01c20000 0x80000011 0x00000011,0x01c20004 0x0000c0a4 0x0000c0a0,...".
 * @size: buf size.
 *
 * return 0 if success, otherwise failed.
 */
static int __compare_item_init(struct compare_group **ppgroup, \
                               const char *buf, size_t size)
{
	char *ptr, *ptr2;
	unsigned long addr = 0;
	u32 val_expect = 0, val_mask = 0;
	struct compare_group *pgroup = NULL;

	/* alloc item buffer */
	pgroup = kmalloc(sizeof(struct compare_group), GFP_KERNEL);
	if (NULL == pgroup)
		return -EINVAL;

	pgroup->pitem = kmalloc(sizeof(struct compare_item) * MAX_COMPARE_ITEM, \
	                        GFP_KERNEL);
	if (NULL == pgroup->pitem) {
		kfree(pgroup);
		return -EINVAL;
	}

	pgroup->num = 0;

	/* get item from buf */
	ptr = (char *)buf;
	do {
		ptr2 = strchr(ptr, ',');
		if (ptr2)
			*ptr2 = '\0';

		if (!__parse_compare_str(ptr, &addr, &val_expect, &val_mask)) {
			pgroup->pitem[pgroup->num].reg_addr = addr;
			pgroup->pitem[pgroup->num].val_expect = val_expect;
			pgroup->pitem[pgroup->num].val_mask = val_mask;
			pgroup->num++;
		} else
			pr_err("%s: Failed to parse string: %s\n", __func__, ptr);

		if (!ptr2)
			break;

		*ptr2 = ',';
		ptr = ptr2 + 1;

	} while(pgroup->num <= MAX_COMPARE_ITEM);

	/* free buffer if no valid item */
	if (0 == pgroup->num) {
		kfree(pgroup->pitem);
		kfree(pgroup);
		return -EINVAL;
	}
	*ppgroup = pgroup;

	return 0;
}

/**
 * dump_show - show func of dump attribute.
 * @dev: class ptr.
 * @attr: attribute ptr.
 * @buf: the input buf which contain the start and end reg.
 *       eg: "0x01c20000,0x01c20100\n".
 *
 * return size written to the buf, otherwise failed.
 */
static ssize_t
dump_show(struct class *class, struct class_attribute *attr, char *buf)
{
	return __dump_regs_ex(&dump_para, buf, PAGE_SIZE);
}

static ssize_t
dump_store(struct class *class, struct class_attribute *attr, \
           const char *buf, size_t count)
{
	int index;
	unsigned long start_reg = 0;
	unsigned long end_reg = 0;

	if (__parse_dump_str(buf, count, &start_reg, &end_reg)) {
		pr_err("%s,%d err, invalid para!\n", __func__, __LINE__);
		goto err;
	}

	index = __addr_valid(start_reg);
	if ((index < 0) || (index != __addr_valid(end_reg))) {
		pr_err("%s,%d err, invalid para!\n", __func__, __LINE__);
		goto err;
	}

	dump_para.pst_addr = start_reg;
	dump_para.ped_addr = end_reg;
	pr_debug("%s,%d, start_reg:"PRINT_ADDR_FMT", end_reg:"PRINT_ADDR_FMT"\n", \
	         __func__, __LINE__, start_reg, end_reg);

	return count;

err:
	dump_para.pst_addr = 0;
	dump_para.ped_addr = 0;

	return -EINVAL;
}

static ssize_t
write_show(struct class *class, struct class_attribute *attr, char *buf)
{
	/* display write result */
	return __write_show(wt_group, buf, PAGE_SIZE);
}

static ssize_t
write_store(struct class *class, struct class_attribute *attr, \
            const char *buf, size_t count)
{
	int i;
	int index;
	unsigned long reg;
	u32 val;
	const struct dump_struct *dump;
	struct dump_reg write_para;

	/* free if not NULL */
	if (wt_group) {
		__write_item_deinit(wt_group);
		wt_group = NULL;
	}

	/* parse input buf for items that will be dumped */
	if (__write_item_init(&wt_group, buf, count) < 0)
		return -EINVAL;

	/**
	 * write reg
	 * it is better if the regs been remaped and unmaped only once,
	 * but we map everytime for the range between min and max address
	 * maybe too large.
	 */
	for (i = 0; i < wt_group->num; i++) {
		reg = wt_group->pitem[i].reg_addr;
		write_para.pst_addr = reg;
		val = wt_group->pitem[i].val;
		index = __addr_valid(reg);
		dump = &dump_table[index];
		if (dump->remap)
			write_para.vaddr = dump->remap(reg, 4);
		else
			write_para.vaddr = (void __iomem *)reg;
		dump->write(val, dump->phys2virt(&write_para, reg));
		if (dump->unmap)
			dump->unmap(write_para.vaddr);
	}

	return count;
}

static ssize_t
compare_show(struct class *class, struct class_attribute *attr, char *buf)
{
	/* dump the items */
	return __compare_regs_ex(cmp_group, buf, PAGE_SIZE);
}


static ssize_t
compare_store(struct class *class, struct class_attribute *attr, \
              const char *buf, size_t count)
{
	/* free if struct not null */
	if (cmp_group) {
		__compare_item_deinit(cmp_group);
		cmp_group = NULL;
	}

	/* parse input buf for items that will be dumped */
	if (__compare_item_init(&cmp_group, buf, count) < 0)
		return -EINVAL;

	return count;
}

static ssize_t
rw_byte_show(struct class *class, struct class_attribute *attr, char *buf)
{
	return sprintf(buf, "read/write mode: %d(%s)\n", rw_byte_mode, \
	                      rw_byte_mode ? "byte" : "word");
}

static ssize_t
rw_byte_store(struct class *class, struct class_attribute *attr, \
              const char *buf, size_t count)
{
	unsigned long value;
	int ret;

	ret = strict_strtoul(buf, 10, &value);
	if (!ret && (value > 1)) {
		pr_err("%s,%d err, invalid para!\n", __func__, __LINE__);
		goto out;
	}
	rw_byte_mode = value;
out:
	return count;
}

static struct class_attribute dump_class_attrs[] = {
	__ATTR(dump,    S_IWUSR | S_IRUGO,      dump_show,      dump_store),
	__ATTR(write,   S_IWUSR | S_IRUGO,      write_show,     write_store),
	__ATTR(compare, S_IWUSR | S_IRUGO,      compare_show,   compare_store),
	__ATTR(rw_byte, S_IWUSR | S_IRUGO,      rw_byte_show,   rw_byte_store),
	__ATTR_NULL,
};

static struct class dump_class = {
	.name		= "sunxi_dump",
	.owner		= THIS_MODULE,
	.class_attrs	= dump_class_attrs,
};

static int __init dump_class_init(void)
{
	int status;

	status = class_register(&dump_class);
	if(status < 0)
		pr_err("%s,%d err, status:%d\n", __func__, __LINE__, status);
	else
		pr_info("%s,%d, success\n", __func__, __LINE__);

	return status;
}
postcore_initcall(dump_class_init);

#ifdef CONFIG_DUMP_REG_MISC
/* for dump_reg misc driver */
static struct dump_reg misc_dump_para;
static struct write_group *misc_wt_group;
static struct compare_group *misc_cmp_group;

static ssize_t
misc_dump_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return __dump_regs_ex(&misc_dump_para, buf, PAGE_SIZE);
}

static ssize_t
misc_dump_store(struct device *dev, struct device_attribute *attr, \
                const char *buf, size_t size)
{
	int index;
	unsigned long start_reg = 0;
	unsigned long end_reg = 0;

	if (__parse_dump_str(buf, size, &start_reg, &end_reg)) {
		pr_err("%s,%d err, invalid para!\n", __func__, __LINE__);
		goto err;
	}

	index = __addr_valid(start_reg);
	if ((index < 0) || (index != __addr_valid(end_reg))) {
		pr_err("%s,%d err, invalid para!\n", __func__, __LINE__);
		goto err;
	}

	misc_dump_para.pst_addr = start_reg;
	misc_dump_para.ped_addr = end_reg;
	pr_debug("%s,%d, start_reg:"PRINT_ADDR_FMT", end_reg:"PRINT_ADDR_FMT"\n", \
	         __func__, __LINE__, start_reg, end_reg);

	return size;

err:
	misc_dump_para.pst_addr = 0;
	misc_dump_para.ped_addr = 0;

	return -EINVAL;
}

static ssize_t
misc_write_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	/* display write result */
	return __write_show(misc_wt_group, buf, PAGE_SIZE);
}

static ssize_t
misc_write_store(struct device *dev, struct device_attribute *attr, \
                 const char *buf, size_t size)
{
	int i;
	int index;
	unsigned long reg;
	u32 val;
	const struct dump_struct *dump;
	struct dump_reg misc_write_para;

	/* free if not NULL */
	if (misc_wt_group) {
		__write_item_deinit(misc_wt_group);
		misc_wt_group = NULL;
	}

	/* parse input buf for items that will be dumped */
	if (__write_item_init(&misc_wt_group, buf, size) < 0)
		return -EINVAL;

	/**
	 * write reg
	 * it is better if the regs been remaped and unmaped only once,
	 * but we map everytime for the range between min and max address
	 * maybe too large.
	 */
	for (i = 0; i < misc_wt_group->num; i++) {
		reg = misc_wt_group->pitem[i].reg_addr;
		misc_write_para.pst_addr = reg;
		val = misc_wt_group->pitem[i].val;
		index = __addr_valid(reg);
		dump = &dump_table[index];
		if (dump->remap)
			misc_write_para.vaddr = dump->remap(reg, 4);
		else
			misc_write_para.vaddr = (void __iomem *)reg;
		dump->write(val, dump->phys2virt(&misc_write_para, reg));
		if (dump->unmap)
			dump->unmap(misc_write_para.vaddr);
	}

	return size;
}

static ssize_t
misc_compare_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	/* dump the items */
	return __compare_regs_ex(misc_cmp_group, buf, PAGE_SIZE);
}


static ssize_t
misc_compare_store(struct device *dev, struct device_attribute *attr, \
                   const char *buf, size_t size)
{
	/* free if struct not null */
	if (misc_cmp_group) {
		__compare_item_deinit(misc_cmp_group);
		misc_cmp_group = NULL;
	}

	/* parse input buf for items that will be dumped */
	if (__compare_item_init(&misc_cmp_group, buf, size) < 0)
		return -EINVAL;

	return size;
}

static DEVICE_ATTR(dump,    S_IWUSR | S_IRUGO, misc_dump_show,    misc_dump_store);
static DEVICE_ATTR(write,   S_IWUSR | S_IRUGO, misc_write_show,   misc_write_store);
static DEVICE_ATTR(compare, S_IWUSR | S_IRUGO, misc_compare_show, misc_compare_store);

static struct attribute *misc_attributes[] = {
	&dev_attr_dump.attr,
	&dev_attr_write.attr,
	&dev_attr_compare.attr,
	NULL,
};

static struct attribute_group misc_attribute_group = {
	.name  = "rw",
	.attrs = misc_attributes,
};

static struct miscdevice dump_reg_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name  = "sunxi-reg",
};

static int __init misc_dump_reg_init(void)
{
	int err;

	pr_info("misc dump reg init\n");

	err = misc_register(&dump_reg_dev);
	if (err) {
		pr_err("dump register driver as misc device error!\n");
		goto exit;
	}

	err = sysfs_create_group(&dump_reg_dev.this_device->kobj, \
	                         &misc_attribute_group);
	if (err)
		pr_err("dump register sysfs create group failed!\n");

exit:
	return err;
}

static void __exit misc_dump_reg_exit(void)
{
	pr_info("misc dump reg exit\n");

	WARN_ON(0 != misc_deregister(&dump_reg_dev));

	sysfs_remove_group(&(dump_reg_dev.this_device->kobj), \
	                   &misc_attribute_group);
}

module_init(misc_dump_reg_init);
module_exit(misc_dump_reg_exit);
#endif

MODULE_ALIAS("dump reg driver");
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("xiafeng <xiafeng@allwinnertech.com>");
MODULE_ALIAS("platform:dump reg");
MODULE_DESCRIPTION("dump registers driver");
