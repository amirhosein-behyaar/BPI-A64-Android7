/**
 * Copyright (C) 2015-2016 Allwinner Technology Limited. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * Author: Albert Yu <yuxyun@allwinnertech.com>
 */

#ifndef _MALI_SUN8I_W7P1_H_
#define _MALI_SUN8I_W7P1_H_

#define MALI_PP_CORES_NUM 2
#define AW_MALI_GPU_RESOURCES_MALI400_MPX MALI_GPU_RESOURCES_MALI400_MP2

#define GPU_PBASE           SUNXI_GPU_PBASE
#define IRQ_GPU_GP          SUNXI_IRQ_GPU_GP
#define IRQ_GPU_GPMMU       SUNXI_IRQ_GPU_GPMMU
#define IRQ_GPU_PP0         SUNXI_IRQ_GPU_PP0
#define IRQ_GPU_PPMMU0      SUNXI_IRQ_GPU_PPMMU0
#define IRQ_GPU_PP1         SUNXI_IRQ_GPU_PP1
#define IRQ_GPU_PPMMU1      SUNXI_IRQ_GPU_PPMMU1

aw_private_data aw_private = {
	.tempctrl      = {
		.temp_ctrl_status = 1,
	},
	.pm            = {
		.regulator      = NULL,
		.regulator_id   = "vdd-gpu",
		.clk[0]         = {
			.clk_name   = "pll",
			.clk_id     = PLL_GPU_CLK,
			.clk_handle = NULL,
		},
		.clk[1]         = {
			.clk_name   = "mali",
			.clk_id     = GPU_CLK,
			.clk_handle = NULL,
		},
		.vf_table[0]   = {
			.vol  = 1200,
			.freq = 144,
		},
		.vf_table[1]   = {
			.vol  = 1200,
			.freq = 264,
		},
		.vf_table[2]   = {
			.vol  = 1100,
			.freq = 384,
		},
		.vf_table[3]   = {
			.vol  = 1200,
			.freq = 456,
		},
		.vf_table[4]   = {
			.vol  = 1200,
			.freq = 576,
		},
		.dvfs_status       = 0,
		.begin_level       = 4,
		.max_level         = 4,
		.scene_ctrl_cmd    = 0,
		.scene_ctrl_status = 1,
		.independent_pow   = 0,
		.dvm               = 0,
	},
	.debug           = {
		.enable      = 0,
		.frequency   = 0,
		.voltage     = 0,
		.tempctrl    = 0,
		.scenectrl   = 0,
		.dvfs        = 0,
		.level       = 0,
	}
};

#endif
