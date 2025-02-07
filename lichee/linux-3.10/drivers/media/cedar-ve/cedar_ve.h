/*
* Copyright (c) 2008-2016 Allwinner Technology Co. Ltd.
* All rights reserved.
*
* File : cedarv_ve.h
* Description : Video engine driver API, Don't modify it in user space.
* History :
*   Author  : xyliu <xyliu@allwinnertech.com>
*   Date    : 2016/04/13
*   Comment :
*/
/* Notice: It's video engine driver API, Don't modify it in user space. */
#ifndef _CEDAR_VE_H_
#define _CEDAR_VE_H_

enum IOCTL_CMD {
	IOCTL_UNKOWN = 0x100,
	IOCTL_GET_ENV_INFO,
	IOCTL_WAIT_VE_DE,
	IOCTL_WAIT_VE_EN,
	IOCTL_RESET_VE,
	IOCTL_ENABLE_VE,
	IOCTL_DISABLE_VE,
	IOCTL_SET_VE_FREQ,
	
	IOCTL_CONFIG_AVS2 = 0x200,
	IOCTL_GETVALUE_AVS2 ,
	IOCTL_PAUSE_AVS2 ,
	IOCTL_START_AVS2 ,
	IOCTL_RESET_AVS2 ,
	IOCTL_ADJUST_AVS2,
	IOCTL_ENGINE_REQ,
	IOCTL_ENGINE_REL,
	IOCTL_ENGINE_CHECK_DELAY,
	IOCTL_GET_IC_VER,
	IOCTL_ADJUST_AVS2_ABS,
	IOCTL_FLUSH_CACHE,
	IOCTL_SET_REFCOUNT,
	IOCTL_FLUSH_CACHE_ALL,
	IOCTL_TEST_VERSION,

	IOCTL_GET_LOCK = 0x310,
	IOCTL_RELEASE_LOCK,
	
	IOCTL_SET_VOL = 0x400,

	IOCTL_WAIT_JPEG_DEC = 0x500,
	/*for get the ve ref_count for ipc to delete the semphore*/
	IOCTL_GET_REFCOUNT,
};

#define VE_LOCK_VDEC 0x01
#define VE_LOCK_VENC 0x02
#define VE_LOCK_JDEC 0x04
#define VE_LOCK_ERR  0x80

struct cedarv_env_infomation{
	unsigned int phymem_start;
	int  phymem_total_size;
	unsigned long  address_macc;
};

#endif
