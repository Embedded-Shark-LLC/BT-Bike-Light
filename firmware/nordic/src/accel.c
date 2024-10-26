/**
 * @file accel.c
 * @author Ben Owen (ben@embeddedshark.com)
 * @brief File containing setup for accelerometer
 * 
 * @date 2024-10-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "accel.h"

#include "lis2dtw12.h"

#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/types.h>

LOG_MODULE_REGISTER(ACCEL, LOG_LEVEL_INF);

double accel_get_x(void)
{
    /* Read acceleration */
    int16_t accel_x = 0;
    lis2dtw12_reg_read(LIS2DTW12_OUT_X_L, &accel_x, 2);
    /* 14-bit = 0.244 mg/digit */
    double accel_x_converted = accel_x * 0.000244;

    return accel_x_converted;
}

double accel_get_y(void)
{
    /* Read acceleration */
    int16_t accel_y = 0;
    lis2dtw12_reg_read(LIS2DTW12_OUT_Y_L, &accel_y, 2);
    /* 14-bit = 0.244 mg/digit */
    double accel_y_converted = accel_y * 0.000244;

    return accel_y_converted;
}

double accel_get_z(void)
{
    /* Read acceleration */
    int16_t accel_z = 0;
    lis2dtw12_reg_read(LIS2DTW12_OUT_Z_L, &accel_z, 2);
    /* 14-bit = 0.244 mg/digit */
    double accel_z_converted = accel_z * 0.000244;

    return accel_z_converted;
}

int16_t accel_get_temp(void)
{
    /* Read temperature */
    int16_t temperature = 0;
    lis2dtw12_reg_read(LIS2DTW12_OUT_T_L, &temperature, 2);
    /* Convert temperature to celsius (integer, no decimals) */
    temperature >>= 8;  // 16 LSB per C, need to shift 4 additional since its 12-bit left-aligned
    temperature += 25;  // 0 LSB = 25 C
    
    return temperature;
}

/**
 * @brief Initialize accelerometer
 * 
 */
void accel_init(void)
{
    /* Make sure device is responding */
    __ASSERT(lis2dtw12_get_device_id() == DEVICE_ID, "Acclerometer not responding on I2C bus");
    /* Initialize accelerometer with desired settings */
    lis2dtw12_init(CTRL1_ODR_25_HZ, CTRL1_MODE_HP, CTRL1_LP_MODE_2, CTRL6_BW_FILT_ODR_2, CTRL6_FS_2G, CTRL6_FDS, 0);
    /* Set INT1 pin configuration */
    lis2dtw12_reg_write(LIS2DTW12_CTRL4_INT1_PAD_CTRL, CTRL4_INT1_PAD_CTRL_INT1_WU);
    /* Set INT2 pin configuration */
    lis2dtw12_reg_write(LIS2DTW12_CTRL5_INT2_PAD_CTRL, CTRL5_INT2_PAD_CTRL_INT2_SLEEP_STATE | CTRL5_INT2_PAD_CTRL_INT2_SLEEP_CHG);
    /* Set wake up threshold */
    lis2dtw12_reg_write(LIS2DTW12_WAKE_UP_THS, WAKE_UP_THS_SLEEP_ON | (0b001000 & WAKE_UP_THS_WK_THS));  // should be 0.25 g
    /* Set wake up/sleep duration
        WAKE UP:
            At 25 Hz, each LSB = 0.04 seconds
            Maximum duration is 0b11 = 0.12 seconds
        SLEEP:
            At 25 Hz, each LSB = 20.48 seconds
            Maximum duration is 0b1111 ~= 300 seconds (5 minutes) */
    lis2dtw12_reg_write(LIS2DTW12_WAKE_UP_DUR, (0b01 & WAKE_UP_DUR_WAKE_DUR) | (0b1111 & WAKE_UP_DUR_SLEEP_DUR));  // should be 0.08 second wake up duration, 20.48 second sleep duration
    /* Set CTRL7 fields to enable interrupts and filter out gravity */
    lis2dtw12_reg_write(LIS2DTW12_CTRL7, CTRL7_INTERRUPTS_ENABLE | CTRL7_HP_REF_MODE);
    /* Allow device time to poll some data */
    k_msleep(100);
}