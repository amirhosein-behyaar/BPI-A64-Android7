/*
 * drivers/char/sunxi-scr/sunxi-scr.c
 *
 * Copyright (C) 2016 Allwinner.
 * fuzhaoke <fuzhaoke@allwinnertech.com>
 *
 * SUNXI SCR Controller Driver
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <linux/err.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/major.h>
#include <linux/poll.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/pinctrl/consumer.h>
#include <linux/platform_device.h>
#include <linux/sys_config.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <asm/irq.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/poll.h>
#include <linux/sched.h>
#include "sunxi-scr.h"

/* ====================  For debug  =============================== */
#define SCR_ENTER()		pr_info("%s()%d - %s\n", __func__, __LINE__, "Enter ...")
#define SCR_EXIT()		pr_info("%s()%d - %s\n", __func__, __LINE__, "Exit")
#define SCR_DBG(fmt, arg...)	pr_debug("%s()%d - "fmt, __func__, __LINE__, ##arg)
#define SCR_INFO(fmt, arg...)	pr_info("%s()%d - "fmt, __func__, __LINE__, ##arg)
#define SCR_WARN(fmt, arg...)	pr_warn("%s()%d - "fmt, __func__, __LINE__, ##arg)
#define SCR_ERR(fmt, arg...)	pr_err("%s()%d - "fmt, __func__, __LINE__, ##arg)

static struct sunxi_scr *pscr;
static int sunxi_scr_major;
static struct class *scr_dev_class;
static struct device *scr_device;

struct scr_data {
	uint8_t buf[SCR_BUF_SIZE];
	uint16_t cnt;  /* valid count of data */
};

static struct scr_data scr_buf_rx, scr_buf_tx;


static void sunxi_scr_do_atr(struct sunxi_scr *pscr);


/* =================  smart card reader basic interface  =================== */
/* clear control and status register */
static inline void scr_clear_csr_reg(void __iomem *reg_base)
{
	writel(0x0, reg_base + SCR_CSR_OFF);
}

/* get control and status register config */
static inline uint32_t scr_get_csr_config(void __iomem *reg_base)
{
	return readl(reg_base + SCR_CSR_OFF);
}

/* get detect status 0:card remove, 1:card insert */
static inline uint8_t scr_get_det_status(void __iomem *reg_base)
{
	uint32_t reg_val;

	reg_val = readl(reg_base + SCR_CSR_OFF);

	return (reg_val >> 31) & 0x1;
}

/* set detect polarity, 0:low active, 1:high active */
static inline void scr_set_det_polar(void __iomem *reg_base, uint8_t config)
{
	uint32_t reg_val;

	reg_val = readl(reg_base + SCR_CSR_OFF);
	reg_val &= ~(0x1 << 24);
	reg_val |= ((config & 0x1) << 24);
	writel(reg_val, reg_base + SCR_CSR_OFF);
}

/* set protocol 0:T0, 1:T1, 2~3:reserved */
static inline void scr_set_t_protocol(void __iomem *reg_base, uint8_t config)
{
	uint32_t reg_val;

	reg_val = readl(reg_base + SCR_CSR_OFF);
	reg_val &= ~(0x3 << 22);
	reg_val |= ((config & 0x3) << 22);
	writel(reg_val, reg_base + SCR_CSR_OFF);
}

/* when enable(1), both RX&TX FIFO will be flush before ATR start */
static inline void scr_set_atr_flush(void __iomem *reg_base, uint8_t config)
{
	uint32_t reg_val;

	reg_val = readl(reg_base + SCR_CSR_OFF);
	reg_val &= ~(0x1 << 21);
	reg_val |= ((config & 0x1) << 21);
	writel(reg_val, reg_base + SCR_CSR_OFF);
}

/* when enable(1), TS charater(first ATR character) will be stored in FIFO */
static inline void scr_set_ts_recv(void __iomem *reg_base, uint8_t config)
{
	uint32_t reg_val;

	reg_val = readl(reg_base + SCR_CSR_OFF);
	reg_val &= ~(0x1 << 20);
	reg_val |= ((config & 0x1) << 20);
	writel(reg_val, reg_base + SCR_CSR_OFF);
}

/* scclk output state during clock stop, 0:low level, 1:high level */
static inline void scr_set_clk_polar(void __iomem *reg_base, uint8_t config)
{
	uint32_t reg_val;

	reg_val = readl(reg_base + SCR_CSR_OFF);
	reg_val &= ~(0x1 << 19);
	reg_val |= ((config & 0x1) << 19);
	writel(reg_val, reg_base + SCR_CSR_OFF);
}

/* parity error character receive enable,
 * 0:disable, 1:enable store error parity in RX FIFO
 */
static inline void scr_set_recv_parity(void __iomem *reg_base, uint8_t config)
{
	uint32_t reg_val;

	reg_val = readl(reg_base + SCR_CSR_OFF);
	reg_val &= ~(0x1 << 18);
	reg_val |= ((config & 0x1) << 18);
	writel(reg_val, reg_base + SCR_CSR_OFF);
}

static inline uint8_t scr_get_recv_parity(void __iomem *reg_base)
{
	uint32_t reg_val;

	reg_val = readl(reg_base + SCR_CSR_OFF);

	return (reg_val >> 18) & 0x1;
}

/* 0:lsb first, 1:msb first */
static inline void scr_set_data_order(void __iomem *reg_base, uint8_t config)
{
	uint32_t reg_val;

	reg_val = readl(reg_base + SCR_CSR_OFF);
	reg_val &= ~(0x1 << 17);
	reg_val |= ((config & 0x1) << 17);
	writel(reg_val, reg_base + SCR_CSR_OFF);
}

/* when enable(1), invert data level */
static inline void scr_set_data_invert(void __iomem *reg_base, uint8_t config)
{
	uint32_t reg_val;

	reg_val = readl(reg_base + SCR_CSR_OFF);
	reg_val &= ~(0x1 << 16);
	reg_val |= ((config & 0x1) << 16);
	writel(reg_val, reg_base + SCR_CSR_OFF);
}

/* deinit card after stop use and auto clear when finish */
static inline void scr_set_deactivation(void __iomem *reg_base)
{
	uint32_t reg_val;

	reg_val = readl(reg_base + SCR_CSR_OFF);
	reg_val |= (0x1 << 11);
	writel(reg_val, reg_base + SCR_CSR_OFF);
}

/* init card before start use and auto clear when finish */
static inline void scr_set_activation(void __iomem *reg_base)
{
	uint32_t reg_val;

	reg_val = readl(reg_base + SCR_CSR_OFF);
	reg_val |= (0x1 << 10);
	writel(reg_val, reg_base + SCR_CSR_OFF);
}

/* reset card */
static inline void scr_set_warmreset(void __iomem *reg_base)
{
	uint32_t reg_val;

	reg_val = readl(reg_base + SCR_CSR_OFF);
	reg_val |= (0x1 << 9);
	writel(reg_val, reg_base + SCR_CSR_OFF);
}

static inline void scr_set_clk_stop(void __iomem *reg_base)
{
	uint32_t reg_val;

	reg_val = readl(reg_base + SCR_CSR_OFF);
	reg_val |= (0x1 << 8);
	writel(reg_val, reg_base + SCR_CSR_OFF);
}

static inline void scr_set_clk_restart(void __iomem *reg_base)
{
	uint32_t reg_val;

	reg_val = readl(reg_base + SCR_CSR_OFF);
	reg_val &= ~(0x1 << 8);
	writel(reg_val, reg_base + SCR_CSR_OFF);
}

static inline void scr_global_interrupt_enable(void __iomem *reg_base)
{
	uint32_t reg_val;

	reg_val = readl(reg_base + SCR_CSR_OFF);
	reg_val |= (0x1 << 2);
	writel(reg_val, reg_base + SCR_CSR_OFF);
}

static inline void scr_global_interrupt_disable(void __iomem *reg_base)
{
	uint32_t reg_val;

	reg_val = readl(reg_base + SCR_CSR_OFF);
	reg_val &= ~(0x1 << 2);
	writel(reg_val, reg_base + SCR_CSR_OFF);
}

/* enable receive data stored in RX FIFO */
static inline void scr_receive_enable(void __iomem *reg_base)
{
	uint32_t reg_val;

	reg_val = readl(reg_base + SCR_CSR_OFF);
	reg_val |= (0x1 << 1);
	writel(reg_val, reg_base + SCR_CSR_OFF);
}

/* disable receive data stored in RX FIFO */
static inline void scr_receive_disable(void __iomem *reg_base)
{
	uint32_t reg_val;

	reg_val = readl(reg_base + SCR_CSR_OFF);
	reg_val &= ~(0x1 << 1);
	writel(reg_val, reg_base + SCR_CSR_OFF);
}

/* enable transmit data stored in TX FIFO */
static inline void scr_transmit_enable(void __iomem *reg_base)
{
	uint32_t reg_val;

	reg_val = readl(reg_base + SCR_CSR_OFF);
	reg_val |= (0x1 << 0);
	writel(reg_val, reg_base + SCR_CSR_OFF);
}

/* disable transmit data stored in TX FIFO */
static inline void scr_transmit_disable(void __iomem *reg_base)
{
	uint32_t reg_val;

	reg_val = readl(reg_base + SCR_CSR_OFF);
	reg_val &= ~(0x1 << 0);
	writel(reg_val, reg_base + SCR_CSR_OFF);
}

/* set each interrupt bit mask */
static inline void scr_set_interrupt_enable(void __iomem *reg_base, uint32_t bm)
{
	uint32_t reg_val;

	reg_val = readl(reg_base + SCR_INTEN_OFF);
	reg_val |= bm;
	writel(reg_val, reg_base + SCR_INTEN_OFF);
}

/* disable interrupt bit mask */
static inline void scr_set_interrupt_disable(void __iomem *reg_base, uint32_t bm)
{
	uint32_t reg_val;

	reg_val = readl(reg_base + SCR_INTEN_OFF);
	reg_val &= ~bm;
	writel(reg_val, reg_base + SCR_INTEN_OFF);
}

/* get all interrupt status */
static inline uint32_t scr_get_interrupt_status(void __iomem *reg_base)
{
	return readl(reg_base + SCR_INTST_OFF);
}

/* write 1 to clear interrupt flag */
static inline void scr_clear_interrupt_status(void __iomem *reg_base, uint32_t bm)
{
	writel(bm, reg_base + SCR_INTST_OFF);
}

/* flush RX FIFO and auto clear when finish */
static inline void scr_flush_rxfifo(void __iomem *reg_base)
{
	uint32_t reg_val;

	reg_val = readl(reg_base + SCR_FCSR_OFF);
	reg_val |= (0x1 << 10);
	writel(reg_val, reg_base + SCR_FCSR_OFF);
}

static inline bool scr_rxfifo_is_full(void __iomem *reg_base)
{
	return (readl(reg_base + SCR_FCSR_OFF) >> 9) & 0x1;
}

static inline bool scr_rxfifo_is_empty(void __iomem *reg_base)
{
	return (readl(reg_base + SCR_FCSR_OFF) >> 8) & 0x1;
}

/* flush TX FIFO and auto clear when finish */
static inline void scr_flush_txfifo(void __iomem *reg_base)
{
	uint32_t reg_val;

	reg_val = readl(reg_base + SCR_FCSR_OFF);
	reg_val |= (0x1 << 2);
	writel(reg_val, reg_base + SCR_FCSR_OFF);
}

static inline bool scr_txfifo_is_full(void __iomem *reg_base)
{
	return (readl(reg_base + SCR_FCSR_OFF) >> 1) & 0x1;
}

static inline bool scr_txfifo_is_empty(void __iomem *reg_base)
{
	return readl(reg_base + SCR_FCSR_OFF) & 0x1;
}

static inline void scr_set_rxfifo_threshold(void __iomem *reg_base, uint8_t thh)
{
	uint32_t reg_val;
	reg_val = readl(reg_base + SCR_FCNT_OFF);
	reg_val &= ~(0xffU << 24);
	reg_val |= thh << 24;
	writel(reg_val, reg_base + SCR_FCNT_OFF);
}

static inline void scr_set_txfifo_threshold(void __iomem *reg_base, uint8_t thh)
{
	uint32_t reg_val;

	reg_val = readl(reg_base + SCR_FCNT_OFF);
	reg_val &= ~(0xffU << 16);
	reg_val |= thh << 16;
	writel(reg_val, reg_base + SCR_FCNT_OFF);
}

static inline uint8_t scr_get_rxfifo_count(void __iomem *reg_base)
{
	return (readl(reg_base + SCR_FCNT_OFF) >> 8) & 0xff;
}

static inline uint8_t scr_get_txfifo_count(void __iomem *reg_base)
{
	return readl(reg_base + SCR_FCNT_OFF) & 0xff;
}

static inline void scr_set_rx_repeat(void __iomem *reg_base, uint8_t repeat)
{
	uint32_t reg_val;

	reg_val = readl(reg_base + SCR_RPT_OFF);
	reg_val &= ~(0xf << 4);
	reg_val |= (repeat & 0xf) << 4;
	writel(reg_val, reg_base + SCR_RPT_OFF);
}

static inline void scr_set_tx_repeat(void __iomem *reg_base, uint8_t repeat)
{
	uint32_t reg_val;

	reg_val = readl(reg_base + SCR_RPT_OFF);
	reg_val &= ~(0xf << 0);
	reg_val |= (repeat & 0xf) << 0;
	writel(reg_val, reg_base + SCR_RPT_OFF);
}

/* baud = F_sysclk/(2*(BAUDDIV+1)), BAUDDIV=bit[31:16] */
static inline void scr_set_baud_divisor(void __iomem *reg_base, uint16_t divisor)
{
	uint32_t reg_val;

	reg_val = readl(reg_base + SCR_DIV_OFF);
	reg_val &= ~(0xffffU << 16);
	reg_val |= divisor << 16;
	writel(reg_val, reg_base + SCR_DIV_OFF);
}

static inline uint16_t scr_get_baud_divisor(void __iomem *reg_base)
{
	return readl(reg_base + SCR_DIV_OFF) >> 16 & 0xffff;
}

/* F_scclk = F_sysclk/(2*(SCCLK+1)), SCCLK=bit[15:0] */
static inline void scr_set_scclk_divisor(void __iomem *reg_base, uint16_t divisor)
{
	uint32_t reg_val;

	reg_val = readl(reg_base + SCR_DIV_OFF);
	reg_val &= ~0xffffU;
	reg_val |= divisor;
	writel(reg_val, reg_base + SCR_DIV_OFF);
}

static inline uint16_t scr_get_scclk_divisor(void __iomem *reg_base)
{
	return readl(reg_base + SCR_DIV_OFF) & 0xffff;
}

/* ATR start time limit, it define the maximum time of ATR response
 * limit_time = 128*ATR*T_scclk, ATR=bit[23:16], T_scclk=1/F_scclk
 */
static inline void scr_set_atr_time(void __iomem *reg_base, uint8_t scclk)
{
	uint32_t reg_val;

	reg_val = readl(reg_base + SCR_LTIM_OFF);
	reg_val &= ~(0xff << 16);
	reg_val |= scclk << 16;
	writel(reg_val, reg_base + SCR_LTIM_OFF);
}

/* reset duration, dura = 128*RST*T_scclk, RST=bit[15:8], T_scclk=1/F_scclk */
static inline void scr_set_reset_time(void __iomem *reg_base, uint8_t scclk)
{
	uint32_t reg_val;

	reg_val = readl(reg_base + SCR_LTIM_OFF);
	reg_val &= ~(0xff << 8);
	reg_val |= scclk << 8;
	writel(reg_val, reg_base + SCR_LTIM_OFF);
}

/* reset duration, dura = 128*ACT*T_scclk, ACT=bit[7:0], T_scclk=1/F_scclk */
static inline void scr_set_activation_time(void __iomem *reg_base, uint8_t scclk)
{
	uint32_t reg_val;

	reg_val = readl(reg_base + SCR_LTIM_OFF);
	reg_val &= ~0xff;
	reg_val |= scclk;
	writel(reg_val, reg_base + SCR_LTIM_OFF);
}

static inline uint32_t scr_get_line_time(void __iomem *reg_base)
{
	return readl(reg_base + SCR_LTIM_OFF);
}

/* character limit, maximum time of two consecutive character, ETU as unit */
static inline void scr_set_chlimit_time(void __iomem *reg_base, uint16_t etu)
{
	uint32_t reg_val;

	reg_val = readl(reg_base + SCR_CTIM_OFF);
	reg_val &= ~(0xffffU << 16);
	reg_val |= etu << 16;
	writel(reg_val, reg_base + SCR_CTIM_OFF);
}

/* character guard time, delay time of each character, ETU as unit */
static inline void scr_set_guard_time(void __iomem *reg_base, uint8_t etu)
{
	uint32_t reg_val;

	reg_val = readl(reg_base + SCR_CTIM_OFF);
	reg_val &= ~0xff;
	reg_val |= etu;
	writel(reg_val, reg_base + SCR_CTIM_OFF);
}

static inline uint32_t scr_get_character_time(void __iomem *reg_base)
{
	return readl(reg_base + SCR_CTIM_OFF);
}

static inline uint32_t scr_get_fsm(void __iomem *reg_base)
{
	return readl(reg_base + SCR_FSM_OFF);
}

static inline void scr_write_fifo(void __iomem *reg_base, uint8_t data)
{
	writel(data, reg_base + SCR_FIFO_OFF);
}

static inline uint8_t scr_read_fifo(void __iomem *reg_base)
{
	return readl(reg_base + SCR_FIFO_OFF) & 0xff;
}

/* =========================  end  =================================== */

/* IRQ interrupt handler */
static irqreturn_t sunxi_scr_interrupt(int irqno, void *dev_id)
{
	struct sunxi_scr *pscr = (struct sunxi_scr *)dev_id;
	uint32_t rx_cnt = 0, i = 0;
	u32 irq_status = 0;

	irq_status = scr_get_interrupt_status(pscr->reg_base);
	scr_clear_interrupt_status(pscr->reg_base, irq_status);

	SCR_DBG("irq_status = 0x%08x\n", irq_status);
	if (irq_status & SCR_INTSTA_INS) {
		SCR_DBG("SmartCard Inserted!!\n");
		scr_set_activation(pscr->reg_base);
		pscr->card_in = true;
		/* avoid error multi trigger */
		mod_timer(&pscr->poll_timer, jiffies + HZ/100); /* 10ms */
	}

	if (irq_status & SCR_INTSTA_REM) {
		SCR_DBG("SmartCard Removed!!\n\n");
		scr_set_deactivation(pscr->reg_base);
		pscr->card_in = false;
		/* avoid error multi trigger */
		mod_timer(&pscr->poll_timer, jiffies + HZ/100); /* 10ms */
	}

	if (irq_status & SCR_INTSTA_ACT) {
		SCR_DBG("SmartCard Activated!!\n");
		memset(&scr_buf_rx, 0, sizeof(struct scr_data));
		pscr->card_in = true;
	}

	if (irq_status & SCR_INTSTA_DEACT) {
		SCR_DBG("SmartCard Deactivated!!\n");
		pscr->card_in = false;
	}

	if ((irq_status & SCR_INTSTA_RXDONE) ||
	    (irq_status & SCR_INTSTA_RXFTH) ||
	    (irq_status & SCR_INTSTA_RXFFULL)) {
		SCR_DBG("SmartCard Rx interrupt!!\n");
		rx_cnt = scr_get_rxfifo_count(pscr->reg_base);
		SCR_DBG("rx_cnt=%d\n", rx_cnt);
		if (rx_cnt > (SCR_BUF_SIZE - scr_buf_rx.cnt)) {
			SCR_ERR("There are not more space filled in RX buffer");
		} else {
			spin_lock(&pscr->rx_lock);
			for (i = 0; i < rx_cnt; i++) {
				scr_buf_rx.buf[scr_buf_rx.cnt] =
					scr_read_fifo(pscr->reg_base);
				scr_buf_rx.cnt++;
			}
			spin_unlock(&pscr->rx_lock);
		}
	}

	if (irq_status & SCR_INTSTA_RXPERR) {
		SCR_DBG("SmartCard Rx Parity Error!!\n");
	}

	if (irq_status & SCR_INTSTA_ATRFAIL) {
		SCR_DBG("SmartCard ATR Fail!!\n");
		pscr->atr_resp = SCR_ATR_RESP_FAIL;
		/* set activation again */
		scr_set_activation(pscr->reg_base);
	}

	if (irq_status & SCR_INTSTA_ATRDONE) {
		SCR_DBG("SmartCard ATR Done!!\n");
		memcpy(pscr->scr_atr_des.atr_data, scr_buf_rx.buf, scr_buf_rx.cnt);
		pscr->scr_atr_des.atr_len = scr_buf_rx.cnt;
		pscr->atr_resp = SCR_ATR_RESP_OK;
		/* parse ATR data to reconfig smart card */
		sunxi_scr_do_atr(pscr);
	}
	if (irq_status & SCR_INTSTA_CHTO) {
		SCR_DBG("character timeout!!\n");
		pscr->rx_transmit_status = SCR_RX_TRANSMIT_TMOUT;
	}

	if (irq_status & SCR_INTSTA_TXFEMPTY) {
		SCR_DBG("SmartCard TX Empty!!\n");
	}

	if (irq_status & SCR_INTSTA_TXDONE) {
		SCR_DBG("SmartCard TX Done!!\n");
	}

	if (irq_status & SCR_INTSTA_TXPERR) {
		SCR_DBG("SmartCard TX Error!!\n");
	}

	if (irq_status & SCR_INTSTA_TXFDONE) {
		SCR_DBG("SmartCard TX FIFO Done!!\n");
	}

	return IRQ_HANDLED;
}

static int scr_request_gpio(struct sunxi_scr *pscr)
{
	int ret = 0;
	struct pinctrl_state *pctrl_state = NULL;

	pscr->scr_pinctrl = devm_pinctrl_get(&(pscr->scr_device->dev));
	if (IS_ERR_OR_NULL(pscr->scr_pinctrl)) {
		SCR_ERR("request pinctrl handle fail!\n");
		return -EINVAL;
	}

	pctrl_state = pinctrl_lookup_state(pscr->scr_pinctrl, PINCTRL_STATE_DEFAULT);
	if (IS_ERR(pctrl_state)) {
		SCR_ERR("pinctrl_lookup_state fail! return %p\n", pctrl_state);
		return -EINVAL;
	}

	ret = pinctrl_select_state(pscr->scr_pinctrl, pctrl_state);
	if (ret < 0)
		SCR_ERR("pinctrl_select_state fail! return %d\n", ret);

	return ret;
}

static void scr_release_gpio(struct sunxi_scr *pscr)
{
	if (!IS_ERR_OR_NULL(pscr->scr_pinctrl))
		devm_pinctrl_put(pscr->scr_pinctrl);
	pscr->scr_pinctrl = NULL;
	return ;
}

static uint32_t scr_init_reg(struct sunxi_scr *pscr)
{
	scr_global_interrupt_disable(pscr->reg_base);
	scr_set_interrupt_disable(pscr->reg_base, 0xffffffff);
	scr_clear_interrupt_status(pscr->reg_base, 0xffffffff);

	scr_flush_txfifo(pscr->reg_base);
	scr_flush_rxfifo(pscr->reg_base);

	scr_set_txfifo_threshold(pscr->reg_base, pscr->txfifo_thh);
	scr_set_rxfifo_threshold(pscr->reg_base, pscr->rxfifo_thh);

	scr_set_tx_repeat(pscr->reg_base, pscr->tx_repeat);
	scr_set_rx_repeat(pscr->reg_base, pscr->rx_repeat);

	scr_set_scclk_divisor(pscr->reg_base, pscr->scclk_div);
	scr_set_baud_divisor(pscr->reg_base, pscr->baud_div);
	scr_set_activation_time(pscr->reg_base, pscr->act_time);
	scr_set_reset_time(pscr->reg_base, pscr->rst_time);
	scr_set_atr_time(pscr->reg_base, pscr->atr_time);
	scr_set_guard_time(pscr->reg_base, pscr->guard_time);
	scr_set_chlimit_time(pscr->reg_base, pscr->chlimit_time);
	scr_set_atr_flush(pscr->reg_base, 1);
	scr_set_ts_recv(pscr->reg_base, 1);
	scr_set_t_protocol(pscr->reg_base, pscr->card_para.protocol_type);

	scr_receive_enable(pscr->reg_base);
	scr_transmit_enable(pscr->reg_base);

	scr_set_interrupt_enable(pscr->reg_base, pscr->inten_bm);
	scr_global_interrupt_enable(pscr->reg_base);

	scr_set_recv_parity(pscr->reg_base,
			    pscr->card_para.recv_no_parity);

	return 0;
}

static void sunxi_scr_param_init(struct sunxi_scr *pscr)
{
	/* init register parameters */
	pscr->inten_bm = 0xfffffff0;
	pscr->txfifo_thh = SCR_FIFO_DEPTH;
	pscr->rxfifo_thh = SCR_FIFO_DEPTH;
	pscr->tx_repeat = 0x3;
	pscr->rx_repeat = 0x3;
	pscr->scclk_div = 0;	/* (APB1CLK/4000000) PCLK/14, <175, && SCCLK >= 1M && =<4M */
	pscr->baud_div = 0;	/* ETU = 372*SCCLK */
	pscr->act_time = 2;	/* =1*256, 100 */
	pscr->rst_time = 0xff;	/* 2*256, >=400 */
	pscr->atr_time = 0xff;	/* scr.atr_time = (40000>>8)+1; //=256*256, 400~40000 */
	pscr->guard_time = 2;	/* =2*ETUs */
	pscr->chlimit_time = 100 * (10 + pscr->guard_time); /* interval time (400-1) characters */
	pscr->atr_resp = SCR_ATR_RESP_INVALID;
	pscr->rx_transmit_status = SCR_RX_TRANSMIT_NOYET;

	/* init card parameters */
	pscr->card_para.f = 372;
	pscr->card_para.d = 1;
	pscr->card_para.freq = 3579;	/* 3.579MHz, unit is KHz, expect baud=9600bps*/
	pscr->card_para.recv_no_parity = 1;
	pscr->card_para.protocol_type = 0;

	/* init atr data */
	pscr->smc_atr_para.TS = 0x3B;
	pscr->smc_atr_para.TK_NUM = 0x00;

	pscr->smc_atr_para.T = 0;	/* T=0 Protocol */
	pscr->smc_atr_para.FMAX = 4;	/* 4MHz, unit is MHz */
	pscr->smc_atr_para.F = 372;
	pscr->smc_atr_para.D = 1;
	pscr->smc_atr_para.I = 50;	/* 50mA */
	pscr->smc_atr_para.P = 5;	/* 5V */
	pscr->smc_atr_para.N = 2;

	/* pscr->clk_freq'unit is hz but pscr->card_para.freq'unit is khz */
	pscr->scclk_div = pscr->clk_freq / pscr->card_para.freq / 2000 - 1;
	pscr->baud_div = (pscr->scclk_div + 1) * (pscr->card_para.f / pscr->card_para.d) - 1;

	SCR_DBG("clk_freq=%d, scclk_div=%d, baud_div=%d\n",
		pscr->clk_freq, pscr->scclk_div, pscr->baud_div);

	/* init registers */
	scr_init_reg(pscr);
	return ;
}

/* use ATR data to reconfig smart card control register */
static void sunxi_scr_do_atr(struct sunxi_scr *pscr)
{
	struct scr_atr *pscr_atr_des = &pscr->scr_atr_des;
	struct smc_atr_para *psmc_atr_para = &pscr->smc_atr_para;
	struct smc_pps_para *psmc_pps_para = &pscr->smc_pps_para;

	SCR_DBG("\nBefore Decode:\n"
		"psmc_atr_para->TS   = 0x%x\n"
		"psmc_atr_para->T    = %d\n"
		"psmc_atr_para->FMAX = %d(MHz), Current: %d(KHz)\n"
		"psmc_atr_para->F    = %d\n"
		"psmc_atr_para->D    = %d\n",
		psmc_atr_para->TS,
		psmc_atr_para->T,
		psmc_atr_para->FMAX,
		pscr->card_para.freq, psmc_atr_para->F, psmc_atr_para->D);

	smartcard_atr_decode(psmc_atr_para, psmc_pps_para,
			     (uint8_t *)&pscr_atr_des->atr_data, 1);


	SCR_DBG("\nAfter Decode:\n"
		"psmc_atr_para->TS   = 0x%x\n"
		"psmc_atr_para->T    = %d\n"
		"psmc_atr_para->FMAX = %d(MHz), Current: %d(KHz)\n"
		"psmc_atr_para->F    = %d\n"
		"psmc_atr_para->D    = %d\n",
		psmc_atr_para->TS,
		psmc_atr_para->T,
		psmc_atr_para->FMAX,
		pscr->card_para.freq, psmc_atr_para->F, psmc_atr_para->D);


	/* use default F&D or set by up layer
	   pscr->card_para.f = pscr->smc_atr_para.F;
	   pscr->card_para.d = pscr->smc_atr_para.D;
	   pscr->card_para.freq = psmc_atr_para->FMAX * 1000;

	   pscr->scclk_div = pscr->clk_freq/pscr->card_para.freq/2000-1;
	   pscr->baud_div = (pscr->scclk_div + 1)*(pscr->card_para.f/pscr->card_para.d)-1;

	   scr_set_scclk_divisor(pscr->reg_base, pscr->scclk_div);
	   scr_set_baud_divisor(pscr->reg_base, pscr->baud_div);
	 */
	pscr->card_para.protocol_type = psmc_atr_para->T;
	scr_set_t_protocol(pscr->reg_base, pscr->card_para.protocol_type);

	if (0x3f == psmc_atr_para->TS) {
		/* 0x3b:direct convention, 0x3f:inverse convention */
		scr_set_data_order(pscr->reg_base, 1);
		scr_set_data_invert(pscr->reg_base, 1);
	}

	return ;
}

static uint32_t sunxi_scr_clk_init(struct sunxi_scr *pscr)
{
	struct platform_device *pdev = pscr->scr_device;
	struct device_node *node = pdev->dev.of_node;

	if (NULL == pdev || !of_device_is_available(node)) {
		SCR_DBG("platform_device invalid!\n");
		return -EINVAL;
	}

	pscr->scr_clk = of_clk_get(node, 0);
	if (!pscr->scr_clk || IS_ERR(pscr->scr_clk)) {
		SCR_DBG("try to get scr clock fail!\n");
		return -EINVAL;
	}

	pscr->scr_clk_source = of_clk_get(node, 1);
	if (!pscr->scr_clk_source || IS_ERR(pscr->scr_clk_source)) {
		SCR_DBG("err: try to get scr_clk_source clock fail!\n");
		return -EINVAL;
	}

	if (of_property_read_u32(node, "clock-frequency", &pscr->clk_freq)) {
		SCR_DBG("get clock-frequency fail! use default 24Mhz\n");
		pscr->clk_freq = 24000000;
	}

	if (clk_set_parent(pscr->scr_clk, pscr->scr_clk_source)) {
		SCR_DBG("set scr_clk parent to scr_clk_source fail!\n");
		return -EINVAL;
	}

	if (clk_set_rate(pscr->scr_clk, pscr->clk_freq)) {
		SCR_DBG("set ir scr_clk freq  failed!\n");
		return -EINVAL;
	}

	if (clk_prepare_enable(pscr->scr_clk)) {
		SCR_DBG("try to enable scr_clk failed!\n");
		return -EINVAL;
	}

	return 0;
}

static uint32_t sunxi_scr_clk_exit(struct sunxi_scr *pscr)
{
	if (NULL == pscr->scr_clk || IS_ERR(pscr->scr_clk)) {
		SCR_ERR("scr_clk handle is invalid, just return!\n");
		return -EINVAL;
	} else {
		clk_disable_unprepare(pscr->scr_clk);
		clk_put(pscr->scr_clk);
		pscr->scr_clk = NULL;
	}
	if (NULL == pscr->scr_clk_source || IS_ERR(pscr->scr_clk_source)) {
		SCR_ERR("scr_clk_source handle is invalid, just return!\n");
		return -EINVAL;
	} else {
		clk_put(pscr->scr_clk_source);
		pscr->scr_clk_source = NULL;
	}

	return 0;
}

static int sunxi_scr_open(struct inode *inode, struct file *file)
{
	file->private_data = pscr;

	if (pscr->open_cnt > 0) {
		SCR_DBG("smart card opened already\n");
		pscr->open_cnt++;
		return 0;
	}

	sunxi_scr_param_init(pscr);
	pscr->card_in = scr_get_det_status(pscr->reg_base) ? true : false;
	pscr->card_last = pscr->card_in;
	if (true == pscr->card_in)
		scr_set_activation(pscr->reg_base);

	pscr->open_cnt++;

	return 0;
}

static int sunxi_scr_release(struct inode *inode, struct file *file)
{
	struct sunxi_scr *pscr = file->private_data;

	if (--pscr->open_cnt) {
		SCR_DBG("There is not really close, just return!\n");
		return 0;
	}
	scr_set_deactivation(pscr->reg_base);
	scr_clear_csr_reg(pscr->reg_base);

	return 0;
}

static ssize_t
sunxi_scr_read(struct file *file, char __user *buf, size_t size, loff_t *ppos)
{
	struct sunxi_scr *pscr = file->private_data;
	uint32_t rx_size = 0;
	int try_num = 100;

	while ((SCR_RX_TRANSMIT_NOYET == pscr->rx_transmit_status) && try_num--) {
		msleep(50);
	};
	if (try_num < 0) {
		SCR_ERR("read timeout\n");
		return -EAGAIN;
	}

	rx_size = scr_buf_rx.cnt;
	if (rx_size > size)
		rx_size = size;

	if (copy_to_user(buf, scr_buf_rx.buf, rx_size))
		return -EFAULT;
	scr_flush_rxfifo(pscr->reg_base);

	return rx_size;
}

static int scr_write(struct sunxi_scr *pscr, char *buf, int size)
{
	int try_num = 100;
	int i;

	for (i = 0; i < size; i++) {
		while (scr_txfifo_is_full(pscr->reg_base) && try_num--) {
			msleep(50);
		}
		if (try_num < 0) {
			SCR_ERR("TX FIFO full, write timeout\n");
			break;
		}
		scr_write_fifo(pscr->reg_base, buf[i]);
		try_num = 100;
	}
	SCR_DBG("writed %d byte\n", i);

	return i;
}

static ssize_t
sunxi_scr_write(struct file *file, const char __user *buf, size_t size, loff_t *ppos)
{
	struct sunxi_scr *pscr = file->private_data;

	if (copy_from_user(scr_buf_tx.buf, buf, size))
		return -EFAULT;

	scr_flush_txfifo(pscr->reg_base);
	memset(&scr_buf_rx, 0, sizeof(struct scr_data));
	pscr->rx_transmit_status = SCR_RX_TRANSMIT_NOYET;

	return scr_write(pscr, (char *)buf, size);
}

static void scr_timer_handler(unsigned long data)
{
	(void)data;
	wake_up(&pscr->scr_poll);
}

unsigned int sunxi_scr_poll(struct file *file, struct poll_table_struct *wait)
{
	struct sunxi_scr *pscr = file->private_data;
	unsigned int mask = 0;

	/* add wait_queue to poll_table */
	poll_wait(file, &pscr->scr_poll,  wait);

	/* using Edge Triggered instand of Level Triggered */
	if (pscr->card_last^pscr->card_in) {
		if (pscr->card_in)
			mask |= POLLIN;
		else
			mask |= POLLOUT;

		pscr->card_last = pscr->card_in;
	}

	return mask;
}

static long sunxi_scr_ioctl(struct file *file, uint32_t cmd, unsigned long arg)
{
	struct sunxi_scr *pscr = file->private_data;
	uint32_t tmp = 0, ret = 0;
	int try_num = 300;

	SCR_ENTER();

	switch (cmd) {
	/* get smart card status 0:SCR_CARD_OUT, 1:SCR_CARD_IN */
	case SCR_IOCGSTATUS:
		tmp = scr_get_det_status(pscr->reg_base);
		ret = put_user(tmp, (int __user *)arg);
		break;

	/* reset card and store ATR data immediately */
	case SCR_IOCRESET:
		scr_set_activation(pscr->reg_base);
		mdelay(10);
		break;

	/* get ATR data, the arg type is struct scr_atr */
	case SCR_IOCGATR:
		while (SCR_ATR_RESP_OK != pscr->atr_resp && try_num--) {
			msleep(10);
		};
		if (try_num < 0) {
			SCR_ERR("SCR_IOCGATR timeout!\n");
			ret = -EAGAIN;
			break;
		}
		ret = copy_to_user((void __user *)arg, &pscr->scr_atr_des,
				   sizeof(struct scr_atr)) ? -EFAULT : 0;
		break;

	/* get current card parameters & status */
	case SCR_IOCGPARA:
		ret = copy_to_user((void __user *)arg, &pscr->card_para,
				   sizeof(struct scr_card_para)) ? -EFAULT : 0;
		break;

	/* set current card parameters & status */
	case SCR_IOCSPARA:
		if (copy_from_user(&pscr->card_para, (void __user *)arg,
				   sizeof(struct scr_card_para))) {
			SCR_ERR("get card para from user error!\n");
			ret = -EFAULT;
			break;
		}
		pscr->scclk_div = pscr->clk_freq / pscr->card_para.freq / 2000 - 1;
		pscr->baud_div = (pscr->scclk_div + 1) * (pscr->card_para.f / pscr->card_para.d) - 1;
		scr_set_scclk_divisor(pscr->reg_base, pscr->scclk_div);
		scr_set_baud_divisor(pscr->reg_base, pscr->baud_div);
		scr_set_recv_parity(pscr->reg_base, pscr->card_para.recv_no_parity);
		scr_set_t_protocol(pscr->reg_base, pscr->card_para.protocol_type);
		break;

	/* get the parse parameters come from ATR data */
	case SCR_IOCGATRPARA:
		ret = copy_to_user((void __user *)arg, &pscr->smc_atr_para,
				   sizeof(struct smc_atr_para)) ? -EFAULT : 0;
		break;

	/* get the pps parse parameters come from ATR data */
	case SCR_IOCGPPSPARA:
		ret = copy_to_user((void __user *)arg, &pscr->smc_pps_para,
				   sizeof(struct smc_pps_para)) ? -EFAULT : 0;
		break;

	/* write cmd and read data immediately */
	case SCR_IOCWRDATA: {
		int rtn_data_len;
		struct scr_wr_data wr_data;
		if (copy_from_user(&wr_data, (void __user *)arg,
				   sizeof(struct scr_wr_data))) {
			SCR_ERR("get wr_data from user error!\n");
			ret = -EFAULT;
			break;
		}
		if (copy_from_user(scr_buf_tx.buf, (void __user *)wr_data.cmd_buf, wr_data.cmd_len)) {
			SCR_ERR("get wr_data cmd_buf from user error!\n");
			ret = -EFAULT;
			break;
		}
		scr_buf_tx.cnt = wr_data.cmd_len;

		scr_flush_txfifo(pscr->reg_base);
		scr_flush_rxfifo(pscr->reg_base);
		memset(&scr_buf_rx, 0, sizeof(struct scr_data));

		/* APDU, smart card cammand format */
		/* type1: CLS + INS + P1 + P2 + le  -> only read, le=read size*/
		if (5 == scr_buf_tx.cnt) {
			scr_write(pscr, scr_buf_tx.buf, 5);
			/* respond: INS(=buf[1]) + valid_data(=buf[4]) + SW1 + SW2 */
			rtn_data_len = scr_buf_tx.buf[4] + 3;
			while ((rtn_data_len > scr_buf_rx.cnt) && try_num--) {
				msleep(10);
			};

			if (try_num < 0) {
				SCR_ERR("read timeout\n");
				ret = -EFAULT;
				break;
			}
			ret = copy_to_user((void __user *)wr_data.rtn_data, &scr_buf_rx.buf[1], scr_buf_tx.buf[4]) ? -EFAULT : 0;
			put_user(scr_buf_tx.buf[4], wr_data.rtn_len);
			put_user(scr_buf_rx.buf[rtn_data_len - 2], wr_data.psw1);
			put_user(scr_buf_rx.buf[rtn_data_len - 1], wr_data.psw2);

		/* type2: CLS + INS + P1 + P2 + lc + data -> only lc, write data, lc=data size */
		} else if (scr_buf_tx.buf[4]+5 == scr_buf_tx.cnt) {
			scr_write(pscr, scr_buf_tx.buf, 5);
			while ((0 == scr_buf_rx.cnt) && try_num--) {
				msleep(10);
			};
			if (try_num < 0) {
				SCR_ERR("timeout: there is not any data\n");
				ret = -EFAULT;
				break;
			}
			if (scr_buf_rx.buf[0] != scr_buf_tx.buf[1]) {
				SCR_ERR("do not support this instruction\n");
				ret = -EFAULT;
				break;
			}
			scr_write(pscr, &scr_buf_tx.buf[5], scr_buf_tx.buf[4]);
			try_num = 300;
			while ((scr_buf_rx.cnt < 3) && try_num--) {
				msleep(10);
			};
			if (try_num < 0) {
				SCR_ERR("timeout: get sw1,sw2 fail\n");
				ret = -EFAULT;
				break;
			}
			put_user(scr_buf_rx.buf[1], wr_data.psw1);
			put_user(scr_buf_rx.buf[2], wr_data.psw2);

		/* type3: CLS + INS + P1 + P2 + lc + data +le -> le+lc  */
		} else if (scr_buf_tx.buf[4]+6 == scr_buf_tx.cnt) {
			scr_write(pscr, scr_buf_tx.buf, 5);
			while ((0 == scr_buf_rx.cnt) && try_num--) {
				msleep(10);
			};
			if (try_num < 0) {
				SCR_ERR("timeout: there is not any data\n");
				ret = -EFAULT;
				break;
			}
			if (scr_buf_rx.buf[0] != scr_buf_tx.buf[1]) {
				SCR_ERR("do not support this instruction\n");
				ret = -EFAULT;
				break;
			}
			scr_write(pscr, &scr_buf_tx.buf[5], scr_buf_tx.buf[4]+1);
			try_num = 300;
			/* respond: INS + valid_data + SW1 + SW2 */
			rtn_data_len = scr_buf_tx.buf[scr_buf_tx.cnt-1] + 3;
			while ((rtn_data_len > scr_buf_rx.cnt) && try_num--) {
				msleep(10);
			};

			if (try_num < 0) {
				SCR_ERR("read timeout\n");
				ret = -EFAULT;
				break;
			}

			ret = copy_to_user((void __user *)wr_data.rtn_data, &scr_buf_rx.buf[1], scr_buf_tx.buf[scr_buf_tx.cnt-1]) ? -EFAULT : 0;
			put_user(scr_buf_tx.buf[scr_buf_tx.cnt-1], wr_data.rtn_len);
			put_user(scr_buf_rx.buf[rtn_data_len - 2], wr_data.psw1);
			put_user(scr_buf_rx.buf[rtn_data_len - 1], wr_data.psw2);

		} else {
			SCR_ERR("invalid command format\n");
			ret = -EFAULT;
			break;
		}
		break;
		}

	default:
		SCR_ERR("Invalid iocontrol command!\n");
		break;
	}
	return ret;
}

static const struct file_operations sunxi_scr_fops = {
	.owner = THIS_MODULE,
	.llseek = noop_llseek,
	.read = sunxi_scr_read,
	.write = sunxi_scr_write,
	.unlocked_ioctl = sunxi_scr_ioctl,
	.open = sunxi_scr_open,
	.release = sunxi_scr_release,
	.poll = sunxi_scr_poll,
};

static int sunxi_scr_probe(struct platform_device *pdev)
{
	struct device_node *node = pdev->dev.of_node;
	struct resource *mem_res = NULL;
	int ret = 0;

	SCR_ENTER();

	pscr = kzalloc(sizeof(struct sunxi_scr), GFP_KERNEL);
	if (!pscr) {
		SCR_ERR("kzalloc struct sunxi_scr fail!\n");
		return -ENOMEM;
	}

	pscr->scr_device = pdev;
	if (!of_device_is_available(node)) {
		SCR_ERR("invalid node!\n");
		ret = -EINVAL;
		goto emloc;
	}

	if (sunxi_scr_clk_init(pscr)) {
		SCR_ERR("sunxi_scr_clk_init fail!\n");
		ret = -EINVAL;
		goto eclk;
	}

	pscr->irq_no = platform_get_irq(pdev, 0);
	if (pscr->irq_no < 0) {
		SCR_ERR("get irq number fail!\n");
		ret = -EINVAL;
		goto eclk;
	}

	mem_res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (mem_res == NULL) {
		SCR_ERR("failed to get MEM res\n");
		ret = -ENXIO;
		goto eclk;
	}

	if (!request_mem_region(mem_res->start,
		resource_size(mem_res), mem_res->name)) {
		SCR_ERR("failed to request mem region\n");
		ret = -EINVAL;
		goto eclk;
	}

	pscr->reg_base = ioremap(mem_res->start, resource_size(mem_res));
	if (!pscr->reg_base) {
		SCR_ERR("failed to io remap\n");
		ret = -EIO;
		goto eiomem;
	}
	pscr->mem_res = mem_res;

	if (request_irq(pscr->irq_no, sunxi_scr_interrupt,
			IRQF_TRIGGER_NONE, "scr", pscr)) {
		SCR_ERR("request irq fail!\n");
		ret = -EINVAL;
		goto eiomap;
	}

	if (scr_request_gpio(pscr)) {
		SCR_ERR("failed to request gpio\n");
		ret = -EINVAL;
		goto eirq;
	}

	spin_lock_init(&pscr->rx_lock);
	sunxi_scr_param_init(pscr);
	init_waitqueue_head(&pscr->scr_poll);
	pscr->poll_timer.expires = jiffies + HZ/100; /* 10ms */
	pscr->poll_timer.function = scr_timer_handler;
	init_timer(&pscr->poll_timer);
	add_timer(&pscr->poll_timer);

	/* creat character device */
	sunxi_scr_major = register_chrdev(0, SCR_MODULE_NAME, &sunxi_scr_fops);
	if (sunxi_scr_major < 0) {
		SCR_ERR("register_chrdev fail!\n");
		ret = -ENODEV;
		goto eirq;
	}
	scr_dev_class = class_create(THIS_MODULE, SCR_MODULE_NAME);
	if (IS_ERR(scr_dev_class)) {
		SCR_ERR("class_create fail!\n");
		ret = -ENODEV;
		goto edev;
	}
	scr_device = device_create(scr_dev_class, NULL, MKDEV(sunxi_scr_major, 0),
				   NULL, SCR_MODULE_NAME);
	if (IS_ERR(scr_device)) {
		SCR_ERR("device_create fail!\n");
		ret = -ENODEV;
		goto ecla;
	}

	platform_set_drvdata(pdev, pscr);

	return 0;

ecla:
	class_destroy(scr_dev_class);

edev:
	unregister_chrdev(sunxi_scr_major, SCR_MODULE_NAME);

eirq:
	free_irq(pscr->irq_no, pscr);

eiomap:
	iounmap(pscr->reg_base);

eiomem:
	release_mem_region(mem_res->start, resource_size(mem_res));

eclk:
	sunxi_scr_clk_exit(pscr);

emloc:
	kfree(pscr);

	return ret;

}

static int sunxi_scr_remove(struct platform_device *pdev)
{
	struct sunxi_scr *pscr = platform_get_drvdata(pdev);

	device_destroy(scr_dev_class, MKDEV(sunxi_scr_major, 0));
	class_destroy(scr_dev_class);
	unregister_chrdev(sunxi_scr_major, SCR_MODULE_NAME);
	free_irq(pscr->irq_no, pscr);
	iounmap(pscr->reg_base);
	release_mem_region(pscr->mem_res->start,
			   resource_size(pscr->mem_res));
	scr_release_gpio(pscr);
	sunxi_scr_clk_exit(pscr);

	del_timer(&pscr->poll_timer);

	SCR_EXIT();

	return 0;
}

#ifdef CONFIG_PM
static int sunxi_scr_suspend(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct sunxi_scr *pscr = platform_get_drvdata(pdev);
	struct pinctrl_state *pctrl_state = NULL;

	pscr->suspended = true;

	if (sunxi_scr_clk_exit(pscr)) {
		SCR_ERR("SCR suspend failed !\n");
		pscr->suspended = false;
		return -1;
	}

	if (!IS_ERR_OR_NULL(pscr->scr_pinctrl)) {
		pctrl_state = pinctrl_lookup_state(pscr->scr_pinctrl, PINCTRL_STATE_SLEEP);
		if (IS_ERR(pctrl_state)) {
			SCR_ERR("SCR pinctrl lookup sleep fail\n");
			return -1;
		}

		if (pinctrl_select_state(pscr->scr_pinctrl, pctrl_state) < 0) {
			SCR_ERR("SCR pinctrl select sleep fail\n");
			return -1;
		}
	}

	disable_irq_nosync(pscr->irq_no);

	SCR_DBG("SCR suspend okay\n");
	return 0;
}

static int sunxi_scr_resume(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct sunxi_scr *pscr = platform_get_drvdata(pdev);
	struct pinctrl_state *pctrl_state = NULL;


	pscr->suspended = false;

	if (sunxi_scr_clk_init(pscr)) {
		SCR_ERR("SCR resume failed !\n");
		return -1;
	}

	if (!IS_ERR_OR_NULL(pscr->scr_pinctrl)) {
		pctrl_state = pinctrl_lookup_state(pscr->scr_pinctrl, PINCTRL_STATE_DEFAULT);
		if (IS_ERR(pctrl_state)) {
			SCR_ERR("SCR pinctrl lookup default fail\n");
			return -1;
		}

		if (pinctrl_select_state(pscr->scr_pinctrl, pctrl_state) < 0) {
			SCR_ERR("SCR pinctrl select default fail\n");
			return -1;
		}
	}

	enable_irq(pscr->irq_no);

	SCR_DBG("SCR resume okay\n");
	return 0;
}

static const struct dev_pm_ops sunxi_scr_dev_pm_ops = {
	.suspend = sunxi_scr_suspend,
	.resume = sunxi_scr_resume,
};

#define SUNXI_SCR_DEV_PM_OPS (&sunxi_scr_dev_pm_ops)
#else
#define SUNXI_SCR_DEV_PM_OPS NULL
#endif

static const struct of_device_id sunxi_scr_match[] = {
	{.compatible = "allwinner,sunxi-scr",},
	{},
};

MODULE_DEVICE_TABLE(of, sunxi_scr_match);

static struct platform_driver scr_platform_driver = {
	.probe = sunxi_scr_probe,
	.remove = sunxi_scr_remove,
	.driver = {
		   .name = SCR_MODULE_NAME,
		   .owner = THIS_MODULE,
		   .pm = SUNXI_SCR_DEV_PM_OPS,
		   .of_match_table = sunxi_scr_match,
		   },
};

static int __init sunxi_scr_init(void)
{
	return platform_driver_register(&scr_platform_driver);
}

static void __exit sunxi_scr_exit(void)
{
	platform_driver_unregister(&scr_platform_driver);
}

module_init(sunxi_scr_init);
module_exit(sunxi_scr_exit);
MODULE_DESCRIPTION("Smart Card Driver");
MODULE_AUTHOR("fuzhaoke");
MODULE_LICENSE("GPL");
