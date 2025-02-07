/*
 * Copyright (C) 2012 Invensense, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _INV_MPU_IIO_REG_20602_H_
#define _INV_MPU_IIO_REG_20602_H_

/*register and associated bit definition*/
#define REG_XA_OFFS_H		0x77
#define REG_YA_OFFS_H		0x7A
#define REG_ZA_OFFS_H           0x7D
#define REG_XA_OFFS_L_TC        0x7
#define REG_PRODUCT_ID          0xC
#define REG_ST_GCT_X            0xD
#define REG_XG_OFFS_USR_H        0x13
#define REG_YG_OFFS_USR_H        0x15
#define REG_ZG_OFFS_USR_H        0x17
#define REG_SAMPLE_RATE_DIV     0x19

#define REG_CONFIG              0x1A
#define EXT_SYNC_SET                      8

#define REG_GYRO_CONFIG		0x1B
#define BITS_SELF_TEST_EN		0xE0
#define SHIFT_GYRO_FS_SEL		0x03

#define REG_ACCEL_CONFIG	0x1C
#define SHIFT_ACCEL_FS			0x03

#define REG_LP_MODE_CTRL	0x1E
#define BIT_GYRO_CYCLE_EN               0x80

#define REG_ACCEL_WOM_THR	0x1F
#define REG_ACCEL_WOM_X_THR	0x20
#define REG_ACCEL_WOM_Y_THR	0x21
#define REG_ACCEL_WOM_Z_THR	0x22

#define REG_ACCEL_MOT_THR       0x1F
#define REG_ACCEL_MOT_DUR       0x20

#define REG_ACCEL_CONFIG_2  0x1D
#define BIT_ACCEL_FCHOCIE_B              0x08

#define REG_FIFO_EN			0x23
#define BITS_GYRO_FIFO_EN	0x10
#define BIT_ACCEL_FIFO_EN	0x08

#define REG_FSYNC_INT		0x36
#define BIT_FSYNC_INT                   0x80

#define REG_INT_PIN_CFG		0x37

#define REG_INT_ENABLE		0x38
#define BIT_WOM_X_INT_EN		0x80
#define BIT_WOM_Y_INT_EN		0x40
#define BIT_WOM_Z_INT_EN		0x20
#define BIT_WOM_ALL_INT_EN		0xE0
#define BIT_FSYNC_INT_EN		0x8
#define BIT_DATA_RDY_EN		        0x1

#define REG_WM_INT_STATUS       0x39

#define REG_INT_STATUS          0x3A
#define BIT_WOM_X_INT                  0x80
#define BIT_WOM_Y_INT                  0x40
#define BIT_WOM_Z_INT                  0x20

#define REG_RAW_ACCEL           0x3B
#define REG_EXT_SENS_DATA_00    0x49
#define REG_EXT_SENS_DATA_08    0x51
#define REG_EXT_SENS_DATA_09    0x52

#define REG_FIFO_WM_TH1         0x60
#define REG_FIFO_WM_TH2         0x61

#define REG_INT2_STATUS         0x62

#define REG_ACCEL_INTEL_CTRL 0x69
#define BIT_ACCEL_INTEL_EN              0x80
#define BIT_ACCEL_INTEL_MODE            0x40

#define REG_USER_CTRL			0x6A
#define BIT_COND_RST				0x01
#define BIT_FIFO_RST				0x04
#define BIT_FIFO_EN				0x40

#define REG_PWR_MGMT_1			0x6B
#define BIT_H_RESET				0x80
#define BIT_SLEEP					0x40
#define BIT_LP_EN                       	0x20
#define BIT_CLK_PLL				0x01
#define BIT_CLK_MASK				0x07

#define REG_PWR_MGMT_2			0x6C
#define BIT_PWR_ACCEL_STBY		0x38
#define BIT_PWR_GYRO_STBY		0x07
#define BIT_PWR_ALL_OFF			0x3F

#define REG_MEM_BANK_SEL	0x6D
#define REG_MEM_START_ADDR	0x6E
#define REG_MEM_R_W		0x6F

#define REG_SW_ID2 		0x70
#define BIT_ACC_DEFAULT                   (2 << 4)

#define REG_FIFO_COUNT_H        0x72
#define REG_FIFO_R_W            0x74
#define REG_WHO_AM_I              0x75

#define REG_6500_XG_ST_DATA     0x50
#define REG_6500_XA_ST_DATA     0xD
#define REG_6500_XA_OFFS_H      0x77
#define REG_6500_YA_OFFS_H      0x7A
#define REG_6500_ZA_OFFS_H      0x7D
#define REG_6500_ACCEL_CONFIG2  0x1D
#define BIT_ACCEL_FCHOCIE_B              0x08
#define BIT_FIFO_SIZE_1K                 0x40

#define REG_LP_MODE_CFG		0x1E

#define REG_6500_LP_ACCEL_ODR   0x1E
#define REG_6500_ACCEL_WOM_THR  0x1F

/* data output control reg 2 */
#define ACCEL_ACCURACY_SET  0x4000
#define GYRO_ACCURACY_SET   0x2000
#define CPASS_ACCURACY_SET  0x1000

/* data definitions */
#define ACCEL_COVARIANCE 0
#define BYTES_PER_SENSOR         6
#define BYTES_FOR_TEMP           2
#define FIFO_COUNT_BYTE          2
#define HARDWARE_FIFO_SIZE       1024
#define FIFO_SIZE                (HARDWARE_FIFO_SIZE * 7 / 8)
#define POWER_UP_TIME            100
#define REG_UP_TIME_USEC         100
#define LEFT_OVER_BYTES          128
#define IIO_BUFFER_BYTES         8
#define BASE_SAMPLE_RATE         1000
#define DRY_RUN_TIME             50

#ifdef BIAS_CONFIDENCE_HIGH
#define DEFAULT_ACCURACY         3
#else
#define DEFAULT_ACCURACY         1
#endif

/* enum for sensor
   The sequence is important.
   It represents the order of apperance from DMP */
enum INV_SENSORS {
	SENSOR_ACCEL = 0,
	SENSOR_TEMP,
	SENSOR_GYRO,
	SENSOR_COMPASS,
	SENSOR_NUM_MAX,
	SENSOR_INVALID,
};

enum inv_filter_e {
	INV_FILTER_256HZ_NOLPF2 = 0,
	INV_FILTER_188HZ,
	INV_FILTER_98HZ,
	INV_FILTER_42HZ,
	INV_FILTER_20HZ,
	INV_FILTER_10HZ,
	INV_FILTER_5HZ,
	INV_FILTER_2100HZ_NOLPF,
	NUM_FILTER
};

#define MPU_DEFAULT_DMP_FREQ     200
#define PEDOMETER_FREQ           (MPU_DEFAULT_DMP_FREQ >> 2)
#define SENSOR_FUSION_MIN_RATE   100
#define GESTURE_ACCEL_RATE       50

#define MIN_MST_ODR_CONFIG       4
#define MAX_MST_ODR_CONFIG       5
#define MIN_COMPASS_RATE         10
#define MAX_COMPASS_RATE         100
/* initial rate is important. For non-DMP mode, it is set as 4 1000/256*/
#define MPU_INIT_SENSOR_RATE     4
#define MAX_MST_NON_COMPASS_ODR_CONFIG 7
#define THREE_AXES               3
#define NINE_ELEM                (THREE_AXES * THREE_AXES)
#define MPU_TEMP_SHIFT           16
#define DMP_DIVIDER              (BASE_SAMPLE_RATE / MPU_DEFAULT_DMP_FREQ)
#define MAX_5_BIT_VALUE          0x1F
#define BAD_COMPASS_DATA         0x7FFF
#define BAD_CAL_COMPASS_DATA     0x7FFF0000
#define DEFAULT_BATCH_RATE       400
#define DEFAULT_BATCH_TIME    (MSEC_PER_SEC / DEFAULT_BATCH_RATE)
#define NINEQ_DEFAULT_COMPASS_RATE 25

#define DATA_AKM_99_BYTES_DMP  10
#define DATA_AKM_89_BYTES_DMP  9
#define DATA_ALS_BYTES_DMP     8
#define APDS9900_AILTL_REG      0x04
#define BMP280_DIG_T1_LSB_REG                0x88
#define TEMPERATURE_SCALE  3340827L
#define TEMPERATURE_OFFSET 1376256L
#define SECONDARY_INIT_WAIT 100
#define MPU_SOFT_REV_ADDR               0x86
#define MPU_SOFT_REV_MASK               0xf
#define SW_REV_LP_EN_MODE               4
#define AK99XX_SHIFT                    23
#define AK89XX_SHIFT                    22

/* this is derived from 1000 divided by 55, which is the pedometer
   running frequency */
#define MS_PER_PED_TICKS         18

/* data limit definitions */
#define MIN_FIFO_RATE            4
#define MAX_FIFO_RATE            MPU_DEFAULT_DMP_FREQ
#define MAX_DMP_OUTPUT_RATE      MPU_DEFAULT_DMP_FREQ

#define MAX_MPU_MEM              8192
#define MAX_PRS_RATE             281

enum inv_devices {
	ICM20608D,
	ICM20690,
	ICM20602,
	INV_NUM_PARTS,
};
#endif
