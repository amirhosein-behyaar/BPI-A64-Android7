/*
 * (C) Copyright 2007-2013
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 * Jerry Wang <wangflord@allwinnertech.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#include <common.h>
#include <malloc.h>
#include <stdarg.h>
#include <asm/arch/dma.h>
#include <sys_config.h>
#include <smc.h>
#include <fdt_support.h>
#include <sys_config_old.h>
DECLARE_GLOBAL_DATA_PTR;

//#define get_wvalue(addr)	(*((volatile unsigned long  *)(addr)))
//#define put_wvalue(addr, v)	(*((volatile unsigned long  *)(addr)) = (unsigned long)(v))
#define  NAND_DRV_VERSION_0		0x03
#define  NAND_DRV_VERSION_1		0x5002
#define  NAND_DRV_DATE			0x20160512
#define  NAND_DRV_TIME			0x1716

#define NAND_CLK_BASE_ADDR (0x03001000)
#define NAND_PIO_BASE_ADDRESS (0x0300B000)

extern int sunxi_get_securemode(void);
__u32 get_wvalue(__u32 addr)
{
	return (smc_readl(addr));
}

void put_wvalue(__u32 addr,__u32 v)
{
	smc_writel(v, addr);
}

__u32 NAND_GetNdfcVersion(void);
void * NAND_Malloc(unsigned int Size);
void NAND_Free(void *pAddr, unsigned int Size);
int NAND_Get_Version(void);
static __u32 boot_mode;
//static __u32 gpio_hdl;

int NAND_set_boot_mode(__u32 boot)
{
	boot_mode = boot;
	return 0;
}


int NAND_Print(const char * str, ...)
{
	if(boot_mode)
		return 0;
	else
	{
		static char _buf[1024];
		va_list args;

		va_start(args, str);
		vsprintf(_buf, str, args);

		tick_printf(_buf);
		return 0;
	}

}

int NAND_Print_DBG(const char * str, ...)
{
	if(boot_mode)
		return 0;
	else
	{
		static char _buf[1024];
		va_list args;

		va_start(args, str);
		vsprintf(_buf, str, args);

		tick_printf(_buf);
		return 0;
	}
}

__s32 NAND_CleanFlushDCacheRegion(void *buff_addr, __u32 len)
{
	flush_cache((ulong)buff_addr, len);
	return 0;
}

void *NAND_DMASingleMap(__u32 rw, void *buff_addr, __u32 len)
{
	return buff_addr;
}

void *NAND_DMASingleUnmap(__u32 rw, void *buff_addr, __u32 len)
{
	return buff_addr;
}

__s32 NAND_AllocMemoryForDMADescs(__u32 *cpu_addr, __u32 *phy_addr)
{
	void *p = NULL;
	p = (void *)NAND_Malloc(1024);

	if (p == NULL) {
		printf("NAND_AllocMemoryForDMADescs(): alloc dma des failed\n");
		return -1;
	} else {
		*cpu_addr = (__u32)p;
		*phy_addr = (__u32)p;
		printf("NAND_AllocMemoryForDMADescs(): cpu: 0x%x    physic: 0x%x\n",
			*cpu_addr, *phy_addr);
	}

	return 0;
}

__s32 NAND_FreeMemoryForDMADescs(__u32 *cpu_addr, __u32 *phy_addr)
{
	NAND_Free((void *)(*cpu_addr), 1024);
	*cpu_addr = 0;
	*phy_addr = 0;

	return 0;
}

int NAND_WaitDmaFinish(void)
{
	return 0;
}

__u32 _Getpll6Clk(void)
{
	__u32 reg_val;
	__u32 factor_n;
	__u32 factor_m0;
	__u32 factor_m1;
	__u32 clock;

	reg_val  = get_wvalue(NAND_CLK_BASE_ADDR + 0x20);
	factor_n = ((reg_val >> 8) & 0xFF) + 1;
	factor_m0 = ((reg_val >> 0) & 0x1) + 1;
	factor_m1 = ((reg_val >> 1) & 0x1) + 1;
	//div_m = ((reg_val >> 0) & 0x3) + 1;

	clock = 24000000 * factor_n / factor_m0/factor_m1/4;
	//NAND_Print("pll6 clock is %d Hz\n", clock);
	//if(clock != 600000000)
	//printf("pll6 clock rate error, %d!!!!!!!\n", clock);

	return clock;
}

__s32 _get_ndfc_clk_v1(__u32 nand_index, __u32 *pdclk)
{
	__u32 sclk0_reg_adr;
	__u32 sclk_src, sclk_src_sel;
	__u32 sclk_pre_ratio_n, sclk_ratio_m;
	__u32 reg_val, sclk0;

	if (nand_index > 1) {
		printf("wrong nand id: %d\n", nand_index);
		return -1;
	}

	if (nand_index == 0) {
		sclk0_reg_adr = (NAND_CLK_BASE_ADDR + 0x0810); //CCM_NAND0_CLK0_REG;
	} else if (nand_index == 1) {
		sclk0_reg_adr = (NAND_CLK_BASE_ADDR + 0x0814); //CCM_NAND1_CLK0_REG;
	}

	// sclk0
	reg_val = get_wvalue(sclk0_reg_adr);
	sclk_src_sel     = (reg_val>>24) & 0x7;
	sclk_pre_ratio_n = (reg_val>>8) & 0x3;;
	sclk_ratio_m     = (reg_val) & 0xf;

	if (sclk_src_sel == 0)
		sclk_src = 24;
	else if(sclk_src_sel < 0x3)
		sclk_src = _Getpll6Clk()/1000000;
	else
		sclk_src = 2*_Getpll6Clk()/1000000;

	sclk0 = (sclk_src >> sclk_pre_ratio_n) / (sclk_ratio_m+1);

	if (nand_index == 0) {
		//NAND_Print("Reg 0x01c20080: 0x%x\n", get_wvalue(0x01c20080));
	} else {
		//NAND_Print("Reg 0x01c20084: 0x%x\n", get_wvalue(0x01c20084));
	}
	//NAND_Print("NDFC%d:  sclk0(2*dclk): %d MHz\n", nand_index, sclk0);

	*pdclk = sclk0/2;

	return 0;
}

__s32 _change_ndfc_clk_v1(__u32 nand_index, __u32 dclk_src_sel, __u32 dclk)
{
	__u32 edo_clk;
	__u32 cfg;
	__u32 m = 0, n = 0;
	__u32 clk_src;


	if(0==nand_index)
		clk_src = ((*(volatile __u32 *)(NAND_CLK_BASE_ADDR + 0x0810))>>24)&0x07;
	else
		clk_src = ((*(volatile __u32 *)(NAND_CLK_BASE_ADDR + 0x0814))>>24)&0x07;
	if(clk_src == 0)
	{
	}
	else
	{
		edo_clk = dclk * 2;

		if(edo_clk <= 20)  //10M
		{
			n =  3;
			m = 14;
		}
		else if((edo_clk >20)&&(edo_clk <= 40))  //20M
		{
			n =  2;
			m = 14;
		}
		else if((edo_clk >40)&&(edo_clk <= 50))  //25M
		{
			n =  1;
			m = 13;
		}
		else if((edo_clk >50)&&(edo_clk <= 60))  //30M
		{
			n = 2;
			m = 9;
		}
         else if((edo_clk >60)&&(edo_clk <= 80))  //40M
         {
			n = 1;
			m = 14;
		}else if((edo_clk >80)&&(edo_clk <= 100))  //50M
         {
			n = 2;
			m = 5;
		}
         else if((edo_clk >100)&&(edo_clk <= 120))  //60M
         {
			n = 1;
			m = 9;
		}
		else //40M
		{
			n = 1;
			m = 14;
		}
		//if the source clock is not 2x ,
		if(clk_src<3)
		{
			n--;
		}
	}


	if(nand_index == 0)
	{
		/*set nand clock*/
		/*set nand clock gate on*/
		cfg = *(volatile __u32 *)(NAND_CLK_BASE_ADDR + 0x0810);
		cfg &= (~(0x03 << 8));
		cfg &= (~(0xf));
		cfg |= ((n&0x3)<<8);
		cfg |= ((m&0xf));
		*(volatile __u32 *)(NAND_CLK_BASE_ADDR + 0x0810) = cfg;

		//NAND_Print("NAND_SetClk, nand_index: 0x%x\n", nand_index);
		//NAND_Print("Reg 0x0810: 0x%x\n", *(volatile __u32 *)(NAND_CLK_BASE_ADDR + 0x0810));

	}
	else
	{
         /*set nand clock*/
		/*set nand clock gate on*/
		cfg = *(volatile __u32 *)(NAND_CLK_BASE_ADDR + 0x0814);
		cfg &= (~(0x03 << 8));
		cfg &= (~(0xf));
		cfg |= ((n&0x3)<<8);
		cfg |= ((m&0xf));
		*(volatile __u32 *)(NAND_CLK_BASE_ADDR + 0x0814) = cfg;

		NAND_Print("NAND_SetClk, nand_index: 0x%x\n", nand_index);
		NAND_Print("Reg 0x08140: 0x%x\n", *(volatile __u32 *)(NAND_CLK_BASE_ADDR + 0x0814));
	}

	return 0;
}

__s32 _close_ndfc_clk_v1(__u32 nand_index)
{
	u32 reg_val;

	if (nand_index == 0) {
		//disable nand sclock gating
		reg_val = *(volatile __u32 *)(NAND_CLK_BASE_ADDR + 0x0810);
		//disable sclock gating
		reg_val &= (~(0x1U<<31));
		*(volatile __u32 *)(NAND_CLK_BASE_ADDR + 0x0810) = reg_val;
	} else if (nand_index == 1) {

		//disable nand sclock gating
		reg_val = *(volatile __u32 *)(NAND_CLK_BASE_ADDR + 0x0814);
		//disable sclock gating
		reg_val &= (~(0x1U<<31));
		*(volatile __u32 *)(NAND_CLK_BASE_ADDR + 0x0814) = reg_val;
	} else {
		printf("close_ndfc_clk error, wrong nand index: %d\n", nand_index);
		return -1;
	}

	return 0;
}


__s32 _open_ndfc_ahb_gate_and_reset_v1(__u32 nand_index)
{
	__u32 reg_val=0;

	/*
	1. release ahb reset and open ahb clock gate for ndfc version 1.
	*/
	if (nand_index == 0) {
		// ahb clock gate
		reg_val = *(volatile __u32 *)(NAND_CLK_BASE_ADDR + 0x082C);
		reg_val &= (~(0x1U<<16));
		reg_val |= (0x1U<<16);
		reg_val &= (~(0x1U<<0));
		reg_val |= (0x1U<<0);
		*(volatile __u32 *)(NAND_CLK_BASE_ADDR + 0x082C) = reg_val;

		//enable nand0  mclock gating
		reg_val = *(volatile __u32 *)(NAND_CLK_BASE_ADDR + 0x0804);
		reg_val &= (~(0x1U<<5));
		reg_val |= (0x1U<<5);
		*(volatile __u32 *)(NAND_CLK_BASE_ADDR + 0x0804) = reg_val;


		//enable nand sclock gating
		reg_val = *(volatile __u32 *)(NAND_CLK_BASE_ADDR + 0x0810);

		//enable sclock gating
		reg_val &= (~(0x1U<<31));
		reg_val |= (0x1U<<31);
		//sclect the source clk :PLL_PREIO2x
		reg_val &= (~(0x7U<<24));
		reg_val |= (0x3U<<24);

		//the PLL_preio2x=1200M, setting the nand clock =10M ,
		reg_val &= (~(0x3U<<8));
		reg_val |= (0x3U<<8);

		reg_val &= (~(0xFU<<0));
		reg_val |= (0x0E<<0);

		*(volatile __u32 *)(NAND_CLK_BASE_ADDR + 0x0810) = reg_val;

	} else {
		printf("open ahb gate and reset, wrong nand index: %d\n", nand_index);
		return -1;
	}

	return 0;
}

__s32 _close_ndfc_ahb_gate_and_reset_v1(__u32 nand_index)
{
	__u32 reg_val=0;

	/*
	1. release ahb reset and open ahb clock gate for ndfc version 1.
	*/
	if (nand_index == 0) {

		reg_val = *(volatile __u32 *)(NAND_CLK_BASE_ADDR + 0x082C);
		reg_val &= (~(0x1U<<16));
		reg_val &= (~(0x1U<<0));
		//reg_val |= (0x1U<<0);
		*(volatile __u32 *)(NAND_CLK_BASE_ADDR + 0x082C) = reg_val;

		//disable nand0  mclock gating
		reg_val = *(volatile __u32 *)(NAND_CLK_BASE_ADDR + 0x0804);
		reg_val &= (~(0x1U<<5));
		//reg_val |= (0x1U<<5);
		*(volatile __u32 *)(NAND_CLK_BASE_ADDR + 0x0804) = reg_val;


		//disable nand sclock gating
		reg_val = *(volatile __u32 *)(NAND_CLK_BASE_ADDR + 0x0810);

		//disable sclock gating
		reg_val &= (~(0x1U<<31));
		//reg_val |= (0x1U<<31);
		//sclect the source clk :PLL_PREIO2x
		*(volatile __u32 *)(NAND_CLK_BASE_ADDR + 0x0810) = reg_val;

	} else {
		printf("close ahb gate and reset, wrong nand index: %d\n", nand_index);
		return -1;
	}
	return 0;
}


__s32 _cfg_ndfc_gpio_v1(__u32 nand_index)
{
	__u32 cfg;
	if (nand_index == 0) {
		//setting PC0 port as Nand control line
		*(volatile __u32 *)(NAND_PIO_BASE_ADDRESS + 0x48) = 0x22222222;
		//setting PC1 port as Nand data line
		*(volatile __u32 *)(NAND_PIO_BASE_ADDRESS + 0x4c) = 0x22222222;
		//setting PC2 port as Nand RB1
		cfg = *(volatile __u32 *)(NAND_PIO_BASE_ADDRESS + 0x50);
		cfg &= (~0x7);
		cfg |= 0x2;
		*(volatile __u32 *)(NAND_PIO_BASE_ADDRESS + 0x50) = cfg;

		//pull-up/down --only setting RB & CE pin pull-up
		*(volatile __u32 *)(NAND_PIO_BASE_ADDRESS + 0x64) = 0x40000440;
		cfg = *(volatile __u32 *)(NAND_PIO_BASE_ADDRESS + 0x68);
		cfg &= (~0x3);
		cfg |= 0x01;
		*(volatile __u32 *)(NAND_PIO_BASE_ADDRESS + 0x68) = cfg;

	} else {
		printf("_cfg_ndfc_gpio_v1, wrong nand index %d\n", nand_index);
		return -1;
	}

	return 0;
}

int NAND_ClkRequest(__u32 nand_index)
{
	__s32 ret = 0;
	__u32 ndfc_version = NAND_GetNdfcVersion();

	if (ndfc_version == 1) {

		if (nand_index != 0) {
			printf("NAND_ClkRequest, wrong nand index %d for ndfc version %d\n",
					nand_index, ndfc_version);
			return -1;
		}
		// 1. release ahb reset and open ahb clock gate
		_open_ndfc_ahb_gate_and_reset_v1(nand_index);

		// 2. configure ndfc's sclk0
		ret = _change_ndfc_clk_v1(nand_index, 1, 10);
		if (ret<0) {
			printf("NAND_ClkRequest, set dclk failed!\n");
			return -1;
		}

	} else {
		printf("NAND_ClkRequest, wrong ndfc version, %d\n", ndfc_version);
		return -1;
	}

	return 0;
}

void NAND_ClkRelease(__u32 nand_index)
{

	__u32 ndfc_version = NAND_GetNdfcVersion();

	if (ndfc_version == 1) {

		if (nand_index != 0) {
			printf("NAND_Clkrelease, wrong nand index %d for ndfc version %d\n",
					nand_index, ndfc_version);
			return;
		}
		_close_ndfc_clk_v1(nand_index);

		_close_ndfc_ahb_gate_and_reset_v1(nand_index);

	}
	return;

}

/*
**********************************************************************************************************************
*
*             NAND_GetCmuClk
*
*  Description:
*
*
*  Parameters:
*
*
*  Return value:
*
*
**********************************************************************************************************************
*/
int NAND_SetClk(__u32 nand_index, __u32 nand_clk0, __u32 nand_clk1)
{
	__u32 ndfc_version = NAND_GetNdfcVersion();
	__u32 dclk_src_sel, dclk;
	__s32 ret = 0;

	if (ndfc_version == 1) {
		if (nand_index != 0) {
			printf("NAND_ClkRequest, wrong nand index %d for ndfc version %d\n",
					nand_index, ndfc_version);
			return -1;
		}

		////////////////////////////////////////////////
		dclk_src_sel = 1;
		dclk = nand_clk0;
		////////////////////////////////////////////////
		ret = _change_ndfc_clk_v1(nand_index, dclk_src_sel, dclk);
		if (ret < 0) {
			printf("NAND_SetClk, change ndfc clock failed\n");
			return -1;
		}

	} else {
		printf("NAND_SetClk, wrong ndfc version, %d\n", ndfc_version);
		return -1;
	}

	return 0;
}

int NAND_GetClk(__u32 nand_index, __u32 *pnand_clk0, __u32 *pnand_clk1)
{
	__s32 ret;
	__u32 ndfc_version = NAND_GetNdfcVersion();

	if (ndfc_version == 1) {

		//NAND_Print("NAND_GetClk for nand index %d \n", nand_index);
		ret = _get_ndfc_clk_v1(nand_index, pnand_clk0);
		if (ret < 0) {
			printf("NAND_GetClk, failed!\n");
			return -1;
		}
	} else {
		printf("NAND_SetClk, wrong ndfc version, %d\n", ndfc_version);
		return -1;
	}

	return 0;
}

__s32 NAND_PIORequest(__u32 nand_index)
{
	__s32 ret = 0;
	__u32 ndfc_version = NAND_GetNdfcVersion();

	if (ndfc_version == 1) {

		NAND_Print("NAND_PIORequest for nand index %d \n", nand_index);
		ret = _cfg_ndfc_gpio_v1(nand_index);
		if (ret < 0) {
			printf("NAND_PIORequest, failed!\n");
			return -1;
		}

	} else {
		printf("NAND_PIORequest, wrong ndfc version, %d\n", ndfc_version);
		return -1;
	}
	return 0;
}

__s32 NAND_PIOFuncChange_DQSc(__u32 nand_index, __u32 en)
{
	__u32 ndfc_version;
	__u32 cfg;

	ndfc_version = NAND_GetNdfcVersion();
	if (ndfc_version == 1) {
		printf("NAND_PIOFuncChange_EnDQScREc: invalid ndfc version!\n");
		return 0;
	}

	if (ndfc_version == 2) {

		if (nand_index == 0) {
			cfg = *(volatile __u32 *)(0x06000800 + 0x50);
			cfg &= (~(0x7U<<8));
			cfg |= (0x3U<<8);
			*(volatile __u32 *)(0x06000800 + 0x50) = cfg;
		} else {
			cfg = *(volatile __u32 *)(0x06000800 + 0x128);
			cfg &= (~(0x7U<<8));
			cfg |= (0x3U<<8);
			*(volatile __u32 *)(0x06000800 + 0x128) = cfg;
		}
	}
	return 0;
}
__s32 NAND_PIOFuncChange_REc(__u32 nand_index, __u32 en)
{
	__u32 ndfc_version;
	__u32 cfg;

	ndfc_version = NAND_GetNdfcVersion();
	if (ndfc_version == 1) {
		printf("NAND_PIOFuncChange_EnDQScREc: invalid ndfc version!\n");
		return 0;
	}

	if (ndfc_version == 2) {

		if (nand_index == 0) {
			cfg = *(volatile __u32 *)(0x06000800 + 0x50);
			cfg &= (~(0x7U<<4));
			cfg |= (0x3U<<4);
			*(volatile __u32 *)(0x06000800 + 0x50) = cfg;
		} else {
			cfg = *(volatile __u32 *)(0x06000800 + 0x128);
			cfg &= (~(0x7U<<4));
			cfg |= (0x3U<<4);
			*(volatile __u32 *)(0x06000800 + 0x128) = cfg;
		}
	}

	return 0;
}

void NAND_PIORelease(__u32 nand_index)
{
	return;
}

void NAND_Memset(void* pAddr, unsigned char value, unsigned int len)
{
	memset(pAddr, value, len);
}

void NAND_Memcpy(void* pAddr_dst, void* pAddr_src, unsigned int len)
{
	memcpy(pAddr_dst, pAddr_src, len);
}


void * NAND_Malloc(unsigned int Size)
{
	void * buf;
	if(Size == 0)
	{
		printf("NAND_Malloc 0!\n");
		return NULL;
	}

	buf = malloc(Size);
	if(buf == NULL)
	{
		printf("NAND_Malloc fail!\n");
	}
	return buf;
}

void NAND_Free(void *pAddr, unsigned int Size)
{
	free(pAddr);
}



void  OSAL_IrqUnLock(unsigned int  p)
{
	;
}
void  OSAL_IrqLock  (unsigned int *p)
{
	;
}

int NAND_WaitRbReady(void)
{
	return 0;
}

void *NAND_IORemap(void *base_addr, unsigned int size)
{
	return (void *)base_addr;
}

void *NAND_VA_TO_PA(void *buff_addr)
{
	return buff_addr;
}

void *NAND_GetIOBaseAddrCH0(void)
{
	return (void*)0x04011000;
}

void *NAND_GetIOBaseAddrCH1(void)
{
	return (void*)0x04011000;
}

__u32 NAND_GetNdfcVersion(void)
{
	return 1;
}

__u32 NAND_GetNdfcDmaMode(void)
{
	/*
		0: General DMA;
		1: MBUS DMA

		Only support General DMA!!!!
	*/
	return 1;
}

int NAND_PhysicLockInit(void)
{
	return 0;
}

int NAND_PhysicLock(void)
{
	return 0;
}

int NAND_PhysicUnLock(void)
{
	return 0;
}

int NAND_PhysicLockExit(void)
{
	return 0;
}

__u32 NAND_GetMaxChannelCnt(void)
{
	return 1;
}

__u32 NAND_GetPlatform(void)
{
	return 64;
}

unsigned int dma_chan = 0;

/* request dma channel and set callback function */
int nand_request_dma(void)
{
	dma_chan = sunxi_dma_request(0);
	if (dma_chan == 0) {
		printf("uboot nand_request_dma: request genernal dma failed!\n");
		return -1;
	} else
		NAND_Print("uboot nand_request_dma: reqest genernal dma for nand success, 0x%x\n", dma_chan);

	return 0;
}
int NAND_ReleaseDMA(__u32 nand_index)
{
	printf("nand release dma:%x\n",dma_chan);
	if(dma_chan)
	{
		sunxi_dma_release(dma_chan);
		dma_chan = 0;
	}
	return 0;
}


int nand_dma_config_start(__u32 write, dma_addr_t addr,__u32 length)
{
	int ret = 0;
	sunxi_dma_setting_t dma_set;

	dma_set.loop_mode = 0;
	dma_set.wait_cyc = 8;
	dma_set.data_block_size = 0;

	if (write) {
		dma_set.cfg.src_drq_type = DMAC_CFG_SRC_TYPE_DRAM;
		dma_set.cfg.src_addr_mode = DMAC_CFG_SRC_ADDR_TYPE_LINEAR_MODE;
		dma_set.cfg.src_burst_length = DMAC_CFG_SRC_1_BURST; //8
		dma_set.cfg.src_data_width = DMAC_CFG_SRC_DATA_WIDTH_32BIT;
		dma_set.cfg.reserved0 = 0;

		dma_set.cfg.dst_drq_type = DMAC_CFG_DEST_TYPE_NAND;
		dma_set.cfg.dst_addr_mode = DMAC_CFG_DEST_ADDR_TYPE_IO_MODE;
		dma_set.cfg.dst_burst_length = DMAC_CFG_DEST_1_BURST; //8
		dma_set.cfg.dst_data_width = DMAC_CFG_DEST_DATA_WIDTH_32BIT;
		dma_set.cfg.reserved1 = 0;
	} else {
		dma_set.cfg.src_drq_type = DMAC_CFG_SRC_TYPE_NAND;
		dma_set.cfg.src_addr_mode = DMAC_CFG_SRC_ADDR_TYPE_IO_MODE;
		dma_set.cfg.src_burst_length = DMAC_CFG_SRC_1_BURST; //8
		dma_set.cfg.src_data_width = DMAC_CFG_SRC_DATA_WIDTH_32BIT;
		dma_set.cfg.reserved0 = 0;

		dma_set.cfg.dst_drq_type = DMAC_CFG_DEST_TYPE_DRAM;
		dma_set.cfg.dst_addr_mode = DMAC_CFG_DEST_ADDR_TYPE_LINEAR_MODE;
		dma_set.cfg.dst_burst_length = DMAC_CFG_DEST_1_BURST; //8
		dma_set.cfg.dst_data_width = DMAC_CFG_DEST_DATA_WIDTH_32BIT;
		dma_set.cfg.reserved1 = 0;
	}

	if ( sunxi_dma_setting(dma_chan, &dma_set) < 0) {
		printf("uboot: sunxi_dma_setting for nand faild!\n");
		return -1;
	}

	if (write)
		ret = sunxi_dma_start(dma_chan, addr, (uint)0x01c03300, length);
	else
		ret = sunxi_dma_start(dma_chan, (uint)0x01c03300, addr, length);
	if (ret < 0) {
		printf("uboot: sunxi_dma_start for nand faild!\n");
		return -1;
	}

	return 0;
}

__u32 NAND_GetNandExtPara(__u32 para_num)
{
	int nand_para;
	int ret;
	char str[9];

	str[0] = 'n';
	str[1] = 'a';
	str[2] = 'n';
	str[3] = 'd';
	str[4] = '0';
	str[5] = '_';
	str[6] = 'p';
	str[7] = '0';
	str[8] = '\0';

	if(para_num == 0)//frequency
	{
		str[7] = '0';
	}
	else if(para_num == 1)//SUPPORT_TWO_PLANE
	{
		str[7] = '1';
	}
	else if(para_num == 2)//SUPPORT_VERTICAL_INTERLEAVE
	{
		str[7] = '2';
	}
	else if(para_num == 3)//SUPPORT_DUAL_CHANNEL
	{
		str[7] = '3';
	}
	else if(para_num == 4)//frequency change
	{
		str[7] = '4';
	}
	else if(para_num == 5)
	{
		str[7] = '5';
	}
	else
	{
		printf("NAND GetNandExtPara: wrong para num: %d\n", para_num);
		return 0xffffffff;
	}

	ret = 0;
	nand_para = 0;

	ret = script_parser_fetch("nand0_para", str, &nand_para, 1);
	if(ret < 0)
	{
		printf("nand0_para, %d, nand type err! %d\n",para_num,ret);
		return 0xffffffff;
	}
	else
	{
		if(nand_para == 0x55aaaa55)
			return 0xffffffff;
		else
			return nand_para;
	}

}

__u32 NAND_GetNandIDNumCtrl(void)
{
	int id_number_ctl;
	int ret;

	ret = script_parser_fetch("nand0_para", "nand0_id_number_ctl", &id_number_ctl, 1);
	if(ret<0) {
		printf("nand : get id number_ctl fail, %x\n",id_number_ctl);
		return 0x0;
	} else {
		printf("nand : get id number_ctl from script:0x%x\n",id_number_ctl);
		if(id_number_ctl == 0x55aaaa55)
			return 0x0;
		else
			return id_number_ctl;
	}
}

void nand_cond_resched(void)
{
	;
}

__u32 NAND_GetNandCapacityLevel(void)
{
	int CapacityLevel;
	int ret;

	ret = script_parser_fetch("nand0_para", "nand0_capacity_level", &CapacityLevel, 1);
	if(ret < 0) {
		printf("nand : get CapacityLevel fail, %x\n",CapacityLevel);
		return 0x0;
	} else {
		printf("nand : get CapacityLevel from script, %x\n",CapacityLevel);
		if(CapacityLevel == 0x55aaaa55)
			return 0x0;
		else
			return CapacityLevel;
	}
}

/*****************************************************************************
*Name         :
*Description  :
*Parameter    :
*Return       :
*Note         : wait rb
*****************************************************************************/
__s32 nand_rb_wait_time_out(__u32 no,__u32* flag)
{
	return 1;
}

__s32 nand_rb_wake_up(__u32 no)
{
	return 0;
}

/*****************************************************************************
*Name         :
*Description  :
*Parameter    :
*Return       :
*Note         : wait dma
*****************************************************************************/
__s32 nand_dma_wait_time_out(__u32 no,__u32* flag)
{
	return 1;
}

__s32 nand_dma_wake_up(__u32 no)
{
	return 0;
}


//int sunxi_get_securemode(void)
//return 0:normal mode
//rerurn 1:secure mode ,but could change clock
	//return !0 && !1: secure mode ,and couldn't change clock=

int NAND_IS_Secure_sys(void)
{
	int mode=0;
	int toc_file = (gd->bootfile_mode == SUNXI_BOOT_FILE_TOC);
	mode = sunxi_get_securemode() || toc_file;
	if(mode==0) //normal mode
		return 0;
	else if((mode==1)||(mode==2))//secure
		return 1;
	return 0;
}


int NAND_GetVoltage(void)
{
	int ret=0;
	return ret;
}

int NAND_ReleaseVoltage(void)
{
	int ret = 0;
	return ret;
}

void NAND_Print_Version(void)
{
	int val[4]={0};

	val[0] = NAND_DRV_VERSION_0;
	val[1] = NAND_DRV_VERSION_1;
	val[2] = NAND_DRV_DATE;
	val[3] = NAND_DRV_TIME;
	printf("uboot: nand version: %x %x %x %x \n",val[0],val[1],val[2],val[3]);
}

int NAND_Get_Version(void)
{
	return NAND_DRV_DATE;
}
