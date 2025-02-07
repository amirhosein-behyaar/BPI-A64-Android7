/*
 * drivers/soc/sunxi-sid.c
 *
 * Copyright(c) 2014-2016 Allwinnertech Co., Ltd.
 *         http://www.allwinnertech.com
 *
 * Author: sunny <sunny@allwinnertech.com>
 * Author: superm <superm@allwinnertech.com>
 * Author: Matteo <duanmintao@allwinnertech.com>
 *
 * Allwinner sunxi soc chip version and chip id manager.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/err.h>
#include <linux/sunxi-smc.h>

#include <linux/sunxi-sid.h>
#include "sunxi-sid-efuse.h"

#define SID_DBG(fmt, arg...) pr_debug("%s()%d - "fmt, __func__, __LINE__, ##arg)
#define SID_ERR(fmt, arg...) pr_err("%s()%d - "fmt, __func__, __LINE__, ##arg)

#if defined(CONFIG_ARCH_SUN50I)
#define SUNXI_SECURITY_SUPPORT	1
#endif

#define SUNXI_VER_MAX_NUM	8
struct soc_ver_map {
	u32 id;
	u32 rev[SUNXI_VER_MAX_NUM];
};

#define SUNXI_SOC_ID_INDEX		1
#define SUNXI_SECURITY_ENABLE_INDEX	2
struct soc_ver_reg {
	s8 compatile[48];
	u32 offset;
	u32 mask;
	u32 shift;
};

#ifdef CONFIG_ARCH_SUN8IW10

static struct soc_ver_map soc_ver[] = {
		{1, {0, SUN8IW10P1_REV_B} },
		{0, {0, SUN8IW10P2_REV_B} },
		{3, {0, SUN8IW10P3_REV_B} },
		{2, {0, SUN8IW10P4_REV_B} },
		{11, {0, SUN8IW10P5_REV_B} },
		{10, {0, SUN8IW10P6_REV_B} },
		{7, {0, SUN8IW10P7_REV_B} },
		{6, {0, SUN8IW10P8_REV_B} },
		{0xFF, {0} },
	};

static struct soc_ver_reg soc_ver_regs[] = {
		{"allwinner,sram_ctrl", 0x24, 0x7, 0},
		{EFUSE_CHIPID_BASE,     0x64, 0xF, 0},
		{"",                    0,    0,   0},
};
#define SUNXI_SOC_ID_IN_SID

#elif defined(CONFIG_ARCH_SUN8IW11)

static struct soc_ver_map soc_ver[] = {
		{0, {SUN8IW11P1_REV_A, SUN8IW11P1_REV_A} },
		{1, {SUN8IW11P2_REV_A, SUN8IW11P2_REV_A} },
		{3, {SUN8IW11P3_REV_A, SUN8IW11P3_REV_A} },
		{5, {SUN8IW11P4_REV_A, SUN8IW11P4_REV_A} },
		{0xFF, {0} },
	};

static struct soc_ver_reg soc_ver_regs[] = {
		{EFUSE_SID_BASE, 0x88, 1, 0},
		{"",             0,    0, 0},
		{"",             0,    0, 0},
};
#elif defined(CONFIG_ARCH_SUN8IW17)
static struct soc_ver_map soc_ver[] = {
		{0, {SUN8IW17P1_REV_A} },
		{0xFF, {0} },
};
#elif defined(CONFIG_ARCH_SUN8IW5)
static struct soc_ver_map soc_ver[] = {
		{0, {SUN8IW5P1_REV_A} },
		{1, {SUN8IW5P1_REV_B} },
		{0xFF, {0} },
};

#elif defined(CONFIG_ARCH_SUN50IW1)

#define SUNXI_GET_CHIPID_BY_SMC
static struct soc_ver_map soc_ver[] = {
		{0, {SUN50IW1P1_REV_A} },
		{0xFF, {0} },
};

#elif defined(CONFIG_ARCH_SUN50IW2)

#define SUNXI_GET_CHIPID_BY_SMC
static struct soc_ver_map soc_ver[] = {
		{0, {SUN50IW2P1_REV_A} },
		{0xFF, {0} },
};

#elif defined(CONFIG_ARCH_SUN50IW3)

static struct soc_ver_map soc_ver[] = {
		{0, {SUN50IW3P1_REV_A} },
		{0xFF, {0} },
};

#elif defined(CONFIG_ARCH_SUN50IW6)

static struct soc_ver_map soc_ver[] = {
		{0, {SUN50IW6P1_REV_A} },
		{0xFF, {0} },
};

#else

static struct soc_ver_map soc_ver[] = {
		{0xFF, {0} },
};
static struct soc_ver_reg soc_ver_regs[] = {
	{"", 0,  0, 0},
	{"", 0,  0, 0},
	{"", 0,  0, 0},
};

#endif

#if defined(CONFIG_ARCH_SUN50IW1)
static struct soc_ver_reg soc_ver_regs[] = {
	{"allwinner,sram_ctrl", 0x24, 0xFF, 0},
	{"",                    0,    0,    0},
	{EFUSE_CHIPID_BASE,     0xF4, 1,    11},
};
#elif defined(CONFIG_ARCH_SUN50IW2)
static struct soc_ver_reg soc_ver_regs[] = {
	{"allwinner,sram_ctrl", 0x24, 0xFF, 0},
	{"",                    0,    0,    0},
	{EFUSE_SID_BASE,        0xA0, 0x1,  0},
};
#elif defined(CONFIG_ARCH_SUN50IW3) || defined(CONFIG_ARCH_SUN50IW6)
static struct soc_ver_reg soc_ver_regs[] = {
	{"allwinner,sram_ctrl", 0x24,  0x7, 0},
	{EFUSE_SID_BASE,        0x100, 0xF, 0},
	{EFUSE_SID_BASE,        0xA0,  0x1, 0},
};
#define SUNXI_SOC_ID_IN_SID
#elif defined(CONFIG_ARCH_SUN8IW17)

static struct soc_ver_reg soc_ver_regs[] = {
	{"allwinner,sram_ctrl", 0x24,  0x7, 0},
	{EFUSE_SID_BASE,        0x100, 0x7, 0},
	{"",                    0,     0,   0},
};
#define SUNXI_SOC_ID_IN_SID

#elif defined(CONFIG_ARCH_SUN8IW5)
static struct soc_ver_reg soc_ver_regs[] = {
	{"allwinner,prcm", 0x190,  0x1, 3},
	{"allwinner,prcm", 0x190,  0x1, 3},
	{"",               0,      0,   0},
};
#define SUNXI_SOC_ID_IN_SID

#endif

static struct soc_ver_reg soc_bin_regs[] = {
#if defined(CONFIG_ARCH_SUN8IW11)
	{EFUSE_CHIPID_BASE, 0x4, 0x7, 22},/* soc bin bit22~24 */
#elif defined(CONFIG_ARCH_SUN8IW5)
	{EFUSE_CHIPID_BASE, 0xc, 0x3f, 11},
#elif defined(CONFIG_ARCH_SUN50IW1)
	{EFUSE_CHIPID_BASE, 0x0, 0x3, 8},
#else
	{EFUSE_CHIPID_BASE, 0x0, 0x3ff, 0},/* soc bin bit0~9 */
#endif
};

static unsigned int sunxi_soc_chipid[4];
static unsigned int sunxi_serial[4];
static int sunxi_soc_secure;
static unsigned int sunxi_soc_bin;
static unsigned int sunxi_soc_ver;

static s32 sid_get_vir_base(struct device_node **pnode, void __iomem **base,
		s8 *compatible)
{
	*pnode = of_find_compatible_node(NULL, NULL, compatible);
	if (IS_ERR_OR_NULL(*pnode)) {
		SID_ERR("Failed to find \"%s\" in dts.\n", compatible);
		return -ENXIO;
	}

	*base = of_iomap(*pnode, 0); /* reg[0] must be accessable. */
	if (*base == NULL) {
		SID_ERR("Unable to remap IO\n");
		return -ENXIO;
	}
	SID_DBG("Base addr of \"%s\" is %p\n", compatible, *base);
	return 0;
}

static s32  sid_get_phy_base(struct device_node **pnode, phys_addr_t **base,
		s8 *compatible)
{
	struct resource res = {0};

	*pnode = of_find_compatible_node(NULL, NULL, compatible);
	if (IS_ERR_OR_NULL(*pnode)) {
		SID_ERR("Failed to find \"%s\" in dts.\n", compatible);
		return -ENXIO;
	}

	if (of_address_to_resource(*pnode, 0, &res)) {
		SID_ERR("Failed to get \"%s\"  base address\n", compatible);
		return -ENXIO;
	}
	*base = (phys_addr_t *)res.start;
	SID_DBG("Base addr of \"%s\" is %p\n", compatible, (void *)*base);
	return 0;
}

static s32 sid_get_base(struct device_node **pnode,
		void __iomem **base, s8 *compatible, u32 sec)
{
	if (sec == 1)
		return sid_get_phy_base(pnode,
			(phys_addr_t **)base, compatible);
	else
		return sid_get_vir_base(pnode, base, compatible);
}

static void sid_put_base(struct device_node *pnode, void __iomem *base, u32 sec)
{
	SID_DBG("base = %p, Sec = %d\n", base, sec);
	if ((sec == 0) && (base != NULL))
		iounmap(base);
	if (pnode)
		of_node_put(pnode);
}

static u32 sid_readl(void __iomem *base, u32 sec)
{
	if (sec == 0)
		return readl(base);

#ifdef CONFIG_SUNXI_SMC
	SID_DBG("Read(%p), Sec %d\n", base, sec);
	if (sunxi_soc_is_secure())
		return sunxi_smc_readl((phys_addr_t)base);
	else
#endif
		return readl(base);
}

static u32 sid_rd_bits(s8 *name, u32 offset, u32 shift, u32 mask, u32 sec)
{
	u32 value = 0;
	void __iomem *baseaddr = NULL;
	struct device_node *dev_node = NULL;

	if (sid_get_base(&dev_node, &baseaddr, name, sec))
		return 0;

	value = sid_readl(baseaddr + offset, sec);

	value = (value >> shift) & mask;
	SID_DBG("Read \"%s\" + %#x, shift %#x, mask %#x, return %#x, Sec %d\n",
			name, offset, shift, mask, value, sec);

	sid_put_base(dev_node, baseaddr, sec);
	return value;
}

void sid_rd_ver_reg(u32 id)
{
	s32 i = 0;
	u32 ver = 0;
	struct soc_ver_reg *reg = &soc_ver_regs[0];

	ver = sid_rd_bits(reg->compatile, reg->offset,
		reg->shift, reg->mask, 0);
	WARN_ON(ver >= SUNXI_VER_MAX_NUM/2);
	BUG_ON(ver >= SUNXI_VER_MAX_NUM);

	sunxi_soc_ver = soc_ver[0].rev[ver];
	for (i = 0; soc_ver[i].id != 0xFF; i++)
		if (id == soc_ver[i].id) {
			sunxi_soc_ver = soc_ver[i].rev[ver];
			break;
		}

	SID_DBG("%d-%d: soc_ver %#x\n", i, ver, sunxi_soc_ver);
}

#ifdef SUNXI_SOC_ID_IN_SID

static s32 sid_rd_soc_ver_from_sid(void)
{
	u32 id = 0;
	struct soc_ver_reg *reg = &soc_ver_regs[SUNXI_SOC_ID_INDEX];

	id = sid_rd_bits(reg->compatile, reg->offset, reg->shift, reg->mask, 0);
	sid_rd_ver_reg(id);

	return 0;
}
#else

/* SMP_init maybe call this function, while CCU module wasn't inited.
   So we have to read/write the CCU register directly. */
static s32 sid_rd_soc_ver_from_ce(void)
{
	s32 ret = 0;
	u32 id = 0;
	void __iomem *ccu_base = NULL;
	struct device_node *ccu_node = NULL;
	u32 bus_clk_reg, bus_rst_reg, ce_clk_reg;

	ret = sid_get_base(&ccu_node, &ccu_base, "allwinner,sunxi-clk-init", 0);
	if (ret)
		return ret;

	/* backup ce clock */
	bus_clk_reg = readl(ccu_base + 0x060);
	bus_rst_reg = readl(ccu_base + 0x2c0);
	ce_clk_reg  = readl(ccu_base + 0x09c);

	if ((bus_clk_reg&(1<<5)) && (bus_rst_reg&(1<<5))
			&& (ce_clk_reg&(1<<31))) {
		SID_DBG("The CE module is already enable.\n");
	} else {
		/* enable ce clock */
		writel(bus_clk_reg | (1<<5), ccu_base + 0x060);
		writel(bus_rst_reg | (1<<5), ccu_base + 0x2c0);
		writel(ce_clk_reg | (1<<31), ccu_base + 0x09c);
	}

	id = sid_rd_bits("allwinner,sunxi-ce", 4, 0, 7, 0);

	/* restore ce clock */
	writel(bus_clk_reg, ccu_base + 0x060);
	writel(bus_rst_reg, ccu_base + 0x2c0);
	writel(ce_clk_reg,  ccu_base + 0x09c);

	sid_rd_ver_reg(id);

	sid_put_base(ccu_node, ccu_base, 0);
	return ret;
}

#endif

static void sid_soc_ver_init(void)
{
	static s32 init_flag;

	if (init_flag == 1) {
		SID_DBG("It's already inited.\n");
		return;
	}

#ifdef SUNXI_SOC_ID_IN_SID
	sid_rd_soc_ver_from_sid();
#else
	sid_rd_soc_ver_from_ce();
#endif

	SID_DBG("The SoC version: %#x\n", sunxi_soc_ver);
	init_flag = 1;
}

static void sid_chipid_init(void)
{
	u32 type = 0;
	static s32 init_flag;
	struct soc_ver_reg *reg;

	if (init_flag == 1) {
		SID_DBG("It's already inited.\n");
		return;
	}

	sunxi_soc_chipid[0] = sid_rd_bits(EFUSE_CHIPID_BASE, 0,
		0, 0xFFFFFFFF, sunxi_soc_is_secure());
	sunxi_soc_chipid[1] = sid_rd_bits(EFUSE_CHIPID_BASE, 4,
		0, 0xFFFFFFFF, sunxi_soc_is_secure());
	sunxi_soc_chipid[2] = sid_rd_bits(EFUSE_CHIPID_BASE, 8,
		0, 0xFFFFFFFF, sunxi_soc_is_secure());
	sunxi_soc_chipid[3] = sid_rd_bits(EFUSE_CHIPID_BASE, 12,
		0, 0xFFFFFFFF, sunxi_soc_is_secure());

	sunxi_serial[0] = sunxi_soc_chipid[3];
	sunxi_serial[1] = sunxi_soc_chipid[2];
	sunxi_serial[2] = (sunxi_soc_chipid[1] >> 16) & 0x0FFFF;

	reg = &soc_bin_regs[0];

	type = sid_rd_bits(reg->compatile, reg->offset, reg->shift,
		reg->mask, sunxi_soc_is_secure());

	switch (type) {
#if defined(CONFIG_ARCH_SUN50IW1)
	case 0b001:
		sunxi_soc_bin = 1;
		break;
	case 0b010:
		sunxi_soc_bin = 2;
		break;
#else
	case 0b001:
		sunxi_soc_bin = 1;
		break;
	case 0b011:
		sunxi_soc_bin = 2;
		break;
	case 0b111:
		sunxi_soc_bin = 3;
		break;
#endif
	default:
		break;
	}

	SID_DBG("soc bin: %d\n", sunxi_soc_bin);

	init_flag = 1;
}

void sid_rd_soc_secure_status(void)
{
#ifdef SUNXI_SECURITY_SUPPORT
#if defined(CONFIG_ARCH_SUN50IW1) && defined(CONFIG_TEE_SUPPORT)
	sunxi_soc_secure = 1;
#else
	static s32 init_flag;
	void __iomem *base = NULL;
	struct device_node *node = NULL;
	struct soc_ver_reg *reg = &soc_ver_regs[SUNXI_SECURITY_ENABLE_INDEX];

	if (init_flag == 1) {
		SID_DBG("It's already inited.\n");
		return;
	}

	if (sid_get_base(&node, &base, reg->compatile, 0))
		return;

	sunxi_soc_secure = ((readl(base + reg->offset))>>reg->shift)&reg->mask;

	sid_put_base(node, base, 0);
	init_flag = 1;
#endif /* end of #if defined(CONFIG_ARCH_SUN50IW1)... */
#endif /* end of #ifdef SUNXI_SECURITY_SUPPORT */
}

s32 sunxi_get_platform(s8 *buf, s32 size)
{
	return snprintf(buf, size, "%s", CONFIG_SUNXI_SOC_NAME);
}
EXPORT_SYMBOL(sunxi_get_platform);

/**
 * soc chipid:
 */
int sunxi_get_soc_chipid(u8 *chipid)
{
	sid_chipid_init();
	memcpy(chipid, sunxi_soc_chipid, 16);
	return 0;
}
EXPORT_SYMBOL(sunxi_get_soc_chipid);

/**
 * soc chipid serial:
 */
int sunxi_get_serial(u8 *serial)
{
	sid_chipid_init();
	memcpy(serial, sunxi_serial, 16);
	return 0;
}
EXPORT_SYMBOL(sunxi_get_serial);

/**
 * soc chipid str:
 */
int sunxi_get_soc_chipid_str(char *serial)
{
	size_t size;

	sid_chipid_init();
	size = sprintf(serial, "%08x", sunxi_soc_chipid[0] & 0x0ff);
	return size;
}
EXPORT_SYMBOL(sunxi_get_soc_chipid_str);

/**
 * soc chipid:
 */
int sunxi_soc_is_secure(void)
{
	sid_rd_soc_secure_status();
	return sunxi_soc_secure;
}
EXPORT_SYMBOL(sunxi_soc_is_secure);

/**
 * get sunxi soc bin
 *
 * return: the bin of sunxi soc, like that:
 * 0 : fail
 * 1 : slow
 * 2 : normal
 * 3 : fast
 */
unsigned int sunxi_get_soc_bin(void)
{
	sid_chipid_init();
	return sunxi_soc_bin;
}
EXPORT_SYMBOL(sunxi_get_soc_bin);

unsigned int sunxi_get_soc_ver(void)
{
	sid_soc_ver_init();
	return sunxi_soc_ver;
}
EXPORT_SYMBOL(sunxi_get_soc_ver);

/* Return 0, unreadable; 1, readable. */
int sid_efuse_key_is_readable(struct efuse_key_map *key_map)
{
#ifndef EFUSE_HAS_NO_RW_PROTECT
	u32 value = 0;
#endif

#ifndef EFUSE_IS_PUBLIC
	if ((key_map->public == 0) && sunxi_soc_is_secure())
		return 0;
#endif

#ifndef EFUSE_HAS_NO_RW_PROTECT
	if (key_map->read_flag_shift < 0)
		return 1;

	/* TODO: Check the protection of wr/rd_protect register. */
	value = sid_rd_bits(EFUSE_CHIPID_BASE, key_map_rd_pro.offset,
			0, 0xFFFFFFFF, sunxi_soc_is_secure());
	if ((value >> key_map->read_flag_shift) & 1) {
		SID_ERR("The key %s is unreadable!\n", key_map->name);
		return 0;
	}

	value = sid_rd_bits(EFUSE_CHIPID_BASE, key_map_wr_pro.offset,
			0, 0xFFFFFFFF, sunxi_soc_is_secure());
	if (((value >> key_map->burn_flag_shift) & 1) == 0) {
		SID_ERR("The key %s has not been burned.\n", key_map->name);
		return 0;
	}
#endif
	return 1;
}

void sid_efuse_key_rd(struct efuse_key_map *key_map, void *buf, u32 n)
{
	u32 i;
	u32 value = 0;
	u32 key_size = 0;
	u32 remainder = n;
	u32 *dst = (u32 *)buf;

	key_size = key_map->size / 8;
	BUG_ON(key_size & 3);
	if (remainder > key_size)
		remainder = key_size;

	SID_DBG("Read the key %s, len: %d\n", key_map->name, remainder);
	for (i = 0; i < key_size/4; i++) {
		value = sid_rd_bits(EFUSE_CHIPID_BASE, key_map->offset + i*4,
				0, 0xFFFFFFFF, sunxi_soc_is_secure());
		pr_debug("0x%04x: 0x%08x\n", key_map->offset + i*4, value);
		if (remainder <= 4) {
			memcpy(&dst[i], &value, remainder);
			break;
		}
		dst[i] = value;
		remainder -= 4;
	}
	pr_debug("\n");
}

/* n - the size of read_buf, in bytes */
s32 sunxi_efuse_readn(void *key_name, void *buf, u32 n)
{
	struct efuse_key_map *key_map = key_maps;

	if ((key_name == NULL) || (*(s8 *)key_name == 0)
			|| (n == 0) || (buf == NULL)) {
		SID_ERR("Invalid parameter. name: %p, read_buf: %p, size: %d\n",
				key_name, buf, n);
		return -EINVAL;
	}
	WARN_ON(n < 4);

	/* Confirm the readable attribute. */
	for (; key_map->name[0] != 0; key_map++) {
		if (strncmp(key_name, key_map->name,
							strlen(key_map->name)))
			continue;

		SID_DBG("Read key: %s, offset: %#x\n",
					key_map->name, key_map->offset);
		if (sid_efuse_key_is_readable(key_map))
			break;
		else
			return -EACCES;
	}

	if (key_map->name[0] == 0) {
		SID_ERR("The key %s is unavailable.\n", (s8 *)key_name);
		return -1;
	}

	sid_efuse_key_rd(key_map, buf, n);
	return 0;
}
EXPORT_SYMBOL(sunxi_efuse_readn);

