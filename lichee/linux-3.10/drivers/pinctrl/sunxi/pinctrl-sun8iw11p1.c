/*
 * Allwinner SoCs(sun8iw11p1) pinctrl driver.
 *
 * Copyright (C) 2014 Jackie Hwang
 *
 * Jackie Hwang <huangshr@allwinnertech.com>
 *
 * Copyright (C) 2014 Chen-Yu Tsai
 *
 * Chen-Yu Tsai <wens@csie.org>
 *
 * Copyright (C) 2014 Maxime Ripard
 *
 * Maxime Ripard <maxime.ripard@free-electrons.com>
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
static const struct sunxi_desc_pin sun8iw11p1_pins[] = {
	SUNXI_PIN(SUNXI_PINCTRL_PIN(A, 0),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "emac0"),		/* ERXD3 */
		  SUNXI_FUNCTION(0x3, "spi1"),		/* CS0 */
		  SUNXI_FUNCTION(0x4, "uart2"),		/* RTS */
		  SUNXI_FUNCTION(0x5, "gmac0"),		/* GRXD3 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(A, 1),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "emac0"),		/* ERXD2 */
		  SUNXI_FUNCTION(0x3, "spi1"),		/* CLK */
		  SUNXI_FUNCTION(0x4, "uart2"),		/* CTS */
		  SUNXI_FUNCTION(0x5, "gmac0"),		/* GRXD2 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(A, 2),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "emac0"),		/* ERXD1 */
		  SUNXI_FUNCTION(0x3, "spi1"),		/* MOSI */
		  SUNXI_FUNCTION(0x4, "uart2"),		/* TX */
		  SUNXI_FUNCTION(0x5, "gmac0"),		/* GRXD1 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(A, 3),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "emac0"),		/* ERXD0 */
		  SUNXI_FUNCTION(0x3, "spi1"),		/* MISO */
		  SUNXI_FUNCTION(0x4, "uart2"),		/* RX */
		  SUNXI_FUNCTION(0x5, "gmac0"),		/* GRXD0 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(A, 4),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "emac0"),		/* ETXD3 */
		  SUNXI_FUNCTION(0x3, "spi1"),		/* CS1 */
		  SUNXI_FUNCTION(0x5, "gmac0"),		/* GTXD3 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(A, 5),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "emac0"),		/* ETXD2 */
		  SUNXI_FUNCTION(0x3, "spi3"),		/* CS0 */
		  SUNXI_FUNCTION(0x5, "gmac0"),		/* GTXD2 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(A, 6),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "emac0"),		/* ETXD1 */
		  SUNXI_FUNCTION(0x3, "spi3"),		/* CLK */
		  SUNXI_FUNCTION(0x5, "gmac0"),		/* GTXD1 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(A, 7),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "emac0"),		/* ETXD0 */
		  SUNXI_FUNCTION(0x3, "spi3"),		/* MOSI */
		  SUNXI_FUNCTION(0x5, "gmac0"),		/* GTXD0 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(A, 8),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "emac0"),		/* ERXCK */
		  SUNXI_FUNCTION(0x3, "spi3"),		/* MISO */
		  SUNXI_FUNCTION(0x5, "gmac0"),		/* GRXCK */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(A, 9),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "emac0"),		/* ERXERR */
		  SUNXI_FUNCTION(0x3, "spi3"),		/* CS1 */
		  SUNXI_FUNCTION(0x5, "gmac0"),		/* GNULL / ERXERR */
		  SUNXI_FUNCTION(0x6, "i2s1"),		/* MCLK */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(A, 10),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "emac0"),		/* ERXDV */
		  SUNXI_FUNCTION(0x4, "uart1"),		/* TX */
		  SUNXI_FUNCTION(0x5, "gmac0"),		/* GRXCTL / ERXDV */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(A, 11),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "emac0"),		/* EMDC */
		  SUNXI_FUNCTION(0x4, "uart1"),		/* RX */
		  SUNXI_FUNCTION(0x5, "gmac0"),		/* GMDC */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(A, 12),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "emac0"),		/* EMDIO */
		  SUNXI_FUNCTION(0x3, "uart6"),		/* TX */
		  SUNXI_FUNCTION(0x4, "uart1"),		/* RTS */
		  SUNXI_FUNCTION(0x5, "gmac0"),		/* GMDIO */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(A, 13),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "emac0"),		/* ETXEN */
		  SUNXI_FUNCTION(0x3, "uart6"),		/* RX */
		  SUNXI_FUNCTION(0x4, "uart1"),		/* CTS */
		  SUNXI_FUNCTION(0x5, "gmac0"),		/* GTXCTL / ETXEN */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(A, 14),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "emac0"),		/* ETXCK */
		  SUNXI_FUNCTION(0x3, "uart7"),		/* TX */
		  SUNXI_FUNCTION(0x4, "uart1"),		/* DTR */
		  SUNXI_FUNCTION(0x5, "gmac0"),		/* GNULL / ETXCK */
		  SUNXI_FUNCTION(0x6, "i2s1"),		/* BCLK */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(A, 15),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "emac0"),		/* ECRS */
		  SUNXI_FUNCTION(0x3, "uart7"),		/* RX */
		  SUNXI_FUNCTION(0x4, "uart1"),		/* DSR */
		  SUNXI_FUNCTION(0x5, "gmac0"),		/* GTXCK / ECRS */
		  SUNXI_FUNCTION(0x6, "i2s1"),		/* LRCK */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(A, 16),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "emac0"),		/* ECOL */
		  SUNXI_FUNCTION(0x3, "can0"),		/* TX */
		  SUNXI_FUNCTION(0x4, "uart1"),		/* DCD */
		  SUNXI_FUNCTION(0x5, "gmac0"),		/* GCLKIN / ECOL */
		  SUNXI_FUNCTION(0x6, "i2s1"),		/* DO */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(A, 17),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "emac0"),		/* ETXERR */
		  SUNXI_FUNCTION(0x3, "can0"),		/* RX */
		  SUNXI_FUNCTION(0x4, "uart1"),		/* RING */
		  SUNXI_FUNCTION(0x5, "gmac0"),		/* GNULL / ETXERR */
		  SUNXI_FUNCTION(0x6, "i2s1"),		/* DI */
		  SUNXI_FUNCTION(0x7, "io_disabled")),

	/* Hole */
	SUNXI_PIN(SUNXI_PINCTRL_PIN(B, 0),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "twi0"),		/* SCK */
		  SUNXI_FUNCTION(0x3, "pll_lock_dbg"),
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(B, 1),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "twi0"),		/* SDA */
		  SUNXI_FUNCTION(0x4, "vdevice"),	/* vdevice */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(B, 2),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x3, "pwm0"),		/* PWM0 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(B, 3),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x3, "pwm1"),		/* PWM1 */
		  SUNXI_FUNCTION(0x4, "spdif0"),	/* MCLK */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(B, 4),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "ir0"),		/* RX */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(B, 5),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "i2s0"),		/* MCLK */
		  SUNXI_FUNCTION(0x3, "ac97"),		/* MCLK */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(B, 6),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "i2s0"),		/* BCLK */
		  SUNXI_FUNCTION(0x3, "ac97"),		/* BCLK */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(B, 7),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "i2s0"),		/* LRCK */
		  SUNXI_FUNCTION(0x3, "ac97"),		/* SYNC */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(B, 8),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "i2s0"),		/* DO0 */
		  SUNXI_FUNCTION(0x3, "ac97"),		/* DO */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(B, 9),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "i2s0"),		/* DO1 */
		  SUNXI_FUNCTION(0x4, "pwm6"),		/* PWM6 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(B, 10),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "i2s0"),		/* DO2 */
		  SUNXI_FUNCTION(0x4, "pwm7"),		/* PWM7 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(B, 11),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "i2s0"),		/* DO3 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(B, 12),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "i2s0"),		/* DI */
		  SUNXI_FUNCTION(0x3, "ac97"),		/* DI */
		  SUNXI_FUNCTION(0x4, "spdif0"),	/* DI */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(B, 13),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "spi2"),		/* CS1 */
		  SUNXI_FUNCTION(0x4, "spdif0"),	/* DO */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(B, 14),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "spi2"),		/* CS0 */
		  SUNXI_FUNCTION(0x3, "jtag0"),		/* MS0 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(B, 15),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "spi2"),		/* CLK */
		  SUNXI_FUNCTION(0x3, "jtag0"),		/* CK0 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(B, 16),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "spi2"),		/* MOSI */
		  SUNXI_FUNCTION(0x3, "jtag0"),		/* DO0 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(B, 17),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "spi2"),		/* MISO */
		  SUNXI_FUNCTION(0x3, "jtag0"),		/* DI0 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(B, 18),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "twi1"),		/* SCK */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(B, 19),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "twi1"),		/* SDA */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(B, 20),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "twi2"),		/* SCK */
		  SUNXI_FUNCTION(0x4, "pwm4"),		/* PWM4 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(B, 21),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "twi2"),		/* SDA */
		  SUNXI_FUNCTION(0x4, "pwm5"),		/* PWM5 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(B, 22),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "uart0"),		/* TX */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(B, 23),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "uart0"),		/* RX */
		  SUNXI_FUNCTION(0x3, "ir1"),		/* RX */
		  SUNXI_FUNCTION(0x7, "io_disabled")),

	/*Hole*/
	SUNXI_PIN(SUNXI_PINCTRL_PIN(C, 0),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "nand0"),		/* NWE */
		  SUNXI_FUNCTION(0x3, "spi0"),		/* MOSI */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(C, 1),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "nand0"),		/* NALE */
		  SUNXI_FUNCTION(0x3, "spi0"),		/* MISO */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(C, 2),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "nand0"),		/* NCLE */
		  SUNXI_FUNCTION(0x3, "spi0"),		/* SCK */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(C, 3),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "nand0"),		/* NCE1 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(C, 4),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "nand0"),		/* NCE0 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(C, 5),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "nand0"),		/* NRE# */
		  SUNXI_FUNCTION(0x3, "sdc2"),		/* DS */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(C, 6),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "nand0"),		/* NRB0 */
		  SUNXI_FUNCTION(0x3, "sdc2"),		/* CMD */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(C, 7),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "nand0"),		/* NRB1 */
		  SUNXI_FUNCTION(0x3, "sdc2"),		/* CLK */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(C, 8),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "nand0"),		/* NDQ0 */
		  SUNXI_FUNCTION(0x3, "sdc2"),		/* D0 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(C, 9),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "nand0"),		/* NDQ1 */
		  SUNXI_FUNCTION(0x3, "sdc2"),		/* D1 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(C, 10),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "nand0"),		/* NDQ2 */
		  SUNXI_FUNCTION(0x3, "sdc2"),		/* D2 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(C, 11),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "nand0"),		/* NDQ3 */
		  SUNXI_FUNCTION(0x3, "sdc2"),		/* D3 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(C, 12),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "nand0"),		/* NDQ4 */
		  SUNXI_FUNCTION(0x3, "sdc2"),		/* D4 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(C, 13),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "nand0"),		/* NDQ5 */
		  SUNXI_FUNCTION(0x3, "sdc2"),		/* D5 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(C, 14),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "nand0"),		/* NDQ6 */
		  SUNXI_FUNCTION(0x3, "sdc2"),		/* D6 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(C, 15),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "nand0"),		/* NDQ7 */
		  SUNXI_FUNCTION(0x3, "sdc2"),		/* D7 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(C, 16),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "nand0"),		/* NWP */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(C, 17),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "nand0"),		/* NCE2 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(C, 18),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "nand0"),		/* NCE3 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(C, 19),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "nand0"),		/* NCE4 */
		  SUNXI_FUNCTION(0x3, "spi2"),		/* CS0 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(C, 20),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "nand0"),		/* NCE5 */
		  SUNXI_FUNCTION(0x3, "spi2"),		/* CLK */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(C, 21),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "nand0"),		/* NCE6 */
		  SUNXI_FUNCTION(0x3, "spi2"),		/* MOSI */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(C, 22),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "nand0"),		/* NCE7 */
		  SUNXI_FUNCTION(0x3, "spi2"),		/* MISO */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(C, 23),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x3, "spi0"),		/* CS0 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(C, 24),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "nand0"),		/* NDQS */
		  SUNXI_FUNCTION(0x3, "sdc2"),		/* RST */
		  SUNXI_FUNCTION(0x7, "io_disabled")),

	/* Hole */
	SUNXI_PIN(SUNXI_PINCTRL_PIN(D, 0),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd0"),		/* D0 */
		  SUNXI_FUNCTION(0x3, "lvds0"),		/* VP0 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(D, 1),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd0"),		/* D1 */
		  SUNXI_FUNCTION(0x3, "lvds0"),		/* VN0 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(D, 2),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd0"),		/* D2 */
		  SUNXI_FUNCTION(0x3, "lvds0"),		/* VP1 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(D, 3),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd0"),		/* D3 */
		  SUNXI_FUNCTION(0x3, "lvds0"),		/* VN1 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(D, 4),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd0"),		/* D4 */
		  SUNXI_FUNCTION(0x3, "lvds0"),		/* VP2 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(D, 5),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd0"),		/* D5 */
		  SUNXI_FUNCTION(0x3, "lvds0"),		/* VN2 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(D, 6),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd0"),		/* D6 */
		  SUNXI_FUNCTION(0x3, "lvds0"),		/* VPC */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(D, 7),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd0"),		/* D7 */
		  SUNXI_FUNCTION(0x3, "lvds0"),		/* VNC */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(D, 8),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd0"),		/* D8 */
		  SUNXI_FUNCTION(0x3, "lvds0"),		/* VP3 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(D, 9),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd0"),		/* D9 */
		  SUNXI_FUNCTION(0x3, "lvds0"),		/* VM3 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(D, 10),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd0"),		/* D10 */
		  SUNXI_FUNCTION(0x3, "lvds1"),		/* VP0 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(D, 11),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd0"),		/* D11 */
		  SUNXI_FUNCTION(0x3, "lvds1"),		/* VN0 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(D, 12),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd0"),		/* D12 */
		  SUNXI_FUNCTION(0x3, "lvds1"),		/* VP1 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(D, 13),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd0"),		/* D13 */
		  SUNXI_FUNCTION(0x3, "lvds1"),		/* VN1 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(D, 14),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd0"),		/* D14 */
		  SUNXI_FUNCTION(0x3, "lvds1"),		/* VP2 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(D, 15),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd0"),		/* D15 */
		  SUNXI_FUNCTION(0x3, "lvds1"),		/* VN2 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(D, 16),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd0"),		/* D16 */
		  SUNXI_FUNCTION(0x3, "lvds1"),		/* VPC */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(D, 17),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd0"),		/* D17 */
		  SUNXI_FUNCTION(0x3, "lvds1"),		/* VNC */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(D, 18),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd0"),		/* D18 */
		  SUNXI_FUNCTION(0x3, "lvds1"),		/* VP3 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(D, 19),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd0"),		/* D19 */
		  SUNXI_FUNCTION(0x3, "lvds1"),		/* VN3 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(D, 20),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd0"),		/* D20 */
		  SUNXI_FUNCTION(0x3, "csi1"),		/* MCLK */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(D, 21),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd0"),		/* D21 */
		  SUNXI_FUNCTION(0x3, "sim0"),		/* VPPEN */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(D, 22),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd0"),		/* D22 */
		  SUNXI_FUNCTION(0x3, "sim0"),		/* VPPPP */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(D, 23),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd0"),		/* D23 */
		  SUNXI_FUNCTION(0x3, "sim0"),		/* DET */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(D, 24),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd0"),		/* CLK */
		  SUNXI_FUNCTION(0x3, "sim0"),		/* VCCEN */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(D, 25),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd0"),		/* DE */
		  SUNXI_FUNCTION(0x3, "sim0"),		/* RST */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(D, 26),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd0"),		/* HSYNC */
		  SUNXI_FUNCTION(0x3, "sim0"),		/* SLK */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(D, 27),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd0"),		/* VSYNC */
		  SUNXI_FUNCTION(0x3, "sim0"),		/* SDA */
		  SUNXI_FUNCTION(0x7, "io_disabled")),

	/* Hole */
	SUNXI_PIN(SUNXI_PINCTRL_PIN(E, 0),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "ts0"),		/* CLK */
		  SUNXI_FUNCTION(0x3, "csi0"),		/* PCLK */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(E, 1),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "ts0"),		/* ERR */
		  SUNXI_FUNCTION(0x3, "csi0"),		/* MCLK */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(E, 2),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "ts0"),		/* SYNC */
		  SUNXI_FUNCTION(0x3, "csi0"),		/* HSYNC */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(E, 3),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "ts0"),		/* DVLD */
		  SUNXI_FUNCTION(0x3, "csi0"),		/* VSYNC */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(E, 4),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "ts0"),		/* D0 */
		  SUNXI_FUNCTION(0x3, "csi0"),		/* D0 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(E, 5),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "ts0"),		/* D1 */
		  SUNXI_FUNCTION(0x3, "csi0"),		/* D1 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(E, 6),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "ts0"),		/* D2 */
		  SUNXI_FUNCTION(0x3, "csi0"),		/* D2 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(E, 7),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "ts0"),		/* D3 */
		  SUNXI_FUNCTION(0x3, "csi0"),		/* D3 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(E, 8),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "ts0"),		/* D4 */
		  SUNXI_FUNCTION(0x3, "csi0"),		/* D4 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(E, 9),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "ts0"),		/* D5 */
		  SUNXI_FUNCTION(0x3, "csi0"),		/* D5 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(E, 10),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "ts0"),		/* D6 */
		  SUNXI_FUNCTION(0x3, "csi0"),		/* D6 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(E, 11),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "ts0"),		/* D7 */
		  SUNXI_FUNCTION(0x3, "csi0"),		/* D7 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),

	/* Hole */
	SUNXI_PIN(SUNXI_PINCTRL_PIN(F, 0),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "sdc0"),		/* D1 */
		  SUNXI_FUNCTION(0x4, "jtag0"),		/* MSI */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(F, 1),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "sdc0"),		/* D0 */
		  SUNXI_FUNCTION(0x4, "jtag0"),		/* DI1 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(F, 2),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "sdc0"),		/* CLK */
		  SUNXI_FUNCTION(0x4, "uart0"),		/* TX */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(F, 3),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "sdc0"),		/* CMD */
		  SUNXI_FUNCTION(0x4, "jtag0"),		/* DO1 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(F, 4),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "sdc0"),		/* D3 */
		  SUNXI_FUNCTION(0x4, "uart0"),		/* RX */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(F, 5),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "sdc0"),		/* D2 */
		  SUNXI_FUNCTION(0x4, "jtag0"),		/* CK1 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),

	/* Hole */
	SUNXI_PIN(SUNXI_PINCTRL_PIN(G, 0),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "ts1"),		/* CLK */
		  SUNXI_FUNCTION(0x3, "csi1"),		/* PCLK */
		  SUNXI_FUNCTION(0x4, "sdc1"),		/* CMD */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(G, 1),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "ts1"),		/* ERR */
		  SUNXI_FUNCTION(0x3, "csi1"),		/* MLCK */
		  SUNXI_FUNCTION(0x4, "sdc1"),		/* CLK */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(G, 2),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "ts1"),		/* SYNC */
		  SUNXI_FUNCTION(0x3, "csi1"),		/* HSYNC */
		  SUNXI_FUNCTION(0x4, "sdc1"),		/* D0 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(G, 3),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "ts1"),		/* DVLD */
		  SUNXI_FUNCTION(0x3, "csi1"),		/* VSYNC */
		  SUNXI_FUNCTION(0x4, "sdc1"),		/* D1 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(G, 4),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "ts1"),		/* D0 */
		  SUNXI_FUNCTION(0x3, "csi1"),		/* D0 */
		  SUNXI_FUNCTION(0x4, "sdc1"),		/* D2 */
		  SUNXI_FUNCTION(0x5, "csi0"),		/* D8 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(G, 5),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "ts1"),		/* D1 */
		  SUNXI_FUNCTION(0x3, "csi1"),		/* D1 */
		  SUNXI_FUNCTION(0x4, "sdc1"),		/* D3 */
		  SUNXI_FUNCTION(0x5, "csi0"),		/* D9 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(G, 6),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "ts1"),		/* D2 */
		  SUNXI_FUNCTION(0x3, "csi1"),		/* D2 */
		  SUNXI_FUNCTION(0x4, "uart3"),		/* TX */
		  SUNXI_FUNCTION(0x5, "csi0"),		/* D10 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(G, 7),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "ts1"),		/* D3 */
		  SUNXI_FUNCTION(0x3, "csi1"),		/* D3 */
		  SUNXI_FUNCTION(0x4, "uart3"),		/* RX */
		  SUNXI_FUNCTION(0x5, "csi0"),		/* D11 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(G, 8),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "ts1"),		/* D4 */
		  SUNXI_FUNCTION(0x3, "csi1"),		/* D4 */
		  SUNXI_FUNCTION(0x4, "uart3"),		/* RTS */
		  SUNXI_FUNCTION(0x5, "csi0"),		/* D12 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(G, 9),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "ts1"),		/* D5 */
		  SUNXI_FUNCTION(0x3, "csi1"),		/* D5 */
		  SUNXI_FUNCTION(0x4, "uart3"),		/* CTS */
		  SUNXI_FUNCTION(0x5, "csi0"),		/* D13 */
		  SUNXI_FUNCTION(0x6, "bist"),		/* result0 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(G, 10),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "ts1"),		/* D6 */
		  SUNXI_FUNCTION(0x3, "csi1"),		/* D6 */
		  SUNXI_FUNCTION(0x4, "uart4"),		/* TX */
		  SUNXI_FUNCTION(0x5, "csi0"),		/* D14 */
		  SUNXI_FUNCTION(0x6, "bist"),		/* result1 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(G, 11),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "ts1"),		/* D7 */
		  SUNXI_FUNCTION(0x3, "csi1"),		/* D7 */
		  SUNXI_FUNCTION(0x4, "uart4"),		/* RX */
		  SUNXI_FUNCTION(0x5, "csi0"),		/* D15 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),

	/* Hole */
	SUNXI_PIN(SUNXI_PINCTRL_PIN(H, 0),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd1"),		/* D0 */
		  SUNXI_FUNCTION(0x3, "io_disabled"),
		  SUNXI_FUNCTION(0x4, "uart3"),		/* TX */
		  SUNXI_FUNCTION(0x7, "csi1"),		/* D0 */
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 0)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(H, 1),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd1"),		/* D1 */
		  SUNXI_FUNCTION(0x3, "io_disabled"),
		  SUNXI_FUNCTION(0x4, "uart3"),		/* RX */
		  SUNXI_FUNCTION(0x7, "csi1"),		/* D1 */
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 1)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(H, 2),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd1"),		/* D2 */
		  SUNXI_FUNCTION(0x3, "io_disabled"),
		  SUNXI_FUNCTION(0x4, "uart3"),		/* RTS */
		  SUNXI_FUNCTION(0x7, "csi1"),		/* D2 */
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 2)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(H, 3),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd1"),		/* D3 */
		  SUNXI_FUNCTION(0x3, "io_disabled"),
		  SUNXI_FUNCTION(0x4, "uart3"),		/* CTS */
		  SUNXI_FUNCTION(0x7, "csi1"),		/* D3 */
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 3)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(H, 4),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd1"),		/* D4 */
		  SUNXI_FUNCTION(0x3, "io_disabled"),
		  SUNXI_FUNCTION(0x4, "uart4"),		/* TX */
		  SUNXI_FUNCTION(0x7, "csi1"),		/* D4 */
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 4)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(H, 5),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd1"),		/* D5 */
		  SUNXI_FUNCTION(0x3, "io_disabled"),
		  SUNXI_FUNCTION(0x4, "uart4"),		/* RX */
		  SUNXI_FUNCTION(0x7, "csi1"),		/* D5 */
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 5)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(H, 6),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd1"),		/* D6 */
		  SUNXI_FUNCTION(0x3, "io_disabled"),
		  SUNXI_FUNCTION(0x4, "uart5"),		/* TX */
		  SUNXI_FUNCTION(0x7, "csi1"),		/* D6 */
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 6)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(H, 7),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd1"),		/* D7 */
		  SUNXI_FUNCTION(0x3, "io_disabled"),
		  SUNXI_FUNCTION(0x4, "uart5"),		/* RX */
		  SUNXI_FUNCTION(0x7, "csi1"),		/* D7 */
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 7)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(H, 8),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd1"),		/* D8 */
		  SUNXI_FUNCTION(0x3, "emac0"),		/* ERXD3 */
		  SUNXI_FUNCTION(0x4, "kp0"),		/* IN0 */
		  SUNXI_FUNCTION(0x7, "csi1"),		/* D8 */
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 8)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(H, 9),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd1"),		/* D9 */
		  SUNXI_FUNCTION(0x3, "emac0"),		/* ERXD2 */
		  SUNXI_FUNCTION(0x4, "kp0"),		/* IN1 */
		  SUNXI_FUNCTION(0x7, "csi1"),		/* D9 */
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 9)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(H, 10),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd1"),		/* D10 */
		  SUNXI_FUNCTION(0x3, "emac0"),		/* ERXD1 */
		  SUNXI_FUNCTION(0x4, "kp0"),		/* IN2 */
		  SUNXI_FUNCTION(0x7, "csi1"),		/* D10 */
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 10)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(H, 11),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd1"),		/* D11 */
		  SUNXI_FUNCTION(0x3, "emac0"),		/* ERXD0 */
		  SUNXI_FUNCTION(0x4, "kp0"),		/* IN3 */
		  SUNXI_FUNCTION(0x7, "csi1"),		/* D11 */
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 11)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(H, 12),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd1"),		/* D12 */
		  SUNXI_FUNCTION(0x3, "io_disabled"),
		  SUNXI_FUNCTION(0x4, "ps21"),		/* SCK1 */
		  SUNXI_FUNCTION(0x7, "csi1"),		/* D12 */
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 12)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(H, 13),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd1"),		/* D13 */
		  SUNXI_FUNCTION(0x3, "io_disabled"),
		  SUNXI_FUNCTION(0x4, "ps21"),		/* SDA1 */
		  SUNXI_FUNCTION(0x5, "scr0"),		/* RST */
		  SUNXI_FUNCTION(0x7, "csi1"),		/* D13 */
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 13)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(H, 14),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd1"),		/* D14 */
		  SUNXI_FUNCTION(0x3, "emac0"),		/* ETXD3 */
		  SUNXI_FUNCTION(0x4, "kp0"),		/* IN4 */
		  SUNXI_FUNCTION(0x5, "scr0"),		/* VPPEN */
		  SUNXI_FUNCTION(0x7, "csi1"),		/* D14 */
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 14)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(H, 15),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd1"),		/* D15 */
		  SUNXI_FUNCTION(0x3, "emac0"),		/* ETXD2 */
		  SUNXI_FUNCTION(0x4, "kp0"),		/* IN5 */
		  SUNXI_FUNCTION(0x5, "scr0"),		/* VPPPP */
		  SUNXI_FUNCTION(0x7, "csi1"),		/* D15 */
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 15)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(H, 16),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd1"),		/* D16 */
		  SUNXI_FUNCTION(0x3, "emac0"),		/* ETXD1 */
		  SUNXI_FUNCTION(0x4, "kp0"),		/* IN6 */
		  SUNXI_FUNCTION(0x5, "scr0"),		/* DETECT */
		  SUNXI_FUNCTION(0x7, "csi1"),		/* D16 */
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 16)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(H, 17),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd1"),		/* D17 */
		  SUNXI_FUNCTION(0x3, "emac0"),		/* ETXD0 */
		  SUNXI_FUNCTION(0x4, "kp0"),		/* IN7 */
		  SUNXI_FUNCTION(0x5, "scr0"),		/* VCCEN */
		  SUNXI_FUNCTION(0x7, "csi1"),		/* D17 */
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 17)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(H, 18),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd1"),		/* D18 */
		  SUNXI_FUNCTION(0x3, "emac0"),		/* ERXCK */
		  SUNXI_FUNCTION(0x4, "kp0"),		/* OUT0 */
		  SUNXI_FUNCTION(0x5, "scr0"),		/* SLK */
		  SUNXI_FUNCTION(0x7, "csi1"),		/* D18 */
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 18)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(H, 19),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd1"),		/* D19 */
		  SUNXI_FUNCTION(0x3, "emac0"),		/* ERXERR */
		  SUNXI_FUNCTION(0x4, "kp0"),		/* OUT1 */
		  SUNXI_FUNCTION(0x5, "scr0"),		/* SDA */
		  SUNXI_FUNCTION(0x7, "csi1"),		/* D19 */
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 19)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(H, 20),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd1"),		/* D20 */
		  SUNXI_FUNCTION(0x3, "emac0"),		/* ERXDV */
		  SUNXI_FUNCTION(0x4, "can0"),		/* TX */
		  SUNXI_FUNCTION(0x5, "io_disabled"),
		  SUNXI_FUNCTION(0x7, "csi1"),		/* D20 */
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 20)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(H, 21),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd1"),		/* D21 */
		  SUNXI_FUNCTION(0x3, "emac0"),		/* EMDC */
		  SUNXI_FUNCTION(0x4, "can1"),		/* RX */
		  SUNXI_FUNCTION(0x5, "io_disabled"),
		  SUNXI_FUNCTION(0x7, "csi1"),		/* D21 */
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 21)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(H, 22),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd1"),		/* D22 */
		  SUNXI_FUNCTION(0x3, "emac0"),		/* EMDIO */
		  SUNXI_FUNCTION(0x4, "kp0"),		/* OUT2 */
		  SUNXI_FUNCTION(0x5, "sdc1"),		/* CMD */
		  SUNXI_FUNCTION(0x6, "io_disabled"),
		  SUNXI_FUNCTION(0x7, "csi1")),		/* D22 */
	SUNXI_PIN(SUNXI_PINCTRL_PIN(H, 23),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd1"),		/* D23 */
		  SUNXI_FUNCTION(0x3, "emac0"),		/* ETXEN */
		  SUNXI_FUNCTION(0x4, "kp0"),		/* OUT3 */
		  SUNXI_FUNCTION(0x5, "sdc1"),		/* CLK */
		  SUNXI_FUNCTION(0x6, "io_disabled"),
		  SUNXI_FUNCTION(0x7, "csi1")),		/* D23 */
	SUNXI_PIN(SUNXI_PINCTRL_PIN(H, 24),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd1"),		/* CLK */
		  SUNXI_FUNCTION(0x3, "emac0"),		/* ETXCK */
		  SUNXI_FUNCTION(0x4, "kp0"),		/* OUT4 */
		  SUNXI_FUNCTION(0x5, "sdc1"),		/* D0 */
		  SUNXI_FUNCTION(0x6, "io_disabled"),
		  SUNXI_FUNCTION(0x7, "csi1")),		/* PCLK */
	SUNXI_PIN(SUNXI_PINCTRL_PIN(H, 25),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd1"),		/* DE */
		  SUNXI_FUNCTION(0x3, "emac0"),		/* ECRS */
		  SUNXI_FUNCTION(0x4, "kp0"),		/* OUT5 */
		  SUNXI_FUNCTION(0x5, "sdc1"),		/* D1 */
		  SUNXI_FUNCTION(0x6, "io_disabled"),
		  SUNXI_FUNCTION(0x7, "csi1")),		/* FIELD */
	SUNXI_PIN(SUNXI_PINCTRL_PIN(H, 26),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd1"),		/* HSYNC */
		  SUNXI_FUNCTION(0x3, "emac0"),		/* ECOL */
		  SUNXI_FUNCTION(0x4, "kp0"),		/* OUT6 */
		  SUNXI_FUNCTION(0x5, "sdc1"),		/* D2 */
		  SUNXI_FUNCTION(0x6, "io_disabled"),
		  SUNXI_FUNCTION(0x7, "csi1")),		/* HSYNC */
	SUNXI_PIN(SUNXI_PINCTRL_PIN(H, 27),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "lcd1"),		/* VSYNC */
		  SUNXI_FUNCTION(0x3, "emac0"),		/* ETXERR */
		  SUNXI_FUNCTION(0x4, "kp0"),		/* OUT7 */
		  SUNXI_FUNCTION(0x5, "sdc1"),		/* D3 */
		  SUNXI_FUNCTION(0x6, "io_disabled"),
		  SUNXI_FUNCTION(0x7, "csi1")),		/* VSYNC */

	/* Hole */
	SUNXI_PIN(SUNXI_PINCTRL_PIN(I, 0),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x3, "twi3"),		/* SCK */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(I, 1),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x3, "twi3"),		/* SDA */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(I, 2),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x3, "twi4"),		/* SCK */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(I, 3),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "pwm1"),		/* PWM1 */
		  SUNXI_FUNCTION(0x3, "twi4"),		/* SDA */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(I, 4),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "sdc3"),		/* CMD */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(I, 5),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "sdc3"),		/* CLK */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(I, 6),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "sdc3"),		/* D0 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(I, 7),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "sdc3"),		/* D1 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(I, 8),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "sdc3"),		/* D2 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(I, 9),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "sdc3"),		/* D3 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(I, 10),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "spi0"),		/* CS0 */
		  SUNXI_FUNCTION(0x3, "uart5"),		/* TX */
		  SUNXI_FUNCTION(0x7, "io_disabled"),
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 22)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(I, 11),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "spi0"),		/* CLK */
		  SUNXI_FUNCTION(0x3, "uart5"),		/* RX */
		  SUNXI_FUNCTION(0x7, "io_disabled"),
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 23)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(I, 12),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "spi0"),		/* MOSI */
		  SUNXI_FUNCTION(0x3, "uart6"),		/* TX */
		  SUNXI_FUNCTION(0x4, "clka"),		/* CLK_OUT_A */
		  SUNXI_FUNCTION(0x7, "io_disabled"),
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 24)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(I, 13),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "spi0"),		/* MISO */
		  SUNXI_FUNCTION(0x3, "uart6"),		/* RX */
		  SUNXI_FUNCTION(0x4, "clkb"),		/* CLK_OUT_B */
		  SUNXI_FUNCTION(0x7, "io_disabled"),
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 25)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(I, 14),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "spi0"),		/* CS1 */
		  SUNXI_FUNCTION(0x3, "ps21"),		/* SCK1 */
		  SUNXI_FUNCTION(0x4, "tclk"),		/* TCLKIN0 */
		  SUNXI_FUNCTION(0x7, "io_disabled"),
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 26)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(I, 15),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "spi1"),		/* CS1 */
		  SUNXI_FUNCTION(0x3, "ps21"),		/* SDA1 */
		  SUNXI_FUNCTION(0x4, "tclk"),		/* TCLKIN1 */
		  SUNXI_FUNCTION(0x7, "io_disabled"),
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 27)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(I, 16),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "spi1"),		/* CS0 */
		  SUNXI_FUNCTION(0x3, "uart2"),		/* RTS */
		  SUNXI_FUNCTION(0x7, "io_disabled"),
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 28)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(I, 17),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "spi1"),		/* CLK */
		  SUNXI_FUNCTION(0x3, "uart2"),		/* CTS */
		  SUNXI_FUNCTION(0x7, "io_disabled"),
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 29)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(I, 18),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "spi1"),		/* MOSI */
		  SUNXI_FUNCTION(0x3, "uart2"),		/* TX */
		  SUNXI_FUNCTION(0x7, "io_disabled"),
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 30)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(I, 19),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "spi1"),		/* MISO */
		  SUNXI_FUNCTION(0x3, "uart2"),		/* RX */
		  SUNXI_FUNCTION(0x7, "io_disabled"),
		  SUNXI_FUNCTION_IRQ_BANK(0x6, 0, 31)),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(I, 20),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "ps20"),		/* SCK0 */
		  SUNXI_FUNCTION(0x3, "uart7"),		/* TX */
		  SUNXI_FUNCTION(0x4, "vdevice"),	/* vdevice */
		  SUNXI_FUNCTION(0x6, "pwm2"),		/* PWM2 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
	SUNXI_PIN(SUNXI_PINCTRL_PIN(I, 21),
		  SUNXI_FUNCTION(0x0, "gpio_in"),
		  SUNXI_FUNCTION(0x1, "gpio_out"),
		  SUNXI_FUNCTION(0x2, "ps20"),		/* SDA0 */
		  SUNXI_FUNCTION(0x3, "uart7"),		/* RX */
		  SUNXI_FUNCTION(0x4, "vdevice"),	/* vdevice */
		  SUNXI_FUNCTION(0x6, "pwm3"),		/* PWM3 */
		  SUNXI_FUNCTION(0x7, "io_disabled")),
};
static const struct sunxi_pinctrl_desc sun8iw11p1_pinctrl_data = {
	.pins = sun8iw11p1_pins,
	.npins = ARRAY_SIZE(sun8iw11p1_pins),
	.pin_base = 0,
	.irq_banks = 1,
};

static int sun8iw11p1_pinctrl_probe(struct platform_device *pdev)
{
	return sunxi_pinctrl_init(pdev,
				  &sun8iw11p1_pinctrl_data);
}

static struct of_device_id sun8iw11p1_pinctrl_match[] = {
	{ .compatible = "allwinner,sun8iw11p1-pinctrl", },
	{}
};
MODULE_DEVICE_TABLE(of, sun8iw11p1_pinctrl_match);

static struct platform_driver sun8iw11p1_pinctrl_driver = {
	.probe	= sun8iw11p1_pinctrl_probe,
	.driver	= {
		.name		= "sun8iw11p1-pinctrl",
		.owner		= THIS_MODULE,
		.of_match_table	= sun8iw11p1_pinctrl_match,
	},
};
static int __init sun8iw11p1_pio_init(void)
{
	int ret;
	ret = platform_driver_register(&sun8iw11p1_pinctrl_driver);
	if (IS_ERR_VALUE(ret)) {
		pr_debug("register sun8iw11p1 pio controller failed\n");
		return -EINVAL;
	}
	return 0;
}
postcore_initcall(sun8iw11p1_pio_init);

MODULE_AUTHOR("Jackie Hwang <huangshr@allwinnertech.com>");
MODULE_AUTHOR("Chen-Yu Tsai <wens@csie.org>");
MODULE_AUTHOR("Boris Brezillon <boris.brezillon@free-electrons.com");
MODULE_AUTHOR("Maxime Ripard <maxime.ripard@free-electrons.com");
MODULE_DESCRIPTION("Allwinner sun8iw11p1 PIO pinctrl driver");
MODULE_LICENSE("GPL");
