/**
 * @file lis2dtw12.h
 * @author Ben Owen (ben@embeddedshark.com)
 * @brief Header file for lis2dtw12.h
 * 
 * @date 2024-10-21
 * 
 * @copyright Copyright (c) 2024 Embedded Shark, LLC
 * 
 */

#ifndef __LIS2DTW12_H__
#define __LIS2DTW12_H__

#include "stdint.h"

/**
 * REGISTER LOCATIONS
 */
#define LIS2DTW12_OUT_T_L                       0x0D    // Temp sensor output
#define LIS2DTW12_OUT_T_H                       0x0E
#define LIS2DTW12_WHO_AM_I                      0x0F    // Who am I ID
#define LIS2DTW12_CTRL1                         0x20    // Control registers
#define LIS2DTW12_CTRL2                         0x21
#define LIS2DTW12_CTRL3                         0x22
#define LIS2DTW12_CTRL4_INT1_PAD_CTRL           0x23
#define LIS2DTW12_CTRL5_INT2_PAD_CTRL           0x24
#define LIS2DTW12_CTRL6                         0x25
#define LIS2DTW12_STATUS                        0x27    // Status data register
#define LIS2DTW12_OUT_X_L                       0x28    // Output registers
#define LIS2DTW12_OUT_X_H                       0x29
#define LIS2DTW12_OUT_Y_L                       0x2A
#define LIS2DTW12_OUT_Y_H                       0x2B
#define LIS2DTW12_OUT_Z_L                       0x2C
#define LIS2DTW12_OUT_Z_H                       0x2D
#define LIS2DTW12_FIFO_CTRL                     0x2E    // FIFO control register
#define LIS2DTW12_FIFI_SAMPLES                  0x2F    // Unread samples stored in FIFO
#define LIS2DTW12_TAP_THS_X                     0x30    // Tap thresholds
#define LIS2DTW12_TAP_THS_Y                     0x31
#define LIS2DTW12_TAP_THS_Z                     0x32
#define LIS2DTW12_INT_DUR                       0x33    // Interrupt duration
#define LIS2DTW12_WAKE_UP_THS                   0x34    // Tap/double tap selection, inactivity enable, wake-up threshold
#define LIS2DTW12_WAKE_UP_DUR                   0x35    // Wake-up duration
#define LIS2DTW12_FREE_FALL                     0x36    // Free-fall configuration
#define LIS2DTW12_STATUS_DUP                    0x37    // Status register
#define LIS2DTW12_WAKE_UP_SRC                   0x38    // Wake-up source
#define LIS2DTW12_TAP_SRC                       0x39    // Tap source
#define LIS2DTW12_SIXD_SRC                      0x3A    // 6D source
#define LIS2DTW12_ALL_INT_SRC                   0x3B
#define LIS2DTW12_X_OFS_USR                     0x3C
#define LIS2DTW12_Y_OFS_USR                     0x3D
#define LIS2DTW12_Z_OFS_USR                     0x3E
#define LIS2DTW12_CTRL7                         0x3F

#define I2C_ADDR_SA0_H                          0b0011001
#define I2C_ADDR_SA0_L                          0b0011000
#define DEVICE_ID                               0x44    // Value stored in WHO_AM_I

/**
 * REGISTER FIELDS
 */

/** 
 * CTRL1 REGISTER (20h)
 */
#define CTRL1_ODR_POWER_DOWN                    0x00
#define CTRL1_ODR_125_16_HZ                     0x10
#define CTRL1_ODR_125_HZ                        0x20
#define CTRL1_ODR_25_HZ                         0x30
#define CTRL1_ODR_50_HZ                         0x40
#define CTRL1_ODR_100_HZ                        0x50
#define CTRL1_ODR_200_HZ                        0x60
#define CTRL1_ODR_400_200_HZ                    0x70
#define CTRL1_ODR_800_200_HZ                    0x80
#define CTRL1_ODR_1600_200_HZ                   0x90
#define CTRL1_MODE_LP                           0x00
#define CTRL1_MODE_HP                           0x04
#define CTRL1_MODE_ON_DEMAND                    0x08
#define CTRL1_LP_MODE_1                         0x00
#define CTRL1_LP_MODE_2                         0x01
#define CTRL1_LP_MODE_3                         0x02
#define CTRL1_LP_MODE_4                         0x03

/** 
 * CTRL2 REGISTER (21h)
 */
#define CTRL2_BOOT                              0x80
#define CTRL2_SOFT_RESET                        0x40
#define CTRL2_CS_PU_DISC                        0x10
#define CTRL2_BDU                               0x08
#define CTRL2_IF_ADD_INC                        0x04
#define CTRL2_I2C_DISABLE                       0x02
#define CTRL2_SIM                               0x01

/** 
 * CTRL3 REGISTER (22h)
 */
#define CTRL3_SELF_TEST_NORMAL                  0x00
#define CTRL3_SELF_TEST_POSITIVE                0x40
#define CTRL3_SELF_TEST_NEGATIVE                0x80
#define CTRL3_PP_OD                             0x20
#define CTRL3_LIR                               0x10
#define CTRL3_H_LACTIVE                         0x08
#define CTRL3_SLP_MODE_SEL                      0x02
#define CTRL3_SLP_MODE_1                        0x01

/** 
 * CTRL4_INT1_PAD_CTRL REGISTER (23h)
 */
#define CTRL4_INT1_PAD_CTRL_INT1_6D             0x80
#define CTRL4_INT1_PAD_CTRL_INT1_SINGLE_TAP     0x40
#define CTRL4_INT1_PAD_CTRL_INT1_WU             0x20
#define CTRL4_INT1_PAD_CTRL_INT1_FF             0x10
#define CTRL4_INT1_PAD_CTRL_INT1_TAP            0x08
#define CTRL4_INT1_PAD_CTRL_INT1_DIFF5          0x04
#define CTRL4_INT1_PAD_CTRL_INT1_FTH            0x02
#define CTRL4_INT1_PAD_CTRL_INT1_DRDY           0x01

/** 
 * CTRL5_INT2_PAD_CTRL REGISTER (24h)
 */
#define CTRL5_INT2_PAD_CTRL_INT2_SLEEP_STATE    0x80
#define CTRL5_INT2_PAD_CTRL_INT2_SLEEP_CHG      0x40
#define CTRL5_INT2_PAD_CTRL_INT2_BOOT           0x20
#define CTRL5_INT2_PAD_CTRL_INT2_DRDY_T         0x10
#define CTRL5_INT2_PAD_CTRL_INT2_OVR            0x08
#define CTRL5_INT2_PAD_CTRL_INT2_DIFF5          0x04
#define CTRL5_INT2_PAD_CTRL_INT2_FTH            0x02
#define CTRL5_INT2_PAD_CTRL_INT2_DRDY           0x01

/** 
 * CTRL6 REGISTER (25h)
 */
#define CTRL6_BW_FILT_ODR_2                     0x00
#define CTRL6_BW_FILT_ODR_4                     0x40
#define CTRL6_BW_FILT_ODR_10                    0x80
#define CTRL6_BW_FILT_ODR_20                    0xC0
#define CTRL6_FS_2G                             0x00
#define CTRL6_FS_4G                             0x10
#define CTRL6_FS_8G                             0x20
#define CTRL6_FS_16G                            0x30
#define CTRL6_FDS                               0x08
#define CTRL6_LOW_NOISE                         0x04

/** 
 * STATUS REGISTER (27h)
 */
#define STATUS_FIFO_THS                         0x80
#define STATUS_WU_IA                            0x40
#define STATUS_SLEEP_STATE                      0x20
#define STATUS_DOUBLE_TAP                       0x10
#define STATUS_SINGLE_TAP                       0x08
#define STATUS_6D_IA                            0x04
#define STATUS_FF_IA                            0x02
#define STATUS_DRDY                             0x01

/** 
 * FIFO_CTRL REGISTER (2Eh)
 */
#define FIFO_CTRL_FMODE_BYPASS                  0x00
#define FIFO_CTRL_FMODE_FIFO                    0x20
#define FIFO_CTRL_FMODE_CONTINUOUS_TO_FIFO      0x60
#define FIFO_CTRL_FMODE_BYPASS_TO_CONTINUOUS    0x80
#define FIFO_CTRL_FMODE_CONTINUOUS              0xC0
#define FIFO_CTRL_FTH                           0x1F

/** 
 * FIFO_SAMPLES REGISTER (2Fh)
 */
#define FIFO_SAMPLES_FIFO_FTH                   0x80
#define FIFO_SAMPLES_FIFO_OVR                   0x40
#define FIFO_SAMPLES_DIFF                       0x3F

/** 
 * TAP_THS_X REGISTER (30h)
 */
#define TAP_THS_X_4D_EN                         0x80
#define TAP_THS_X_6D_THS                        0x60
#define TAP_THS_X_TAP_THSX                      0x1F

/** 
 * TAP_THS_Y REGISTER (31h)
 */
#define TAP_THS_Y_TAP_PRIOR_XYZ                 0x00
#define TAP_THS_Y_TAP_PRIOR_YXZ                 0x20
#define TAP_THS_Y_TAP_PRIOR_XZY                 0x40
#define TAP_THS_Y_TAP_PRIOR_ZYX                 0x60
#define TAP_THS_Y_TAP_PRIOR_YZX                 0xA0
#define TAP_THS_Y_TAP_PRIOR_ZXY                 0xC0
#define TAP_THS_Y_TAP_THSY                      0x1F

/** 
 * TAP_THS_Z REGISTER (32h)
 */
#define TAP_THS_Z_TAP_X_EN                      0x80
#define TAP_THS_Z_TAP_Y_EN                      0x40
#define TAP_THS_Z_TAP_Z_EN                      0x20
#define TAP_THS_Z_TAP_THSZ                      0x1F

/** 
 * INT_DUR REGISTER (33h)
 */
#define INT_DUR_LATENCY                         0xF0
#define INT_DUR_QUIET                           0x0C
#define INT_DUR_SHOCK                           0x03

/** 
 * WAKE_UP_THS REGISTER (34h)
 */
#define WAKE_UP_THS_SINGLE_DOUBLE_TAP           0x80
#define WAKE_UP_THS_SLEEP_ON                    0x40
#define WAKE_UP_THS_WK_THS                      0x3F

/**
 * WAKE_UP_DUR REGISTER (35h)
 */
#define WAKE_UP_DUR_FF_DUR5                     0x80
#define WAKE_UP_DUR_WAKE_DUR                    0x60
#define WAKE_UP_DUR_STATIONARY                  0x10
#define WAKE_UP_DUR_SLEEP_DUR                   0x0F

/**
 * FREE_FALL REGISTER (36h)
 */
#define FREE_FALL_FF_DUR                        0xF8
#define FREE_FALL_FF_THS                        0x07

/**
 * STATUS_DUP REGISTER (37h)
 */
#define STATUS_DUP_OVR                          0x80
#define STATUS_DUP_DRDY_T                       0x40
#define STATUS_DUP_SLEEP_STATE_IA               0x20
#define STATUS_DUP_DOUBLE_TAP                   0x10
#define STATUS_DUP_SINGLE_TAP                   0x08
#define STATUS_DUP_6D_IA                        0x04
#define STATUS_DUP_FF_IA                        0x02
#define STATUS_DUP_DRDY                         0x01

/**
 * WAKE_UP_SRC REGISTER (38h)
 */
#define WAKE_UP_SRC_FF_IA                       0x20
#define WAKE_UP_SRC_SLEEP_STATE_IA              0x10
#define WAKE_UP_SRC_WU_IA                       0x08
#define WAKE_UP_SRC_X_WU                        0x04
#define WAKE_UP_SRC_Y_WU                        0x02
#define WAKE_UP_SRC_Z_WU                        0x01

/**
 * TAP_SRC REGISTER (39h)
 */
#define TAP_SRC_TAP_IA                          0x40
#define TAP_SRC_SINGLE_TAP                      0x20
#define TAP_SRC_DOUBLE_TAP                      0x10
#define TAP_SRC_TAP_SIGN                        0x08
#define TAP_SRC_X_TAP                           0x04
#define TAP_SRC_Y_TAP                           0x02
#define TAP_SRC_Z_TAP                           0x01

/**
 * SIXD_SRC REGISTER (3Ah)
 */
#define SIXD_SRC_6D_IA                          0x40
#define SIXD_SRC_ZH                             0x20
#define SIXD_SRC_ZL                             0x10
#define SIXD_SRC_YH                             0x08
#define SIXD_SRC_YL                             0x04
#define SIXD_SRC_XH                             0x02
#define SIXD_SRC_XL                             0x01

/**
 * ALL_INT_SRC REGISTER (3Bh)
 */
#define ALL_INT_SRC_SLEEP_CHANGE_IA             0x20
#define ALL_INT_SRC_6D_IA                       0x10
#define ALL_INT_SRC_DOUBLE_TAP                  0x08
#define ALL_INT_SRC_SINGLE_TAP                  0x04
#define ALL_INT_SRC_WU_IA                       0x02
#define ALL_INT_SRC_FF_IA                       0x01

/**
 * CTRL7 REGISTER (3Fh)
 */
#define CTRL7_DRDY_PULSED                       0x80
#define CTRL7_INT2_ON_INT1                      0x40
#define CTRL7_INTERRUPTS_ENABLE                 0x20
#define CTRL7_USR_OFF_ON_OUT                    0x10
#define CTRL7_USR_OFF_ON_WU                     0x08
#define CTRL7_USR_OFF_W                         0x04
#define CTRL7_HP_REF_MODE                       0x02
#define CTRL7_LPASS_ON6D                        0x01

/**
 * FUNCTION DECLARATIONS
 */

void lis2dtw12_reg_read(uint8_t reg, void * data, uint8_t length);
void lis2dtw12_reg_write(uint8_t reg, uint8_t data);

uint8_t lis2dtw12_get_device_id();
void lis2dtw12_init(uint8_t odr, uint8_t mode, uint8_t lp_mode, uint8_t bw_filt, uint8_t fs, uint8_t fds, uint8_t low_noise);

#endif  /* __LIS2DTW12_H__ */