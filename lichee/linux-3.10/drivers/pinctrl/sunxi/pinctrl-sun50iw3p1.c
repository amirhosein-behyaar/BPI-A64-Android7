/*
 * Allwinner sun50iw3p1 SoCs pinctrl driver.
 *
 * Copyright(c) 2016-2020 Allwinnertech Co., Ltd.
 * Author: WimHuang <huangwei@allwinnertech.com>
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/pinctrl/pinctrl.h>

#include "pinctrl-sunxi.h"

static const struct sunxi_desc_pin sun50iw3p1_pins[] = {
	/* HOLE */
	SUNXI_PIN(SUNXI_PINCTRL_PIN(B, 0),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "uart2"),		/* TX */
		  SUNXI_FUNCTION(0x4, "jtag0"),		/* MS0 */
		  SUNXI_FUNCTION(0x7, "io_disabled"),
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 0)),	/* PB_EINT0 */
	SUNXI_PIN(SUNXI_PINCTRL_PIN(B, 1),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "uart2"),		/* RX */
		  SUNXI_FUNCTION(0x4, "jtag0"),		/* CK0 */
		  SUNXI_FUNCTION(0x5, "vdevice"),	/* virtual */
		  SUNXI_FUNCTION(0x7, "io_disabled"),
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 1)),	/* PB_EINT1 */
	SUNXI_PIN(SUNXI_PINCTRL_PIN(B, 2),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "uart2"),		/* RTS */
		  SUNXI_FUNCTION(0x4, "jtag0"),		/* DO0 */
		  SUNXI_FUNCTION(0x5, "vdevice"),	/* virtual */
		  SUNXI_FUNCTION(0x7, "io_disabled"),
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 2)),	/* PB_EINT2 */
	SUNXI_PIN(SUNXI_PINCTRL_PIN(B, 3),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "uart2"),		/* CTS */
		  SUNXI_FUNCTION(0x4, "jtag0"),		/* DI0 */
		  SUNXI_FUNCTION(0x7, "io_disabled"),
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 3)),	/* PB_EINT3 */
	SUNXI_PIN(SUNXI_PINCTRL_PIN(B, 4),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "aif2"),		/* SYNC */
		  SUNXI_FUNCTION(0x3, "i2s0"),		/* LRCK */
		  SUNXI_FUNCTION(0x7, "io_disabled"),
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 4)),	/* PB_EINT4 */
	SUNXI_PIN(SUNXI_PINCTRL_PIN(B, 5),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "aif2"),		/* BCLK */
		  SUNXI_FUNCTION(0x3, "i2s0"),		/* BCLK */
		  SUNXI_FUNCTION(0x7, "io_disabled"),
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 5)),	/* PB_EINT5 */
	SUNXI_PIN(SUNXI_PINCTRL_PIN(B, 6),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "aif2"),		/* DOUT */
		  SUNXI_FUNCTION(0x3, "i2s0"),		/* DOUT */
		  SUNXI_FUNCTION(0x7, "io_disabled"),
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 6)),	/* PB_EINT6 */
	SUNXI_PIN(SUNXI_PINCTRL_PIN(B, 7),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "aif2"),		/* DIN */
		  SUNXI_FUNCTION(0x3, "i2s0"),		/* DIN */
		  SUNXI_FUNCTION(0x7, "io_disabled"),
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 7)),	/* PB_EINT7 */
	SUNXI_PIN(SUNXI_PINCTRL_PIN(B, 8),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x3, "i2s0"),		/* MCLK */
		  SUNXI_FUNCTION(0x7, "io_disabled"),
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 8)),	/* PB_EINT8 */
	SUNXI_PIN(SUNXI_PINCTRL_PIN(B, 9),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x4, "uart0"),		/* TX */
		  SUNXI_FUNCTION(0x7, "io_disabled"),
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 9)),	/* PB_EINT9 */
	SUNXI_PIN(SUNXI_PINCTRL_PIN(B, 10),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x4, "uart0"),		/* RX */
		  SUNXI_FUNCTION(0x7, "io_disabled"),
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 10)),	/* PB_EINT10 */

	/* HOLE */
	SUNXI_PIN(SUNXI_PINCTRL_PIN(C, 0),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "nand0"),		/* WE */
		SUNXI_FUNCTION(0x4, "spi0"),		/* CLK */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(C, 1),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "nand0"),		/* ALE */
		SUNXI_FUNCTION(0x3, "sdc2"),		/* DS */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(C, 2),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "nand0"),		/* CLE */
		SUNXI_FUNCTION(0x4, "spi0"),		/* MOSI */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(C, 3),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "nand0"),		/* CE0 */
		SUNXI_FUNCTION(0x4, "spi0"),		/* MISO */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(C, 4),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "nand0"),		/* RE */
		SUNXI_FUNCTION(0x3, "sdc2"),		/* CLK */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(C, 5),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "nand0"),		/* RB0 */
		SUNXI_FUNCTION(0x3, "sdc2"),		/* CMD */
		SUNXI_FUNCTION(0x4, "spi0"),		/* CS */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(C, 6),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "nand0"),		/* DQ0 */
		SUNXI_FUNCTION(0x3, "sdc2"),		/* D0 */
		SUNXI_FUNCTION(0x4, "spi0"),		/* HOLD */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(C, 7),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "nand0"),		/* DQ1 */
		SUNXI_FUNCTION(0x3, "sdc2"),		/* D1 */
		SUNXI_FUNCTION(0x4, "spi0"),		/* WP */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(C, 8),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "nand0"),		/* DQ2 */
		SUNXI_FUNCTION(0x3, "sdc2"),		/* D2 */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(C, 9),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "nand0"),		/* DQ3 */
		SUNXI_FUNCTION(0x3, "sdc2"),		/* D3 */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(C, 10),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "nand0"),		/* DQ4 */
		SUNXI_FUNCTION(0x3, "sdc2"),		/* D4 */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(C, 11),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "nand0"),		/* DQ5 */
		SUNXI_FUNCTION(0x3, "sdc2"),		/* D5 */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(C, 12),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "nand0"),		/* DQ6 */
		SUNXI_FUNCTION(0x3, "sdc2"),		/* D6 */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(C, 13),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "nand0"),		/* DQ7 */
		SUNXI_FUNCTION(0x3, "sdc2"),		/* D7 */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(C, 14),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "nand0"),		/* DQS */
		SUNXI_FUNCTION(0x3, "sdc2"),		/* RST */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(C, 15),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "nand0"),		/* CE1 */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(C, 16),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "nand0"),		/* NB1 */
		SUNXI_FUNCTION(0x7, "io_disabled")),

	/* HOLE */
	SUNXI_PIN(SUNXI_PINCTRL_PIN(D, 0),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "pwm0"),		/* PWM0 */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(D, 1),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "pwm1"),		/* PWM1 */
		SUNXI_FUNCTION(0x3, "cpu"),		/* CUR_W */
		SUNXI_FUNCTION(0x7, "io_disabled")),

	/* HOLE */
	SUNXI_PIN(SUNXI_PINCTRL_PIN(E, 0),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "csi0"),		/* PCLK */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(E, 1),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "csi0"),		/* MCLK */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(E, 2),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "csi0"),		/* HSYNC */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(E, 3),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "csi0"),		/* VSYNC */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(E, 4),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "csi0"),		/* D0 */
		SUNXI_FUNCTION(0x3, "bist"),		/* RESULT0 */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(E, 5),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "csi0"),		/* D1 */
		SUNXI_FUNCTION(0x3, "bist"),		/* RESULT1 */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(E, 6),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "csi0"),		/* D2 */
		SUNXI_FUNCTION(0x3, "bist"),		/* RESULT2 */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(E, 7),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "csi0"),		/* D3 */
		SUNXI_FUNCTION(0x3, "bist"),		/* RESULT3 */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(E, 8),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "csi0"),		/* D4 */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(E, 9),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "csi0"),		/* D5 */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(E, 10),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "csi0"),		/* D6 */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(E, 11),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "csi0"),		/* D7 */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(E, 12),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "csi0"),		/* CLK */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(E, 13),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "csi0"),		/* SDA */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(E, 14),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "csi0"),		/* D8 */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(E, 15),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "csi0"),		/* D9 */
		SUNXI_FUNCTION(0x2, "pll"),		/* LOCK_DBG */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(E, 16),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x3, "twi2"),		/* SCK */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(E, 17),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x3, "twi2"),		/* SDA */
		SUNXI_FUNCTION(0x7, "io_disabled")),

	/* HOLE */
	SUNXI_PIN(SUNXI_PINCTRL_PIN(F, 0),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "sdc0"),		/* D1 */
		SUNXI_FUNCTION(0x3, "jtag0"),		/* MS1 */
		SUNXI_FUNCTION(0x7, "io_disabled"),
		SUNXI_FUNCTION_IRQ_BANK(0x6, 1, 0)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(F, 1),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "sdc0"),		/* D0 */
		SUNXI_FUNCTION(0x3, "jtag0"),		/* DI1 */
		SUNXI_FUNCTION(0x7, "io_disabled"),
		SUNXI_FUNCTION_IRQ_BANK(0x6, 1, 1)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(F, 2),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "sdc0"),		/* CLK */
		SUNXI_FUNCTION(0x3, "uart0"),		/* TX */
		SUNXI_FUNCTION(0x7, "io_disabled"),
		SUNXI_FUNCTION_IRQ_BANK(0x6, 1, 2)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(F, 3),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "sdc0"),		/* CMD */
		SUNXI_FUNCTION(0x3, "jtag0"),		/* DO1 */
		SUNXI_FUNCTION(0x7, "io_disabled"),
		SUNXI_FUNCTION_IRQ_BANK(0x6, 1, 3)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(F, 4),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "sdc0"),		/* D3 */
		SUNXI_FUNCTION(0x3, "uart0"),		/* RX */
		SUNXI_FUNCTION(0x7, "io_disabled"),
		SUNXI_FUNCTION_IRQ_BANK(0x6, 1, 4)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(F, 5),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "sdc0"),		/* D2 */
		SUNXI_FUNCTION(0x3, "jtag0"),		/* CK1 */
		SUNXI_FUNCTION(0x7, "io_disabled"),
		SUNXI_FUNCTION_IRQ_BANK(0x6, 1, 5)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(F, 6),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x7, "io_disabled"),
		SUNXI_FUNCTION_IRQ_BANK(0x6, 1, 6)),

#if defined(CONFIG_FPGA_V4_PLATFORM)
	SUNXI_PIN(SUNXI_PINCTRL_PIN(F, 16),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "sdc2"),		/* d0 */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(F, 17),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "sdc2"),		/* d1 */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(F, 18),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "sdc2"),		/* d2 */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(F, 19),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "sdc2"),		/* d3 */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(F, 20),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "sdc2"),		/* d4 */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(F, 21),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "sdc2"),		/* d5 */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(F, 22),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "sdc2"),		/* d6 */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(F, 23),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "sdc2"),		/* d7 */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(F, 24),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "sdc2"),		/* cmd */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(F, 25),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "sdc2"),		/* clk */
		SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(F, 27),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "sdc2"),		/* ds */
		SUNXI_FUNCTION(0x7, "io_disabled")),
#endif

	/* HOLE */
	SUNXI_PIN(SUNXI_PINCTRL_PIN(G, 0),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "sdc1"),		/* CLK */
		SUNXI_FUNCTION(0x7, "io_disabled"),
		SUNXI_FUNCTION_IRQ_BANK(0x6, 2, 0)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(G, 1),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "sdc1"),		/* CMD */
		SUNXI_FUNCTION(0x7, "io_disabled"),
		SUNXI_FUNCTION_IRQ_BANK(0x6, 2, 1)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(G, 2),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "sdc1"),		/* D0 */
		SUNXI_FUNCTION(0x7, "io_disabled"),
		SUNXI_FUNCTION_IRQ_BANK(0x6, 2, 2)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(G, 3),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "sdc1"),		/* D1 */
		SUNXI_FUNCTION(0x7, "io_disabled"),
		SUNXI_FUNCTION_IRQ_BANK(0x6, 2, 3)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(G, 4),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "sdc1"),		/* D2 */
		SUNXI_FUNCTION(0x7, "io_disabled"),
		SUNXI_FUNCTION_IRQ_BANK(0x6, 2, 4)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(G, 5),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "sdc1"),		/* D3 */
		SUNXI_FUNCTION(0x7, "io_disabled"),
		SUNXI_FUNCTION_IRQ_BANK(0x6, 2, 5)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(G, 6),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "uart1"),		/* TX */
		SUNXI_FUNCTION(0x7, "io_disabled"),
		SUNXI_FUNCTION_IRQ_BANK(0x6, 2, 6)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(G, 7),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "uart1"),		/* RX */
		SUNXI_FUNCTION(0x7, "io_disabled"),
		SUNXI_FUNCTION_IRQ_BANK(0x6, 2, 7)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(G, 8),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "uart1"),		/* RTS */
		SUNXI_FUNCTION(0x7, "io_disabled"),
		SUNXI_FUNCTION_IRQ_BANK(0x6, 2, 8)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(G, 9),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "uart1"),		/* CTS */
		SUNXI_FUNCTION(0x3, "i2s1"),		/* MCLK */
		SUNXI_FUNCTION(0x7, "io_disabled"),
		SUNXI_FUNCTION_IRQ_BANK(0x6, 2, 9)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(G, 10),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "aif3"),		/* SYNC */
		SUNXI_FUNCTION(0x3, "i2s1"),		/* LRCK */
		SUNXI_FUNCTION(0x7, "io_disabled"),
		SUNXI_FUNCTION_IRQ_BANK(0x6, 2, 10)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(G, 11),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "aif3"),		/* BCLK */
		SUNXI_FUNCTION(0x3, "i2s1"),		/* BCLK */
		SUNXI_FUNCTION(0x7, "io_disabled"),
		SUNXI_FUNCTION_IRQ_BANK(0x6, 2, 11)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(G, 12),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "aif3"),		/* DOUT */
		SUNXI_FUNCTION(0x3, "i2s1"),		/* DOUT */
		SUNXI_FUNCTION(0x7, "io_disabled"),
		SUNXI_FUNCTION_IRQ_BANK(0x6, 2, 12)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(G, 13),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "aif3"),		/* DIN */
		SUNXI_FUNCTION(0x3, "i2s1"),		/* DIN */
		SUNXI_FUNCTION(0x7, "io_disabled"),
		SUNXI_FUNCTION_IRQ_BANK(0x6, 2, 13)),

	/* HOLE */
	SUNXI_PIN(SUNXI_PINCTRL_PIN(H, 0),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "twi0"),		/* SCK */
		SUNXI_FUNCTION(0x7, "io_disabled"),
		SUNXI_FUNCTION_IRQ_BANK(0x6, 3, 0)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(H, 1),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "twi0"),		/* SDA */
		SUNXI_FUNCTION(0x7, "io_disabled"),
		SUNXI_FUNCTION_IRQ_BANK(0x6, 3, 1)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(H, 2),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "twi1"),		/* SCK */
		SUNXI_FUNCTION(0x3, "cpu"),		/* CUR_W */
		SUNXI_FUNCTION(0x7, "io_disabled"),
		SUNXI_FUNCTION_IRQ_BANK(0x6, 3, 2)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(H, 3),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "twi1"),		/* SDA */
		SUNXI_FUNCTION(0x7, "io_disabled"),
		SUNXI_FUNCTION_IRQ_BANK(0x6, 3, 3)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(H, 4),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "uart3"),		/* TX */
		SUNXI_FUNCTION(0x3, "i2s2"),		/* MCLK */
		SUNXI_FUNCTION(0x4, "spi1"),		/* CS */
		SUNXI_FUNCTION(0x5, "twi3"),		/* SCK */
		SUNXI_FUNCTION(0x7, "io_disabled"),
		SUNXI_FUNCTION_IRQ_BANK(0x6, 3, 4)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(H, 5),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "uart3"),		/* RX */
		SUNXI_FUNCTION(0x3, "i2s2"),		/* LRCK */
		SUNXI_FUNCTION(0x4, "spi1"),		/* CLK */
		SUNXI_FUNCTION(0x5, "twi3"),		/* SDA */
		SUNXI_FUNCTION(0x7, "io_disabled"),
		SUNXI_FUNCTION_IRQ_BANK(0x6, 3, 5)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(H, 6),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "uart3"),		/* RTS */
		SUNXI_FUNCTION(0x3, "i2s2"),		/* BCLK */
		SUNXI_FUNCTION(0x4, "spi1"),		/* MOSI */
		SUNXI_FUNCTION(0x4, "twi4"),		/* SCK */
		SUNXI_FUNCTION(0x7, "io_disabled"),
		SUNXI_FUNCTION_IRQ_BANK(0x6, 3, 6)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(H, 7),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "uart3"),		/* CTS */
		SUNXI_FUNCTION(0x3, "i2s2"),		/* DOUT */
		SUNXI_FUNCTION(0x4, "spi1"),		/* MISO */
		SUNXI_FUNCTION(0x4, "twi4"),		/* SDA */
		SUNXI_FUNCTION(0x7, "io_disabled"),
		SUNXI_FUNCTION_IRQ_BANK(0x6, 3, 7)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(H, 8),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "mic"),		/* DATA3 */
		SUNXI_FUNCTION(0x3, "i2s2"),		/* DIN */
		SUNXI_FUNCTION(0x7, "io_disabled"),
		SUNXI_FUNCTION_IRQ_BANK(0x6, 3, 8)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(H, 9),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "mic"),		/* DATA2 */
		SUNXI_FUNCTION(0x7, "io_disabled"),
		SUNXI_FUNCTION_IRQ_BANK(0x6, 3, 9)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(H, 10),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "mic"),		/* DATA1 */
		SUNXI_FUNCTION(0x7, "io_disabled"),
		SUNXI_FUNCTION_IRQ_BANK(0x6, 3, 10)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(H, 11),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "mic"),		/* DATA0 */
		SUNXI_FUNCTION(0x7, "io_disabled"),
		SUNXI_FUNCTION_IRQ_BANK(0x6, 3, 11)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(H, 12),
		SUNXI_FUNCTION(0x0, "gpio_in"),
		SUNXI_FUNCTION(0x1, "gpio_out"),
		SUNXI_FUNCTION(0x2, "mic"),		/* CLK */
		SUNXI_FUNCTION(0x7, "io_disabled"),
		SUNXI_FUNCTION_IRQ_BANK(0x6, 3, 12)),
};

static const struct sunxi_pinctrl_desc sun50iw3p1_pinctrl_data = {
	.pins = sun50iw3p1_pins,
	.npins = ARRAY_SIZE(sun50iw3p1_pins),
	.pin_base = 0,
	.irq_banks = 4,
};

static int sun50iw3p1_pinctrl_probe(struct platform_device *pdev)
{
	return sunxi_pinctrl_init(pdev, &sun50iw3p1_pinctrl_data);
}

static struct of_device_id sun50iw3p1_pinctrl_match[] = {
	{ .compatible = "allwinner,sun50iw3p1-pinctrl", },
	{}
};
MODULE_DEVICE_TABLE(of, sun50iw3p1_pinctrl_match);

static struct platform_driver sun50iw3p1_pinctrl_driver = {
	.probe	= sun50iw3p1_pinctrl_probe,
	.driver	= {
		.name		= "sun50iw3p1-pinctrl",
		.owner		= THIS_MODULE,
		.of_match_table	= sun50iw3p1_pinctrl_match,
	},
};

static int __init sun50iw3p1_pio_init(void)
{
	int ret;
	ret = platform_driver_register(&sun50iw3p1_pinctrl_driver);
	if (IS_ERR_VALUE(ret)) {
		pr_err("register sun50iw3p1 pio controller failed\n");
		return -EINVAL;
	}
	return 0;
}
postcore_initcall(sun50iw3p1_pio_init);

MODULE_AUTHOR("WimHuang<huangwei@allwinnertech.com>");
MODULE_DESCRIPTION("Allwinner sun50iw3p1 pio pinctrl driver");
MODULE_LICENSE("GPL");
