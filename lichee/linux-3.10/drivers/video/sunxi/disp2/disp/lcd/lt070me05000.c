#include "lt070me05000.h"

static void LCD_power_on(u32 sel);
static void LCD_power_off(u32 sel);
static void LCD_bl_open(u32 sel);
static void LCD_bl_close(u32 sel);

static void LCD_panel_init(u32 sel);
static void LCD_panel_exit(u32 sel);

#define panel_reset(val) sunxi_lcd_gpio_set_value(sel, 0, val)
#define power_en(val)  sunxi_lcd_gpio_set_value(sel, 1, val)

#define DK
#define DK_A64

#ifdef DK_A64

#define REGFLAG_END_OF_TABLE     0x102
#define REGFLAG_DELAY            0x101

struct lcd_setting_table {
    u16 cmd;
    u32 count;
    u8 para_list[64];
};

static struct lcd_setting_table lcm_initialization_setting[] = {
	{0x01,1,{0x00}},        //data type 05
    {REGFLAG_DELAY, 12, {}},  // delay
    {0xb0,1,{0x00}},       //data type 23
    {0xb3,5,{0x14,0x08,0x00,0x22,0x00}},  //data type 29
    {0xb4,1,{0x0c}},                 //data type  29
    {0xb6,2,{0x3a,0xd3}},            //data type  29
    {0x51,1,{0xe6}},                //data type 15
    {0x53,1,{0x2c}},                //data type 15
    {0x29,1,{0x00}},              //data type 05
    {REGFLAG_DELAY, 12, {}},
    {0x11,1,{0x00}},             //data type  05

    {REGFLAG_DELAY, 120, {}},

    {REGFLAG_END_OF_TABLE,0x00,{}}
};

#endif

static void LCD_cfg_panel_info(panel_extend_para * info)
{
	u32 i = 0, j=0;
	u32 items;
	u8 lcd_gamma_tbl[][2] =
	{
		//{input value, corrected value}
		{0, 0},
		{15, 15},
		{30, 30},
		{45, 45},
		{60, 60},
		{75, 75},
		{90, 90},
		{105, 105},
		{120, 120},
		{135, 135},
		{150, 150},
		{165, 165},
		{180, 180},
		{195, 195},
		{210, 210},
		{225, 225},
		{240, 240},
		{255, 255},
	};

	u32 lcd_cmap_tbl[2][3][4] = {
	{
		{LCD_CMAP_G0,LCD_CMAP_B1,LCD_CMAP_G2,LCD_CMAP_B3},
		{LCD_CMAP_B0,LCD_CMAP_R1,LCD_CMAP_B2,LCD_CMAP_R3},
		{LCD_CMAP_R0,LCD_CMAP_G1,LCD_CMAP_R2,LCD_CMAP_G3},
		},
		{
		{LCD_CMAP_B3,LCD_CMAP_G2,LCD_CMAP_B1,LCD_CMAP_G0},
		{LCD_CMAP_R3,LCD_CMAP_B2,LCD_CMAP_R1,LCD_CMAP_B0},
		{LCD_CMAP_G3,LCD_CMAP_R2,LCD_CMAP_G1,LCD_CMAP_R0},
		},
	};

	items = sizeof(lcd_gamma_tbl)/2;
	for (i=0; i<items-1; i++) {
		u32 num = lcd_gamma_tbl[i+1][0] - lcd_gamma_tbl[i][0];

		for (j=0; j<num; j++) {
			u32 value = 0;

			value = lcd_gamma_tbl[i][1] + ((lcd_gamma_tbl[i+1][1] - lcd_gamma_tbl[i][1]) * j)/num;
			info->lcd_gamma_tbl[lcd_gamma_tbl[i][0] + j] = (value<<16) + (value<<8) + value;
		}
	}
	info->lcd_gamma_tbl[255] = (lcd_gamma_tbl[items-1][1]<<16) + (lcd_gamma_tbl[items-1][1]<<8) + lcd_gamma_tbl[items-1][1];

	memcpy(info->lcd_cmap_tbl, lcd_cmap_tbl, sizeof(lcd_cmap_tbl));

}

static s32 LCD_open_flow(u32 sel)
{
	printk("[BPI-lt070me05000]LCD_open_flow\n");
	
	LCD_OPEN_FUNC(sel, LCD_power_on, 20);   //open lcd power, and delay 50ms
	LCD_OPEN_FUNC(sel, LCD_panel_init, 10);   //open lcd power, than delay 200ms
	LCD_OPEN_FUNC(sel, sunxi_lcd_tcon_enable, 20);     //open lcd controller, and delay 100ms
	LCD_OPEN_FUNC(sel, LCD_bl_open, 0);     //open lcd backlight, and delay 0ms

	return 0;
}

static s32 LCD_close_flow(u32 sel)
{
	printk("[BPI-lt070me05000]LCD_close_flow\n");
	LCD_CLOSE_FUNC(sel, LCD_bl_close, 0);       //close lcd backlight, and delay 0ms
	LCD_CLOSE_FUNC(sel, sunxi_lcd_tcon_disable, 0);         //close lcd controller, and delay 0ms
	LCD_CLOSE_FUNC(sel, LCD_panel_exit,	200);   //open lcd power, than delay 200ms
	LCD_CLOSE_FUNC(sel, LCD_power_off, 500);   //close lcd power, and delay 500ms

	return 0;
}

static void LCD_power_on(u32 sel)
{
	printk("[BPI-lt070me05000]LCD_power_on\n");

	sunxi_lcd_power_enable(sel, 0);//config lcd_power pin to open lcd power
	sunxi_lcd_delay_ms(5);
	sunxi_lcd_power_enable(sel, 1);//config lcd_power pin to open lcd power0
	sunxi_lcd_delay_ms(5);
	sunxi_lcd_power_enable(sel, 2);//config lcd_power pin to open lcd power2
	sunxi_lcd_delay_ms(5);
	
	/* jdi, power on */
#ifdef DK
	power_en(1);
	sunxi_lcd_delay_ms(50);
	panel_reset(1);
	//sunxi_lcd_delay_ms(20);
	//panel_reset(0);
	//sunxi_lcd_delay_ms(10);
	//panel_reset(1);
	sunxi_lcd_delay_ms(20);
#endif
	
	sunxi_lcd_pin_cfg(sel, 1);
}

static void LCD_power_off(u32 sel)
{
	printk("[BPI-lt070me05000]LCD_power_off\n");
	
	sunxi_lcd_pin_cfg(sel, 0);
	
	power_en(0);
	sunxi_lcd_delay_ms(20);
	panel_reset(0);

	sunxi_lcd_delay_ms(5);
	sunxi_lcd_power_disable(sel, 2);//config lcd_power pin to close lcd power2
	sunxi_lcd_delay_ms(5);
	sunxi_lcd_power_disable(sel, 1);//config lcd_power pin to close lcd power1
	sunxi_lcd_delay_ms(5);
	sunxi_lcd_power_disable(sel, 0);//config lcd_power pin to close lcd power
}

static void LCD_bl_open(u32 sel)
{
	printk("[BPI-lt070me05000]LCD_bl_open\n");
	sunxi_lcd_pwm_enable(sel);
	sunxi_lcd_backlight_enable(sel);//config lcd_bl_en pin to open lcd backlight
}

static void LCD_bl_close(u32 sel)
{
	printk("[BPI-lt070me05000]LCD_bl_close\n");
	sunxi_lcd_backlight_disable(sel);
	sunxi_lcd_pwm_disable(sel);
}

#ifdef DK
static void LCD_panel_init(u32 sel)
{
#ifndef DK_A64
	disp_panel_para *panel_info = kmalloc(sizeof(disp_panel_para), GFP_KERNEL | __GFP_ZERO);
	
	u32 bright = 0;
	u8 para[9];

	bsp_disp_get_panel_info(sel, panel_info);
	bright = bsp_disp_lcd_get_bright(sel);

	printk("[BPI-lt070me05000]LCD_panel_init, bright=%d\n", bright);

	sunxi_lcd_dsi_clk_enable(sel);
	sunxi_lcd_delay_ms(20);
	
	/* type 05, 15, 39 use dcs, type 13, 23, 29 use gen */
	sunxi_lcd_dsi_dcs_write_0para(sel,DSI_DCS_SOFT_RESET);
	sunxi_lcd_delay_ms(10);
	
	sunxi_lcd_dsi_gen_write_1para(sel,0xb4,0x0c);
	sunxi_lcd_dsi_gen_write_2para(sel,0xb6,0x3a,0xd3);
	sunxi_lcd_dsi_dcs_write_1para(sel,0x51,0xe6);
	sunxi_lcd_dsi_dcs_write_1para(sel,0x53,0x24);
	sunxi_lcd_dsi_dcs_write_1para(sel,0x55,0x0);
	sunxi_lcd_dsi_dcs_write_1para(sel,DSI_DCS_SET_PIXEL_FORMAT,0x77);
	sunxi_lcd_dsi_dcs_write_4para(sel,DSI_DCS_SET_COLUMN_ADDRESS,0x00,0x00,0x04,0xaf);
	sunxi_lcd_dsi_dcs_write_4para(sel,DSI_DCS_SET_PAGE_ADDRESS,0x00,0x00,0x07,0x7f);
	sunxi_lcd_dsi_dcs_write_0para(sel,DSI_DCS_EXIT_SLEEP_MODE);
	sunxi_lcd_delay_ms(120);
	sunxi_lcd_dsi_dcs_write_0para(sel,DSI_DCS_SET_DISPLAY_ON);
	sunxi_lcd_delay_ms(200);
	sunxi_lcd_dsi_gen_write_1para(sel,0xb0,0x00);
	sunxi_lcd_delay_ms(10);
	sunxi_lcd_dsi_gen_write_5para(sel,0xb3,0x14,0x08,0x00,0x22,0x00);
	sunxi_lcd_delay_ms(10);
	sunxi_lcd_dsi_gen_write_1para(sel,0xb0,0x03);
	sunxi_lcd_delay_ms(10);

	sunxi_lcd_dsi_gen_write_1para(sel,0xd6,0x01);
	para[0] = 0x31;
	para[1] = 0xf7;
	para[2] = 0x80;
	para[3] = 0x00;
	para[4] = panel_info->lcd_vbp-1;
	para[5] = 0x00;
	para[6] = 0x08;
	para[7] = 0x00;
	para[8] = 0x00;
	sunxi_lcd_dsi_gen_write(sel, 0xc2, para, 9);

	sunxi_lcd_dsi_clk_enable(sel);
	sunxi_lcd_delay_ms(20);
	
	kfree(panel_info);

	return;
#else

	u32 i;

	printk("[BPI-lt070me05000]LCD_panel_init\n");
	
	for (i = 0; ; i++) {
        	if(lcm_initialization_setting[i].cmd == REGFLAG_END_OF_TABLE) {
            		break;
        	} 
		else if (lcm_initialization_setting[i].cmd == REGFLAG_DELAY) {
            		sunxi_lcd_delay_ms(lcm_initialization_setting[i].count);
        	} else {
            		dsi_dcs_wr(sel, (u8)lcm_initialization_setting[i].cmd, lcm_initialization_setting[i].para_list, lcm_initialization_setting[i].count);
        	}
    	}

	sunxi_lcd_dsi_clk_enable(sel);

	return;
#endif
}
#else
static void LCD_panel_init(u32 sel)
{
	printk("[BPI-lt070me05000]LCD_panel_init\n");
		
	power_en(1);
	sunxi_lcd_delay_ms(20);
	panel_reset(1);
	sunxi_lcd_delay_ms(20);
	panel_reset(0);
	sunxi_lcd_delay_ms(80);
	panel_reset(1);
	sunxi_lcd_delay_ms(80);

	sunxi_lcd_dsi_clk_enable(sel);
	sunxi_lcd_delay_ms(20);
}

#endif

static void LCD_panel_exit(u32 sel)
{
	sunxi_lcd_dsi_dcs_write_0para(sel,DSI_DCS_SET_DISPLAY_OFF);
	sunxi_lcd_delay_ms(20);
	sunxi_lcd_dsi_dcs_write_0para(sel,DSI_DCS_ENTER_SLEEP_MODE);
	sunxi_lcd_delay_ms(80);

	sunxi_lcd_dsi_clk_disable(sel);

	return ;
}

//sel: 0:lcd0; 1:lcd1
static s32 LCD_user_defined_func(u32 sel, u32 para1, u32 para2, u32 para3)
{
	return 0;
}

__lcd_panel_t lt070me05000_panel = {
	/* panel driver name, must mach the name of lcd_drv_name in sys_config.fex */
	.name = "lt070me05000_panel",
	.func = {
		.cfg_panel_info = LCD_cfg_panel_info,
		.cfg_open_flow = LCD_open_flow,
		.cfg_close_flow = LCD_close_flow,
		.lcd_user_defined_func = LCD_user_defined_func,
	},
};
