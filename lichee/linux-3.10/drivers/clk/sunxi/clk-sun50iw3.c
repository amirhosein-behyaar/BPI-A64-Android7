/*
 * Copyright (C) 2013 Allwinnertech, kevin.z.m <kevin@allwinnertech.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <linux/clk-private.h>
#include <linux/clk-provider.h>
#include <linux/clkdev.h>
#include <linux/delay.h>
#include <linux/clk/sunxi.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include "clk-sunxi.h"
#include "clk-factors.h"
#include "clk-periph.h"
#include "clk-sun50iw3.h"
#include "clk-sun50iw3_tbl.c"

#define FACTOR_SIZEOF(name) (sizeof(factor_pll##name##_tbl)/ \
			     sizeof(struct sunxi_clk_factor_freq))

#define FACTOR_SEARCH(name) (sunxi_clk_com_ftr_sr( \
		&sunxi_clk_factor_pll_##name, factor, \
		factor_pll##name##_tbl, index, \
		FACTOR_SIZEOF(name)))

#ifndef CONFIG_EVB_PLATFORM
	#define LOCKBIT(x) 31
#else
	#define LOCKBIT(x) x
#endif

static DEFINE_SPINLOCK(clk_lock);
void __iomem *sunxi_clk_base;
void __iomem *sunxi_clk_cpus_base;
int sunxi_clk_maxreg = SUNXI_CLK_MAX_REG;
int cpus_clk_maxreg = CPUS_CLK_MAX_REG;

/*                                ns  nw  ks  kw  ms  mw  ps  pw  d1s d1w d2s d2w {frac   out mode}   en-s    sdmss   sdmsw   sdmpat          sdmval*/
SUNXI_CLK_FACTORS(pll_cpu,        8,  8,  0,  0,  0,  2,  16, 2,  0,  0,  0,  0,    0,    0,  0,      31,     0,      0,      0,              0);
SUNXI_CLK_FACTORS(pll_ddr0,       8,  8,  0,  0,  0,  0,  0,  0,  1,  1,  0,  1,    0,    0,  0,      31,     24,     0,      PLL_DDR0PAT,    0xd1303333);
SUNXI_CLK_FACTORS(pll_ddr1,       8,  8,  0,  0,  0,  0,  0,  0,  1,  1,  0,  1,    0,    0,  0,      31,     0,      0,      PLL_DDR1PAT,    0xd1303333);
SUNXI_CLK_FACTORS(pll_periph0,    8,  8,  0,  0,  0,  0,  0,  0,  1,  1,  0,  1,    0,    0,  0,      31,     0,      0,      0,              0);
SUNXI_CLK_FACTORS(pll_periph1,    8,  8,  0,  0,  0,  0,  0,  0,  1,  1,  0,  1,    0,    0,  0,      31,     24,     0,      PLL_PERI1PAT0,  0xd1303333);
SUNXI_CLK_FACTORS(pll_gpu,        8,  8,  0,  0,  0,  0,  0,  0,  1,  1,  0,  1,    0,    0,  0,      31,     24,     0,      PLL_GPUPAT0,    0xd1303333);
SUNXI_CLK_FACTORS(pll_video0,     8,  8,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,    0,    0,  0,      31,     24,     0,      PLL_VIDEO0PAT0, 0xd1303333);
SUNXI_CLK_FACTORS(pll_video1,     8,  8,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,    0,    0,  0,      31,     24,     0,      PLL_VIDEO1PAT0, 0xd1303333);
SUNXI_CLK_FACTORS(pll_ve,         8,  8,  0,  0,  0,  0,  0,  0,  1,  1,  0,  1,    0,    0,  0,      31,     24,     0,      PLL_VEPAT0,     0xd1303333);
SUNXI_CLK_FACTORS(pll_de,         8,  8,  0,  0,  0,  0,  0,  0,  1,  1,  0,  1,    0,    0,  0,      31,     24,     0,      PLL_DEPAT0,     0xd1303333);
SUNXI_CLK_FACTORS(pll_audio,      8,  8,  0,  0,  0,  0,  16, 6,  1,  1,  0,  1,    0,    0,  0,      31,     24,     0,      PLL_AUDIOPAT0,  0xd1303333);

static int get_factors_pll_cpu(u32 rate, u32 parent_rate,
		struct clk_factors_value *factor)
{

	int index;
	u64 tmp_rate;

	if (!factor)
		return -1;
	tmp_rate = rate > pllcpu_max ? pllcpu_max : rate;
	do_div(tmp_rate, 1000000);
	index = tmp_rate;

	if (FACTOR_SEARCH(cpu))
		return -1;

	return 0;
}

static int get_factors_pll_ddr0(u32 rate, u32 parent_rate,
		struct clk_factors_value *factor)
{
	int index;
	u64 tmp_rate;

	if (!factor)
		return -1;

	tmp_rate = rate > pllddr0_max ? pllddr0_max : rate;
	do_div(tmp_rate, 1000000);
	index = tmp_rate;

	if (FACTOR_SEARCH(ddr0))
		return -1;

	return 0;
}

static int get_factors_pll_ddr1(u32 rate, u32 parent_rate,
		struct clk_factors_value *factor)
{
	int index;
	u64 tmp_rate;

	if (!factor)
		return -1;

	tmp_rate = rate > pllddr1_max ? pllddr1_max : rate;
	do_div(tmp_rate, 1000000);
	index = tmp_rate;

	if (FACTOR_SEARCH(ddr1))
		return -1;

	return 0;
}

static int get_factors_pll_periph0(u32 rate, u32 parent_rate,
		struct clk_factors_value *factor)
{
	int index;
	u64 tmp_rate;

	if (!factor)
		return -1;

	tmp_rate = rate > pllperiph0_max ? pllperiph0_max : rate;
	do_div(tmp_rate, 1000000);
	index = tmp_rate;

	if (FACTOR_SEARCH(periph0))
		return -1;

	return 0;
}

static int get_factors_pll_periph1(u32 rate, u32 parent_rate,
		struct clk_factors_value *factor)
{
	int index;
	u64 tmp_rate;

	if (!factor)
		return -1;

	tmp_rate = rate > pllperiph1_max ? pllperiph1_max : rate;
	do_div(tmp_rate, 1000000);
	index = tmp_rate;

	if (FACTOR_SEARCH(periph1))
		return -1;

	return 0;
}

static int get_factors_pll_gpu(u32 rate, u32 parent_rate,
		struct clk_factors_value *factor)
{
	u64 tmp_rate;
	int index;

	if (!factor)
		return -1;

	tmp_rate = rate > pllgpu_max ? pllgpu_max : rate;
	do_div(tmp_rate, 1000000);
	index = tmp_rate;

	if (FACTOR_SEARCH(gpu))
		return -1;

	return 0;
}

static int get_factors_pll_video0(u32 rate, u32 parent_rate,
		struct clk_factors_value *factor)
{
	u64 tmp_rate;
	int index;

	if (!factor)
		return -1;

	tmp_rate = rate > pllvideo0_max ? pllvideo0_max : rate;
	do_div(tmp_rate, 1000000);
	index = tmp_rate;

	if (FACTOR_SEARCH(video0))
		return -1;

	return 0;
}

static int get_factors_pll_video1(u32 rate, u32 parent_rate,
		struct clk_factors_value *factor)
{
	u64 tmp_rate;
	int index;

	if (!factor)
		return -1;

	tmp_rate = rate > pllvideo1_max ? pllvideo1_max : rate;
	do_div(tmp_rate, 1000000);
	index = tmp_rate;

	if (FACTOR_SEARCH(video1))
		return -1;

	return 0;
}

static int get_factors_pll_ve(u32 rate, u32 parent_rate,
		struct clk_factors_value *factor)
{
	u64 tmp_rate;
	int index;

	if (!factor)
		return -1;

	tmp_rate = rate > pllve_max ? pllve_max : rate;
	do_div(tmp_rate, 1000000);
	index = tmp_rate;

	if (FACTOR_SEARCH(ve))
		return -1;

	return 0;
}

static int get_factors_pll_de(u32 rate, u32 parent_rate,
		struct clk_factors_value *factor)
{
	u64 tmp_rate;
	int index;

	if (!factor)
		return -1;

	tmp_rate = rate > pllde_max ? pllde_max : rate;
	do_div(tmp_rate, 1000000);
	index = tmp_rate;

	if (FACTOR_SEARCH(de))
		return -1;

	return 0;
}

static int get_factors_pll_audio(u32 rate, u32 parent_rate,
		struct clk_factors_value *factor)
{
	u64 tmp_rate;
	int index;

	if (!factor)
		return -1;

	tmp_rate = rate > pllaudio_max ? pllaudio_max : rate;
	do_div(tmp_rate, 1000000);
	index = tmp_rate;

	if (FACTOR_SEARCH(audio))
		return -1;

	return 0;
}

/*    pll_cpux: 24*N/M/P (P=2^factorp)  */
static unsigned long calc_rate_pll_cpu(u32 parent_rate,
		struct clk_factors_value *factor)
{
	u64 tmp_rate = (parent_rate ? parent_rate : 24000000);
	tmp_rate = tmp_rate * (factor->factorn + 1);
	do_div(tmp_rate, (factor->factorm + 1) * (1 << factor->factorp));
	return (unsigned long)tmp_rate;
}

/*    pll_ddr: 24*N/D1/D2    */
static unsigned long calc_rate_pll_ddr(u32 parent_rate,
		struct clk_factors_value *factor)
{
	u64 tmp_rate = (parent_rate ? parent_rate : 24000000);
	tmp_rate = tmp_rate * (factor->factorn + 1);
	do_div(tmp_rate, (factor->factord1 + 1) * (factor->factord2 + 1));
	return (unsigned long)tmp_rate;
}

/*    pll_periph0/pll_periph1: 24*N/D1/D2/2    */
static unsigned long calc_rate_pll_periph(u32 parent_rate,
		struct clk_factors_value *factor)
{
	u64 tmp_rate = (parent_rate ? parent_rate : 24000000);
	tmp_rate = tmp_rate * (factor->factorn + 1);
	do_div(tmp_rate, 2 * (factor->factord1 + 1) * (factor->factord2 + 1));
	return (unsigned long)tmp_rate;
}

/*    pll_video0/pll_video1: 24*N/D1/4    */
static unsigned long calc_rate_video(u32 parent_rate,
		struct clk_factors_value *factor)
{
	u64 tmp_rate = (parent_rate ? parent_rate : 24000000);
	tmp_rate = tmp_rate * (factor->factorn + 1);
	do_div(tmp_rate, 4 * (factor->factord1 + 1));
	return (unsigned long)tmp_rate;
}

/*    pll_media: 24*N/D1/D2    */
static unsigned long calc_rate_media(u32 parent_rate,
		struct clk_factors_value *factor)
{
	u64 tmp_rate = (parent_rate ? parent_rate : 24000000);
	tmp_rate = tmp_rate * (factor->factorn + 1);
	do_div(tmp_rate, (factor->factord1 + 1) * (factor->factord2 + 1));
	return (unsigned long)tmp_rate;
}

/*
 *    pll_audio: 24*N/D1/D2/P
 *
 *    NOTE: pll_audiox4 = 24*N/D1/2
 *          pll_audiox2 = 24*N/D1/4
 *
 *    pll_audiox4=2*pll_audiox2=4*pll_audio only when D2*P=8
 */
static unsigned long calc_rate_audio(u32 parent_rate,
		struct clk_factors_value *factor)
{
	u64 tmp_rate = (parent_rate ? parent_rate : 24000000);
	tmp_rate = tmp_rate * (factor->factorn + 1);
	do_div(tmp_rate, (factor->factord1 + 1) * (factor->factord2 + 1) * (factor->factorp + 1));
	return (unsigned long)tmp_rate;
}

static const char *hosc_parents[] = {"hosc"};
struct factor_init_data sunxi_factos[] = {
	/* name         parent        parent_num, flags                                      reg          lock_reg     lock_bit     pll_lock_ctrl_reg lock_en_bit lock_mode           config                         get_factors               calc_rate              priv_ops*/
	{"pll_cpu",     hosc_parents, 1,          CLK_GET_RATE_NOCACHE, PLL_CPU,     PLL_CPU,     LOCKBIT(28), PLL_CPU,     29,          PLL_LOCK_NONE_MODE, &sunxi_clk_factor_pll_cpu,     &get_factors_pll_cpu,     &calc_rate_pll_cpu,    (struct clk_ops *)NULL},
	{"pll_ddr0",    hosc_parents, 1,          CLK_GET_RATE_NOCACHE, PLL_DDR0,    PLL_DDR0,    LOCKBIT(28), PLL_DDR0,    29,          PLL_LOCK_NONE_MODE, &sunxi_clk_factor_pll_ddr0,    &get_factors_pll_ddr0,    &calc_rate_pll_ddr,    (struct clk_ops *)NULL},
	{"pll_ddr1",    hosc_parents, 1,          CLK_GET_RATE_NOCACHE, PLL_DDR1,    PLL_DDR1,    LOCKBIT(28), PLL_DDR1,    29,          PLL_LOCK_NONE_MODE, &sunxi_clk_factor_pll_ddr1,    &get_factors_pll_ddr1,    &calc_rate_pll_ddr,    (struct clk_ops *)NULL},
	{"pll_periph0", hosc_parents, 1,          0,                    PLL_PERIPH0, PLL_PERIPH0, LOCKBIT(28), PLL_PERIPH0, 29,          PLL_LOCK_NONE_MODE, &sunxi_clk_factor_pll_periph0, &get_factors_pll_periph0, &calc_rate_pll_periph, (struct clk_ops *)NULL},
	{"pll_periph1", hosc_parents, 1,          0,                    PLL_PERIPH1, PLL_PERIPH1, LOCKBIT(28), PLL_PERIPH1, 29,          PLL_LOCK_NONE_MODE, &sunxi_clk_factor_pll_periph1, &get_factors_pll_periph1, &calc_rate_pll_periph, (struct clk_ops *)NULL},
	{"pll_gpu",     hosc_parents, 1,          0,                    PLL_GPU,     PLL_GPU,     LOCKBIT(28), PLL_GPU,     29,          PLL_LOCK_NONE_MODE, &sunxi_clk_factor_pll_gpu,     &get_factors_pll_gpu,     &calc_rate_media,      (struct clk_ops *)NULL},
	{"pll_video0",  hosc_parents, 1,          0,                    PLL_VIDEO0,  PLL_VIDEO0,  LOCKBIT(28), PLL_VIDEO0,  29,          PLL_LOCK_NONE_MODE, &sunxi_clk_factor_pll_video0,  &get_factors_pll_video0,  &calc_rate_video,      (struct clk_ops *)NULL},
	{"pll_video1",  hosc_parents, 1,          0,                    PLL_VIDEO1,  PLL_VIDEO1,  LOCKBIT(28), PLL_VIDEO1,  29,          PLL_LOCK_NONE_MODE, &sunxi_clk_factor_pll_video1,  &get_factors_pll_video1,  &calc_rate_video,      (struct clk_ops *)NULL},
	{"pll_ve",      hosc_parents, 1,          0,                    PLL_VE,      PLL_VE,      LOCKBIT(28), PLL_VE,      29,          PLL_LOCK_NONE_MODE, &sunxi_clk_factor_pll_ve,      &get_factors_pll_ve,      &calc_rate_media,      (struct clk_ops *)NULL},
	{"pll_de",      hosc_parents, 1,          0,                    PLL_DE,      PLL_DE,      LOCKBIT(28), PLL_DE,      29,          PLL_LOCK_NONE_MODE, &sunxi_clk_factor_pll_de,      &get_factors_pll_de,      &calc_rate_media,      (struct clk_ops *)NULL},
	{"pll_audio",   hosc_parents, 1,          0,                    PLL_AUDIO,   PLL_AUDIO,   LOCKBIT(28), PLL_AUDIO,   29,          PLL_LOCK_NONE_MODE, &sunxi_clk_factor_pll_audio,   &get_factors_pll_audio,   &calc_rate_audio,      (struct clk_ops *)NULL},
};

static const char *cpu_parents[] = {"hosc", "losc", "iosc", "pll_cpu"};
static const char *axi_parents[] = {"cpu"};
static const char *cpuapb_parents[] = {"cpu"};
static const char *psi_parents[] = {"hosc", "losc", "iosc", "pll_periph0"};
static const char *ahb1_parents[] = {"psi"};
static const char *ahb2_parents[] = {"psi"};
static const char *ahb3_parents[] = {"hosc", "losc", "psi", "pll_periph0"};
static const char *apb1_parents[] = {"hosc", "losc", "psi", "pll_periph0"};
static const char *apb2_parents[] = {"hosc", "losc", "psi", "pll_periph0"};
static const char *mbus_parents[] = {"hosc", "pll_periph0x2", "pll_ddr0", "pll_ddr1"};
static const char *de_parents[] = {"pll_de", "pll_periph0x2"};
static const char *gpu_parents[] = {"pll_gpu", ""};
static const char *ce_parents[] = {"hosc", "pll_periph0x2"};
static const char *ve_parents[] = {"pll_ve", ""};
static const char *emce_parents[] = {"hosc", "pll_periph0x2"};
static const char *vp9_parents[] = {"pll_ve", "pll_periph0x2"};
static const char *ahb1mod_parents[] = {"ahb1"};
static const char *ahb3mod_parents[] = {"ahb3"};
static const char *apb1mod_parents[] = {"apb1"};
static const char *apb2mod_parents[] = {"apb2"};
static const char *sdram_parents[] = {"pll_ddr0", "pll_ddr1", "", ""};
static const char *nand_parents[] = {"hosc", "pll_periph0", "pll_periph1", "pll_periph0x2", "pll_periph1x2", "", "", ""};
static const char *smhc_parents[] = {"hosc", "pll_periph0x2", "pll_periph1x2", ""};
static const char *spi_parents[] = {"hosc", "pll_periph0", "pll_periph1", "pll_periph0x2", "pll_periph1x2", "", "", ""};
static const char *audio_parents[] = {"pll_audio", "pll_audiox2", "pll_audiox4", ""};
static const char *usbohci_12m_parents[] = {"osc48md4", "hoscd2", "losc", ""};
static const char *mipi_dphy_parents[] = {"pll_video0", "pll_video0x4", "pll_video1", ""};
static const char *mipi_host_parents[] = {"pll_periph0", "pll_periph0x2", "hosc", ""};
static const char *tcon_lcd_parents[] = {"pll_video0", "pll_video0x4", "pll_video1", "", "", "", "", ""};
static const char *csi_top_parents[] = {"pll_video0", "", "pll_ve", "pll_periph0", "", "", "", ""};
static const char *csi_master_parents[] = {"hosc", "pll_video0", "pll_periph0", "pll_periph0", "", "", "", ""};
static const char *cpurcpus_pll_parents[] = {"pll_periph0"};
static const char *cpurcpus_parents[] = {"hosc", "losc", "iosc", "cpurcpus_pll"};
static const char *cpurahbs_parents[] = {"cpurcpus"};
static const char *cpurapbs1_parents[] = {"cpurahbs"};
static const char *cpurapbs2_pll_parents[] = {"pll_periph0"};
static const char *cpurapbs2_parents[] = {"hosc", "losc", "iosc", "cpurapbs2_pll"};
static const char *cpurpio_parents[] = {"cpurapbs1"};
static const char *losc_parents[] = {"losc"};

struct sunxi_clk_comgate com_gates[] = {
{"nand",    0, 0x3, BUS_GATE_SHARE|RST_GATE_SHARE|MBUS_GATE_SHARE, 0},
{"codec",   0, 0x3, BUS_GATE_SHARE|RST_GATE_SHARE, 0},
{"mipi0",   0, 0x3, BUS_GATE_SHARE|RST_GATE_SHARE, 0},
{"mipi1",   0, 0x3, BUS_GATE_SHARE|RST_GATE_SHARE, 0},
{"csi",     0, 0x7, BUS_GATE_SHARE|RST_GATE_SHARE|MBUS_GATE_SHARE, 0},
};

/*
SUNXI_CLK_PERIPH(name,           mux_reg,         mux_sft, mux_wid,      div_reg,            div_msft,  div_mwid,   div_nsft,   div_nwid,   gate_flag,  en_reg,          rst_reg,         bus_gate_reg,  drm_gate_reg,  en_sft,     rst_sft,    bus_gate_sft,   dram_gate_sft, lock,      com_gate,         com_gate_off)
*/
SUNXI_CLK_PERIPH(cpu,            CPU_CFG,         24,      2,            0,                  0,         0,          0,          0,          0,          0,               0,               0,             0,             0,          0,          0,              0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(axi,            0,               0,       0,            CPU_CFG,            0,         2,          0,          0,          0,          0,               0,               0,             0,             0,          0,          0,              0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(cpuapb,         0,               0,       0,            CPU_CFG,            8,         2,          0,          0,          0,          0,               0,               0,             0,             0,          0,          0,              0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(psi,            PSI_CFG,         24,      2,            PSI_CFG,            0,         2,          8,          2,          0,          0,               PSI_GATE,        PSI_GATE,      0,             0,          16,         0,              0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(ahb1,           0,               0,       0,            0,                  0,         0,          0,          0,          0,          0,               0,               0,             0,             0,          0,          0,              0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(ahb2,           0,               0,       0,            0,                  0,         0,          0,          0,          0,          0,               0,               0,             0,             0,          0,          0,              0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(ahb3,           AHB3_CFG,        24,      2,            AHB3_CFG,           0,         2,          8,          2,          0,          0,               0,               0,             0,             0,          0,          0,              0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(apb1,           APB1_CFG,        24,      2,            APB1_CFG,           0,         2,          8,          2,          0,          0,               0,               0,             0,             0,          0,          0,              0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(apb2,           APB2_CFG,        24,      2,            APB2_CFG,           0,         2,          8,          2,          0,          0,               0,               0,             0,             0,          0,          0,              0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(mbus,           MBUS_CFG,        24,      2,            MBUS_CFG,           0,         3,          0,          0,          0,          MBUS_CFG,        MBUS_CFG,        0,             0,             31,         30,         0,              0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(de,             DE_CFG,          24,      1,            DE_CFG,             0,         4,          0,          0,          0,          DE_CFG,          DE_GATE,         DE_GATE,       0,             31,         16,         0,              0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(gpu,            GPU_CFG,         24,      1,            GPU_CFG,            0,         3,          0,          0,          0,          GPU_CFG,         GPU_GATE,        GPU_GATE,      0,             31,         16,         0,              0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(ce,             CE_CFG,          24,      1,            CE_CFG,             0,         4,          8,          2,          0,          CE_CFG,          CE_GATE,         CE_GATE,       MBUS_GATE,     31,         16,         0,              2,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(ve,             VE_CFG,          24,      1,            VE_CFG,             0,         3,          0,          0,          0,          VE_CFG,          VE_GATE,         VE_GATE,       MBUS_GATE,     31,         16,         0,              1,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(emce,           EMCE_CFG,        24,      1,            EMCE_CFG,           0,         4,          8,          2,          0,          EMCE_CFG,        EMCE_GATE,       EMCE_GATE,     0,             31,         16,         0,              0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(vp9,            VP9_CFG,         24,      1,            VP9_CFG,            0,         3,          0,          0,          0,          VP9_CFG,         VP9_GATE,        VP9_GATE,      0,             31,         16,         0,              0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(dma,            0,                0,      0,            0,                  0,         0,          0,          0,          0,          0,               DMA_GATE,        DMA_GATE,      MBUS_GATE,     0,          16,         0,              0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(msgbox,         0,               0,       0,            0,                  0,         0,          0,          0,          0,          0,               MSGBOX_GATE,     MSGBOX_GATE,   0,             0,          16,         0,              0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(hwspinlock_rst, 0,               0,       0,            0,                  0,         0,          0,          0,          0,          0,               SPINLOCK_GATE,   0,             0,             0,          16,         0,              0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(hwspinlock_bus, 0,               0,       0,            0,                  0,         0,          0,          0,          0,          0,               0,               SPINLOCK_GATE, 0,             0,          0,          0,              0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(hstimer,        0,               0,       0,            0,                  0,         0,          0,          0,          0,          0,               HSTIMER_GATE,    HSTIMER_GATE,     0,          0,          16,         0,              0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(avs,            0,               0,       0,            0,                  0,         0,          0,          0,          0,          AVS_CFG,         0,               0,                0,          31,         0,          0,              0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(dbgsys,         0,               0,       0,            0,                  0,         0,          0,          0,          0,          0,               DBGSYS_GATE,     DBGSYS_GATE,      0,          0,          16,         0,              0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(pwm,            0,               0,       0,            0,                  0,         0,          0,          0,          0,          0,               PWM_GATE,        PWM_GATE,         0,          0,          16,         0,              0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(iommu,          0,               0,       0,            0,                  0,         0,          0,          0,          0,          0,               0,               IOMMU_GATE,       0,          0,          0,          0,              0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(sdram,          DRAM_CFG,        24,      2,            DRAM_CFG,           0,         2,          0,          0,          0,          0,               DRAM_GATE,       DRAM_GATE,        0,          0,          16,         0,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(nand0,          NAND0_CFG,       24,      3,            NAND0_CFG,          0,         4,          8,          2,          0,          NAND0_CFG,       NAND_GATE,      NAND_GATE,         MBUS_GATE,  31,         16,         0,             5,             &clk_lock, &com_gates[0],    0);
SUNXI_CLK_PERIPH(nand1,          NAND1_CFG,       24,      3,            NAND1_CFG,          0,         4,          8,          2,          0,          NAND1_CFG,       NAND_GATE,      NAND_GATE,         MBUS_GATE,  31,         16,         0,             5,             &clk_lock, &com_gates[0],    1);
SUNXI_CLK_PERIPH(sdmmc0_mod,     SMHC0_CFG,       24,      2,            SMHC0_CFG,          0,         4,          8,          2,          0,          SMHC0_CFG,       0,              0,                 0,          31,         0,          0,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(sdmmc0_rst,     0,               0,       0,            0,                  0,         0,          0,          0,          0,          0,               SMHC_GATE,      0,                 0,          0,          16,         0,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(sdmmc0_bus,     0,               0,       0,            0,                  0,         0,          0,          0,          0,          0,               0,              SMHC_GATE,         0,          0,          0,         0,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(sdmmc1_mod,     SMHC1_CFG,       24,      2,            SMHC0_CFG,          0,         4,          8,          2,          0,          SMHC1_CFG,       0,              0,                 0,          31,         0,          0,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(sdmmc1_rst,     0,               0,       0,            0,                  0,         0,          0,          0,          0,          0,               SMHC_GATE,      0,                 0,          0,          17,         0,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(sdmmc1_bus,     0,               0,       0,            0,                  0,         0,          0,          0,          0,          0,               0,              SMHC_GATE,         0,          0,          0,          1,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(sdmmc2_mod,     SMHC2_CFG,       24,      2,            SMHC2_CFG,          0,         4,          8,          2,          0,          SMHC2_CFG,       0,              0,                 0,          31,         0,          0,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(sdmmc2_rst,     0,               0,       0,            0,                  0,         0,          0,          0,          0,          0,               SMHC_GATE,      0,                 0,          0,          18,         0,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(sdmmc2_bus,     0,               0,       0,            0,                  0,         0,          0,          0,          0,          0,               0,              SMHC_GATE,         0,          0,          0,          2,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(uart0,          0,               0,       0,            0,                  0,         0,          0,          0,          0,          0,               UART_GATE,      UART_GATE,          0,         0,          16,         0,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(uart1,          0,               0,       0,            0,                  0,         0,          0,          0,          0,          0,               UART_GATE,      UART_GATE,          0,         0,          17,         1,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(uart2,          0,               0,       0,            0,                  0,         0,          0,          0,          0,          0,               UART_GATE,      UART_GATE,          0,         0,          18,         2,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(uart3,          0,               0,       0,            0,                  0,         0,          0,          0,          0,          0,               UART_GATE,      UART_GATE,          0,         0,          19,         3,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(twi0,           0,               0,       0,            0,                  0,         0,          0,          0,          0,          0,               TWI_GATE,       TWI_GATE,           0,         0,          16,         0,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(twi1,           0,               0,       0,            0,                  0,         0,          0,          0,          0,          0,               TWI_GATE,       TWI_GATE,           0,         0,          17,         1,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(twi2,           0,               0,       0,            0,                  0,         0,          0,          0,          0,          0,               TWI_GATE,       TWI_GATE,           0,         0,          18,         2,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(twi3,           0,               0,       0,            0,                  0,         0,          0,          0,          0,          0,               TWI_GATE,       TWI_GATE,           0,         0,          19,         3,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(twi4,           0,               0,       0,            0,                  0,         0,          0,          0,          0,          0,               TWI_GATE,       TWI_GATE,           0,         0,          20,         4,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(spi0,           SPI0_CFG,        24,      3,            SPI0_CFG,           0,         4,          8,          2,          0,          SPI0_CFG,        SPI_GATE,       SPI_GATE,          0,          31,         16,         0,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(spi1,           SPI1_CFG,        24,      3,            SPI1_CFG,           0,         4,          8,          2,          0,          SPI1_CFG,        SPI_GATE,       SPI_GATE,          0,          31,         17,         1,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(gpadc,          0,               0,       0,            0,                  0,         0,          0,          0,          0,          0,               GPADC_GATE,     GPADC_GATE,        0,          0,          16,         0,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(ths,            0,               0,       0,            0,                  0,         0,          0,          0,          0,          0,               THS_GATE,        THS_GATE,         0,          0,          16,         0,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(i2s0,           I2S0_CFG,        24,      2,            I2S0_CFG,           0,         0,          8,          2,          0,          I2S0_CFG,        I2S_GATE,        I2S_GATE,         0,          31,         16,         0,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(i2s1,           I2S1_CFG,        24,      2,            I2S1_CFG,           0,         0,          8,          2,          0,          I2S1_CFG,        I2S_GATE,        I2S_GATE,         0,          31,         17,         1,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(i2s2,           I2S2_CFG,        24,      2,            I2S2_CFG,           0,         0,          8,          2,          0,          I2S2_CFG,        I2S_GATE,        I2S_GATE,         0,          31,         18,         2,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(dmic,           DMIC_CFG,        24,      2,            DMIC_CFG,           0,         0,          8,          2,          0,          DMIC_CFG,        DMIC_GATE,       DMIC_GATE,        0,          31,         16,         0,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(codec_1x,       CODEC_1X_CFG,    24,      2,            CODEC_1X_CFG,       0,         4,          0,          0,          0,          CODEC_1X_CFG,    CODEC_GATE,      CODEC_GATE,       0,          31,         16,         0,             0,             &clk_lock, &com_gates[1],    0);
SUNXI_CLK_PERIPH(codec_4x,       CODEC_4X_CFG,    24,      2,            CODEC_4X_CFG,       0,         4,          0,          0,          0,          CODEC_4X_CFG,    CODEC_GATE,      CODEC_GATE,       0,          31,         16,         0,             0,             &clk_lock, &com_gates[1],    1);
SUNXI_CLK_PERIPH(usbphy0,        0,               0,       0,            0,                  0,         0,          0,          0,          0,          USB0_CFG,        USB0_CFG,        0,                0,          29,         30,         0,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(usbphy1,        0,               0,       0,            0,                  0,         0,          0,          0,          0,          USB1_CFG,        USB1_CFG,        0,                0,          29,         30,         0,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(usbohci0,       0,               0,       0,            0,                  0,         0,          0,          0,          0,          USB0_CFG,        USB_GATE,        USB_GATE,         0,          31,         16,         0,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(usbohci0_12m,   USB0_CFG,        24,      2,            0,                  0,         0,          0,          0,          0,          0,               0,               0,                0,          0,          0,          0,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(usbohci1,       0,               0,       0,            0,                  0,         0,          0,          0,          0,          USB1_CFG,        USB_GATE,        USB_GATE,         0,          31,         17,         1,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(usbohci1_12m,   USB1_CFG,        24,      2,            0,                  0,         0,          0,          0,          0,          0,               0,               0,                0,          0,          0,          0,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(usbehci0,       0,               0,       0,            0,                  0,         0,          0,          0,          0,          0,               USB_GATE,        USB_GATE,         0,          0,          20,         4,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(usbehci1,       0,               0,       0,            0,                  0,         0,          0,          0,          0,          0,               USB_GATE,        USB_GATE,         0,          0,          21,         5,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(usbotg,         0,               0,       0,            0,                  0,         0,          0,          0,          0,          0,               USB_GATE,        USB_GATE,         0,          0,          24,         8,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(mipi_dphy0,     MIPI_DPHY0_CFG,  24,      2,            MIPI_DPHY0_CFG,     0,         4,          8,          2,          0,          MIPI_DPHY0_CFG,  MIPI_GATE,       MIPI_GATE,         0,         31,         16,         0,            17,             &clk_lock, &com_gates[2],    0);
SUNXI_CLK_PERIPH(mipi_host0,     MIPI_HOST0_CFG,  24,      2,            MIPI_HOST0_CFG,     0,         4,          0,          0,          0,          MIPI_HOST0_CFG,  MIPI_GATE,       MIPI_GATE,        0,          31,         16,         0,            17,             &clk_lock, &com_gates[2],    0);
SUNXI_CLK_PERIPH(mipi_dphy1,     MIPI_DPHY1_CFG,  24,      2,            MIPI_DPHY1_CFG,     0,         4,          8,          2,          0,          MIPI_DPHY1_CFG,  MIPI_GATE,       MIPI_GATE,        0,          31,         17,         1,            17,             &clk_lock, &com_gates[3],    1);
SUNXI_CLK_PERIPH(mipi_host1,     MIPI_HOST1_CFG,  24,      2,            MIPI_HOST1_CFG,     0,         4,          0,          0,          0,          MIPI_HOST1_CFG,  MIPI_GATE,       MIPI_GATE,        0,          31,         17,         1,            17,             &clk_lock, &com_gates[3],    1);
SUNXI_CLK_PERIPH(display_top,    0,               0,       0,            0,                  0,         0,          0,          0,          0,          0,               DISPLAY_TOP_GATE, DISPLAY_TOP_GATE, 0,         0,          16,         0,            0,              &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(tcon_lcd0,      TCON_LCD0_CFG,   24,      3,            0,                  0,         0,          0,          0,          0,          TCON_LCD0_CFG,   TCON_LCD_GATE,   TCON_LCD_GATE, 0,             31,         16,         0,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(tcon_lcd1,      TCON_LCD1_CFG,   24,      3,            0,                  0,         0,          0,          0,          0,          TCON_LCD1_CFG,   TCON_LCD_GATE,   TCON_LCD_GATE, 0,             31,         17,         1,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(edp,            EDP_CFG,         0,       0,            0,                  0,         0,          0,          0,          0,          EDP_CFG,         EDP_GATE,        EDP_GATE,      0,             31,         16,         0,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(csi_misc,       0,               0,       0,            0,                  0,         0,          0,          0,          0,          CSI_MISC_CFG,    CSI_GATE,        CSI_GATE,      MBUS_GATE,     0,          16,         0,             8,             &clk_lock, &com_gates[4],    0);
SUNXI_CLK_PERIPH(csi_top,        CSI_TOP_CFG,     24,      3,            CSI_TOP_CFG,        0,         4,          0,          0,          0,          CSI_TOP_CFG,     CSI_GATE,        CSI_GATE,      MBUS_GATE,     31,         16,         0,             8,             &clk_lock, &com_gates[4],    1);
SUNXI_CLK_PERIPH(csi_master,     CSI_MASTER_CFG,  24,      3,            CSI_MASTER_CFG,     0,         5,          0,          0,          0,          CSI_MASTER_CFG,  CSI_GATE,        CSI_GATE,      MBUS_GATE,     31,         16,         0,             8,             &clk_lock, &com_gates[4],    2);
SUNXI_CLK_PERIPH(pio,            0,               0,       0,            0,                  0,         0,          0,          0,          0,          0,               0,               0,             0,             0,          0,          0,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(cpurcpus_pll,   0,               0,       0,            CPUS_CFG,           0,         5,          0,          0,          0,          0,               0,               0,             0,             0,          0,          0,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(cpurcpus,       CPUS_CFG,        24,      2,            CPUS_CFG,           0,         0,          8,          2,          0,          0,               0,               0,             0,             0,          0,          0,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(cpurahbs,       0,               0,       0,            0,                  0,         0,          0,          0,          0,          0,               0,               0,             0,             0,          0,          0,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(cpurapbs1,      0,               0,       0,            CPUS_APBS1_CFG,     0,         2,          0,          0,          0,          0,               0,               0,             0,             0,          0,          0,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(cpurapbs2_pll,  0,               0,       0,            CPUS_APBS2_CFG,     0,         5,          0,          0,          0,          0,               0,               0,             0,             0,          0,          0,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(cpurapbs2,      CPUS_APBS2_CFG,  24,      2,            CPUS_APBS2_CFG,     0,         0,          8,          2,          0,          0,               0,               0,             0,             0,          0,          0,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(losc_out,       0,               0,       0,            0,                  0,         0,          0,          0,          0,          0,               0,               LOSC_OUT_GATE, 0,             0,          0,          0,             0,             &clk_lock, NULL,             0);
SUNXI_CLK_PERIPH(cpurpio,        0,               0,       0,            0,                  0,         0,          0,          0,          0,          0,               0,               0,             0,             0,          0,          0,             0,             &clk_lock, NULL,             0);

struct periph_init_data sunxi_periphs_init[] = {
	{"cpu",            CLK_GET_RATE_NOCACHE, cpu_parents,            ARRAY_SIZE(cpu_parents),            &sunxi_clk_periph_cpu              },
	{"axi",            0,                    axi_parents,            ARRAY_SIZE(axi_parents),            &sunxi_clk_periph_axi              },
	{"cpuapb",         0,                    cpuapb_parents,         ARRAY_SIZE(cpuapb_parents),         &sunxi_clk_periph_cpuapb           },
	{"psi",            0,                    psi_parents,            ARRAY_SIZE(psi_parents),            &sunxi_clk_periph_psi              },
	{"ahb1",           0,                    ahb1_parents,           ARRAY_SIZE(ahb1_parents),           &sunxi_clk_periph_ahb1             },
	{"ahb2",           0,                    ahb2_parents,           ARRAY_SIZE(ahb2_parents),           &sunxi_clk_periph_ahb2             },
	{"ahb3",           0,                    ahb3_parents,           ARRAY_SIZE(ahb3_parents),           &sunxi_clk_periph_ahb3             },
	{"apb1",           0,                    apb1_parents,           ARRAY_SIZE(apb1_parents),           &sunxi_clk_periph_apb1             },
	{"apb2",           0,                    apb2_parents,           ARRAY_SIZE(apb2_parents),           &sunxi_clk_periph_apb2             },
	{"mbus",           0,                    mbus_parents,           ARRAY_SIZE(mbus_parents),           &sunxi_clk_periph_mbus             },
	{"de",             0,                    de_parents,             ARRAY_SIZE(de_parents),             &sunxi_clk_periph_de               },
	{"gpu",            0,                    gpu_parents,            ARRAY_SIZE(gpu_parents),            &sunxi_clk_periph_gpu              },
	{"ce",             0,                    ce_parents,             ARRAY_SIZE(ce_parents),             &sunxi_clk_periph_ce               },
	{"ve",             0,                    ve_parents,             ARRAY_SIZE(ve_parents),             &sunxi_clk_periph_ve               },
	{"emce",           0,                    emce_parents,           ARRAY_SIZE(emce_parents),           &sunxi_clk_periph_emce             },
	{"vp9",            0,                    vp9_parents,            ARRAY_SIZE(vp9_parents),            &sunxi_clk_periph_vp9              },
	{"dma",            0,                    ahb1mod_parents,        ARRAY_SIZE(ahb1mod_parents),        &sunxi_clk_periph_dma              },
	{"msgbox",         0,                    ahb1mod_parents,        ARRAY_SIZE(ahb1mod_parents),        &sunxi_clk_periph_msgbox           },
	{"hwspinlock_rst", 0,                    ahb1mod_parents,        ARRAY_SIZE(ahb1mod_parents),        &sunxi_clk_periph_hwspinlock_rst   },
	{"hwspinlock_bus", 0,                    ahb1mod_parents,        ARRAY_SIZE(ahb1mod_parents),        &sunxi_clk_periph_hwspinlock_bus   },
	{"hstimer",        0,                    ahb1mod_parents,        ARRAY_SIZE(ahb1mod_parents),        &sunxi_clk_periph_hstimer          },
	{"avs",            0,                    hosc_parents,           ARRAY_SIZE(hosc_parents),           &sunxi_clk_periph_avs              },
	{"dbgsys",         0,                    ahb1mod_parents,        ARRAY_SIZE(ahb1mod_parents),        &sunxi_clk_periph_dbgsys           },
	{"pwm",            0,                    apb1mod_parents,        ARRAY_SIZE(apb1mod_parents),        &sunxi_clk_periph_pwm              },
	{"iommu",          0,                    ahb1mod_parents,        ARRAY_SIZE(ahb1mod_parents),        &sunxi_clk_periph_iommu            },
	{"sdram",          0,                    sdram_parents,          ARRAY_SIZE(sdram_parents),          &sunxi_clk_periph_sdram            },
	{"nand0",          0,                    nand_parents,           ARRAY_SIZE(nand_parents),           &sunxi_clk_periph_nand0            },
	{"nand1",          0,                    nand_parents,           ARRAY_SIZE(nand_parents),           &sunxi_clk_periph_nand1            },
	{"sdmmc0_mod",     0,                    smhc_parents,           ARRAY_SIZE(smhc_parents),           &sunxi_clk_periph_sdmmc0_mod       },
	{"sdmmc0_rst",     0,                    smhc_parents,           ARRAY_SIZE(smhc_parents),           &sunxi_clk_periph_sdmmc0_rst       },
	{"sdmmc0_bus",     0,                    smhc_parents,           ARRAY_SIZE(smhc_parents),           &sunxi_clk_periph_sdmmc0_bus       },
	{"sdmmc1_mod",     0,                    smhc_parents,           ARRAY_SIZE(smhc_parents),           &sunxi_clk_periph_sdmmc1_mod       },
	{"sdmmc1_rst",     0,                    smhc_parents,           ARRAY_SIZE(smhc_parents),           &sunxi_clk_periph_sdmmc1_rst       },
	{"sdmmc1_bus",     0,                    smhc_parents,           ARRAY_SIZE(smhc_parents),           &sunxi_clk_periph_sdmmc1_bus       },
	{"sdmmc2_mod",     0,                    smhc_parents,           ARRAY_SIZE(smhc_parents),           &sunxi_clk_periph_sdmmc2_mod       },
	{"sdmmc2_rst",     0,                    smhc_parents,           ARRAY_SIZE(smhc_parents),           &sunxi_clk_periph_sdmmc2_rst       },
	{"sdmmc2_bus",     0,                    smhc_parents,           ARRAY_SIZE(smhc_parents),           &sunxi_clk_periph_sdmmc2_bus       },
	{"uart0",          0,                    apb2mod_parents,        ARRAY_SIZE(apb2mod_parents),        &sunxi_clk_periph_uart0            },
	{"uart1",          0,                    apb2mod_parents,        ARRAY_SIZE(apb2mod_parents),        &sunxi_clk_periph_uart1            },
	{"uart2",          0,                    apb2mod_parents,        ARRAY_SIZE(apb2mod_parents),        &sunxi_clk_periph_uart2            },
	{"uart3",          0,                    apb2mod_parents,        ARRAY_SIZE(apb2mod_parents),        &sunxi_clk_periph_uart3            },
	{"twi0",           0,                    apb2mod_parents,        ARRAY_SIZE(apb2mod_parents),        &sunxi_clk_periph_twi0             },
	{"twi1",           0,                    apb2mod_parents,        ARRAY_SIZE(apb2mod_parents),        &sunxi_clk_periph_twi1             },
	{"twi2",           0,                    apb2mod_parents,        ARRAY_SIZE(apb2mod_parents),        &sunxi_clk_periph_twi2             },
	{"twi3",           0,                    apb2mod_parents,        ARRAY_SIZE(apb2mod_parents),        &sunxi_clk_periph_twi3             },
	{"twi4",           0,                    apb2mod_parents,        ARRAY_SIZE(apb2mod_parents),        &sunxi_clk_periph_twi4             },
	{"spi0",           0,                    spi_parents,            ARRAY_SIZE(spi_parents),            &sunxi_clk_periph_spi0             },
	{"spi1",           0,                    spi_parents,            ARRAY_SIZE(spi_parents),            &sunxi_clk_periph_spi1             },
	{"gpadc",          0,                    apb1mod_parents,        ARRAY_SIZE(apb1mod_parents),        &sunxi_clk_periph_gpadc            },
	{"ths",            0,                    apb1mod_parents,        ARRAY_SIZE(apb1mod_parents),        &sunxi_clk_periph_ths              },
	{"i2s0",           0,                    audio_parents,          ARRAY_SIZE(audio_parents),          &sunxi_clk_periph_i2s0             },
	{"i2s1",           0,                    audio_parents,          ARRAY_SIZE(audio_parents),          &sunxi_clk_periph_i2s1             },
	{"i2s2",           0,                    audio_parents,          ARRAY_SIZE(audio_parents),          &sunxi_clk_periph_i2s2             },
	{"dmic",           0,                    audio_parents,          ARRAY_SIZE(audio_parents),          &sunxi_clk_periph_dmic             },
	{"codec_1x",       0,                    audio_parents,          ARRAY_SIZE(audio_parents),          &sunxi_clk_periph_codec_1x         },
	{"codec_4x",       0,                    audio_parents,          ARRAY_SIZE(audio_parents),          &sunxi_clk_periph_codec_4x         },
	{"usbphy0",        0,                    hosc_parents,           ARRAY_SIZE(hosc_parents),           &sunxi_clk_periph_usbphy0          },
	{"usbphy1",        0,                    hosc_parents,           ARRAY_SIZE(hosc_parents),           &sunxi_clk_periph_usbphy1          },
	{"usbohci0",       0,                    ahb3mod_parents,        ARRAY_SIZE(ahb3mod_parents),        &sunxi_clk_periph_usbohci0         },
	{"usbohci0_12m",   0,                    usbohci_12m_parents,    ARRAY_SIZE(usbohci_12m_parents),    &sunxi_clk_periph_usbohci0_12m     },
	{"usbohci1",       0,                    ahb3mod_parents,        ARRAY_SIZE(ahb3mod_parents),        &sunxi_clk_periph_usbohci1         },
	{"usbohci1_12m",   0,                    usbohci_12m_parents,    ARRAY_SIZE(usbohci_12m_parents),    &sunxi_clk_periph_usbohci1_12m     },
	{"usbehci0",       0,                    ahb3mod_parents,        ARRAY_SIZE(ahb3mod_parents),        &sunxi_clk_periph_usbehci0         },
	{"usbehci1",       0,                    ahb3mod_parents,        ARRAY_SIZE(ahb3mod_parents),        &sunxi_clk_periph_usbehci1         },
	{"usbotg",         0,                    ahb3mod_parents,        ARRAY_SIZE(ahb3mod_parents),        &sunxi_clk_periph_usbotg           },
	{"mipi_dphy0",     0,                    mipi_dphy_parents,      ARRAY_SIZE(mipi_dphy_parents),      &sunxi_clk_periph_mipi_dphy0       },
	{"mipi_host0",     0,                    mipi_host_parents,      ARRAY_SIZE(mipi_host_parents),      &sunxi_clk_periph_mipi_host0       },
	{"mipi_dphy1",     0,                    mipi_dphy_parents,      ARRAY_SIZE(mipi_dphy_parents),      &sunxi_clk_periph_mipi_dphy1       },
	{"mipi_host1",     0,                    mipi_host_parents,      ARRAY_SIZE(mipi_host_parents),      &sunxi_clk_periph_mipi_host1       },
	{"display_top",    0,                    ahb3mod_parents,        ARRAY_SIZE(ahb3mod_parents),        &sunxi_clk_periph_display_top      },
	{"tcon_lcd0",      0,                    tcon_lcd_parents,       ARRAY_SIZE(tcon_lcd_parents),       &sunxi_clk_periph_tcon_lcd0        },
	{"tcon_lcd1",      0,                    tcon_lcd_parents,       ARRAY_SIZE(tcon_lcd_parents),       &sunxi_clk_periph_tcon_lcd1        },
	{"edp",            0,                    hosc_parents,           ARRAY_SIZE(hosc_parents),           &sunxi_clk_periph_edp              },
	{"csi_misc",       0,                    hosc_parents,           ARRAY_SIZE(hosc_parents),           &sunxi_clk_periph_csi_misc         },
	{"csi_top",        0,                    csi_top_parents,        ARRAY_SIZE(csi_top_parents),        &sunxi_clk_periph_csi_top          },
	{"csi_master",     0,                    csi_master_parents,     ARRAY_SIZE(csi_master_parents),     &sunxi_clk_periph_csi_master       },
	{"pio",            0,                    apb1mod_parents,        ARRAY_SIZE(apb1mod_parents),        &sunxi_clk_periph_pio              },
};

struct periph_init_data sunxi_periphs_cpus_init[] = {
	{"cpurcpus_pll",    CLK_GET_RATE_NOCACHE|CLK_READONLY,  cpurcpus_pll_parents,   ARRAY_SIZE(cpurcpus_pll_parents),   &sunxi_clk_periph_cpurcpus_pll  },
	{"cpurcpus",        CLK_GET_RATE_NOCACHE|CLK_READONLY,  cpurcpus_parents,       ARRAY_SIZE(cpurcpus_parents),       &sunxi_clk_periph_cpurcpus      },
	{"cpurahbs",        CLK_GET_RATE_NOCACHE|CLK_READONLY,  cpurahbs_parents,       ARRAY_SIZE(cpurahbs_parents),       &sunxi_clk_periph_cpurahbs      },
	{"cpurapbs1",       CLK_GET_RATE_NOCACHE|CLK_READONLY,  cpurapbs1_parents,      ARRAY_SIZE(cpurapbs1_parents),      &sunxi_clk_periph_cpurapbs1     },
	{"cpurapbs2_pll",   CLK_GET_RATE_NOCACHE|CLK_READONLY,  cpurapbs2_pll_parents,  ARRAY_SIZE(cpurapbs2_pll_parents),  &sunxi_clk_periph_cpurapbs2_pll },
	{"cpurapbs2",       CLK_GET_RATE_NOCACHE|CLK_READONLY,  cpurapbs2_parents,      ARRAY_SIZE(cpurapbs2_parents),      &sunxi_clk_periph_cpurapbs2     },
	{"losc_out",        0,                                  losc_parents,           ARRAY_SIZE(losc_parents),           &sunxi_clk_periph_losc_out      },
	{"cpurpio",         CLK_GET_RATE_NOCACHE|CLK_READONLY,  cpurpio_parents,        ARRAY_SIZE(cpurpio_parents),        &sunxi_clk_periph_cpurpio       },
};

/**
 * set default rate for clk
 */
static int __set_clk_rates(struct device_node *node, struct clk *clk)
{
	u32 assigned_clock_rates = 0;
	bool res = -1;

	/*set pll default rate here , and make you know it is setted
	 * succeed or not
	 */
	if (!of_property_read_u32(node , "assigned-clock-rates" ,
		&assigned_clock_rates)) {
		u32 real_clock_rate = 0;
		clk_set_rate(clk , assigned_clock_rates);
		real_clock_rate = clk_get_rate(clk);
		if (real_clock_rate != assigned_clock_rates) {
			pr_info("%s-set_default_rate=%u, but real_get_rate=%u failured!\n",
				__clk_get_name(clk),
				assigned_clock_rates,
				real_clock_rate);
		} else {
			pr_info("%s-set_default_rate=%u success!\n",
				__clk_get_name(clk),
				assigned_clock_rates);
			res = 0;
		}
	}

	return res;
}

/**
 * set default clk source for clk
 */
static int __set_clk_parents(struct device_node *node, struct clk *clk)
{
	int index = 0, rc;
	struct of_phandle_args clkspec;
	struct clk *pclk;

	rc = of_parse_phandle_with_args(node, "assigned-clock-parents",
				"#clock-cells", index, &clkspec);
	if (rc < 0) {
		/* skip empty (null) phandles */
		return rc;
	}

	pclk = of_clk_get_from_provider(&clkspec);
	if (IS_ERR(pclk)) {
		pr_warn("clk: couldn't get parent clock %d for %s\n",
				index, node->full_name);
		return PTR_ERR(pclk);
	}

	rc = clk_set_parent(clk, pclk);
	if (rc < 0) {
		pr_err("%s-set_default_source=%s failed at: %d\n",
			__clk_get_name(clk), __clk_get_name(pclk), rc);
	} else {
		pr_info("%s-set_default_source=%s success!\n",
			__clk_get_name(clk), __clk_get_name(pclk));
	}

	return rc;
}

/*
 * of_sunxi_clocks_init() - Clocks initialize
 */
void of_sunxi_clocks_init(struct device_node *node)
{
	sunxi_clk_base = of_iomap(node, 0);
	sunxi_clk_cpus_base = of_iomap(node, 1);
	sunxi_clk_periph_losc_out.gate.bus = of_iomap(node, 2);
	/*do some initialize arguments here*/
	sunxi_clk_factor_initlimits();
}

/*
 * of_sunxi_fixed_clk_setup() - Setup function for fixed clk
 */
void of_sunxi_fixed_clk_setup(struct device_node *node)
{
	struct clk *clk;
	const char *clk_name = node->name;
	u32 rate;

	if (of_property_read_u32(node, "clock-frequency", &rate))
		return;

	of_property_read_string(node, "clock-output-names", &clk_name);

	clk = clk_register_fixed_rate(NULL, clk_name, NULL,
					 CLK_IS_ROOT, rate);
	if (!IS_ERR(clk)) {
		clk_register_clkdev(clk, clk_name, NULL);
		of_clk_add_provider(node, of_clk_src_simple_get, clk);
	}
}

/*
 * of_sunxi_fixed_factor_clk_setup() - Setup function for fixed factors clk
 */
void of_sunxi_fixed_factor_clk_setup(struct device_node *node)
{
	struct clk *clk;
	const char *clk_name = node->name;
	const char *parent_name;
	u32 div, mult;

	if (of_property_read_u32(node, "clock-div", &div)) {
		pr_err("%s Fixed factor clock <%s> must have a clock-div property\n",
			__func__, node->name);
		return;
	}

	if (of_property_read_u32(node, "clock-mult", &mult)) {
		pr_err("%s Fixed factor clock <%s> must have a clokc-mult property\n",
			__func__, node->name);
		return;
	}

	of_property_read_string(node, "clock-output-names", &clk_name);
	parent_name = of_clk_get_parent_name(node, 0);

	clk = clk_register_fixed_factor(NULL, clk_name, parent_name, 0,
					mult, div);
	if (!IS_ERR(clk)) {
		clk_register_clkdev(clk, clk_name, NULL);
		of_clk_add_provider(node, of_clk_src_simple_get, clk);
	}
}

/*
 * of_sunxi_pll_clk_setup() - Setup function for pll factors clk
 */
void of_sunxi_pll_clk_setup(struct device_node *node)
{
	struct clk *clk;
	const char *clk_name = node->name;
	const char *lock_mode = NULL;
	struct factor_init_data *factor;
	int i;
	int ret;

	of_property_read_string(node, "clock-output-names", &clk_name);
	ret = of_property_read_string(node, "lock-mode", &lock_mode);

	/*get pll clk init config */
	for (i = 0; i < ARRAY_SIZE(sunxi_factos); i++) {
		factor = &sunxi_factos[i];
		if (strcmp(clk_name , factor->name))
			continue;

		if (!strcmp(lock_mode, "new"))
			factor->lock_mode = PLL_LOCK_NEW_MODE;
		else if (!strcmp(lock_mode, "old"))
			factor->lock_mode = PLL_LOCK_OLD_MODE;
		else
			factor->lock_mode = PLL_LOCK_NONE_MODE;

		/*register clk */
		clk = sunxi_clk_register_factors(NULL,
				sunxi_clk_base, &clk_lock, factor);
		/*add to of */
		if (!IS_ERR(clk)) {
			clk_register_clkdev(clk, clk_name, NULL);
			of_clk_add_provider(node, of_clk_src_simple_get, clk);
			__set_clk_parents(node, clk);
			__set_clk_rates(node, clk);
			return;
		}
	}

	pr_err("clk %s not found in %s\n", clk_name, __func__);
}

/*
 * of_sunxi_periph_clk_setup() - Setup function for periph clk
 */
void of_sunxi_periph_clk_setup(struct device_node *node)
{
	struct clk *clk;
	const char *clk_name = node->name;
	struct periph_init_data *pd;
	unsigned int i;

	of_property_read_string(node, "clock-output-names", &clk_name);

	/*get pd clk init config */
	for (i = 0; i < ARRAY_SIZE(sunxi_periphs_init); i++) {
		pd = &sunxi_periphs_init[i];
		if (strcmp(clk_name, pd->name))
			continue;

		/*register clk */
		clk = sunxi_clk_register_periph(pd, sunxi_clk_base);
		/*add to of */
		if (!IS_ERR(clk)) {
			clk_register_clkdev(clk, clk_name, NULL);
			of_clk_add_provider(node, of_clk_src_simple_get, clk);
			__set_clk_parents(node, clk);
			__set_clk_rates(node, clk);
			return;
		}
	}
	pr_err("clk %s not found in %s\n", clk_name, __func__);
}

/*
 * of_periph_cpus_clk_setup() - Setup function for periph cpus clk
 */
struct sunxi_reg_ops priv_regops;
void of_sunxi_periph_cpus_clk_setup(struct device_node *node)
{
	struct clk *clk;
	const char *clk_name = node->name;
	struct periph_init_data *pd;
	unsigned int i;

	of_property_read_string(node, "clock-output-names", &clk_name);

	/*get periph cpus clk init config */
	for (i = 0; i < ARRAY_SIZE(sunxi_periphs_cpus_init); i++) {
		pd = &sunxi_periphs_cpus_init[i];
		if (strcmp(clk_name , pd->name))
			continue;

		/* register clk */
		clk = sunxi_clk_register_periph(pd,
				(strcmp(clk_name, "losc_out")
				 ? sunxi_clk_cpus_base
				 : 0));
		/* add to of */
		if (!IS_ERR(clk)) {
			clk_register_clkdev(clk, clk_name, NULL);
			of_clk_add_provider(node, of_clk_src_simple_get, clk);
			__set_clk_parents(node , clk);
			__set_clk_rates(node , clk);
			return;
		}
	}

	pr_err("clk %s not found in %s\n", clk_name, __func__);
}

CLK_OF_DECLARE(sunxi_clocks_init, "allwinner,sunxi-clk-init",
		 of_sunxi_clocks_init);
CLK_OF_DECLARE(sunxi_fixed_clk, "allwinner,fixed-clock",
		 of_sunxi_fixed_clk_setup);
CLK_OF_DECLARE(pll_clk, "allwinner,sunxi-pll-clock",
		 of_sunxi_pll_clk_setup);
CLK_OF_DECLARE(sunxi_fixed_factor_clk, "allwinner,fixed-factor-clock",
		 of_sunxi_fixed_factor_clk_setup);
CLK_OF_DECLARE(periph_clk, "allwinner,sunxi-periph-clock",
		 of_sunxi_periph_clk_setup);
CLK_OF_DECLARE(periph_cpus_clk, "allwinner,sunxi-periph-cpus-clock",
		 of_sunxi_periph_cpus_clk_setup);
