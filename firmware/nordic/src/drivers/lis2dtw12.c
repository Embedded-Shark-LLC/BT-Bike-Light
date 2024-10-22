/**
 * @file lis2dtw12.c
 * @author Ben Owen (ben@embeddedshark.com)
 * @brief Driver for LIS2DTW12 accelerometer/temperature sensor
 * 
 * @date 2024-10-21
 * 
 * @copyright Copyright (c) 2024 Embedded Shark, LLC
 * 
 */

#include "lis2dtw12.h"

#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/types.h>

/* STEP 4 - Define the driver compatible from the custom binding */

LOG_MODULE_REGISTER(LIS2DTW12, LOG_LEVEL_INF);

/**
 * REGISTER LOCATIONS
 */
#define OUT_T_L             0x0D    // Temp sensor output
#define OUT_T_H             0x0E
#define WHO_AM_I            0x0F    // Who am I ID
#define CTRL1               0x20    // Control registers
#define CTRL2               0x21
#define CTRL3               0x22
#define CTRL4_INT1_PAD_CTRL 0x22
#define CTRL5_INT2_PAD_CTRL 0x22
#define CTRL6               0x22
#define STATUS              0x27    // Status data register
#define OUT_X_L             0x28    // Output registers
#define OUT_X_H             0x29
#define OUT_Y_L             0x2A
#define OUT_Y_H             0x2B
#define OUT_Z_L             0x2C
#define OUT_Z_H             0x2D
#define FIFO_CTRL           0x2E    // FIFO control register
#define FIFI_SAMPLES        0x2F    // Unread samples stored in FIFO
#define TAP_THS_X           0x30    // Tap thresholds
#define TAP_THS_Y           0x31
#define TAP_THS_Z           0x32
#define INT_DUR             0x33    // Interrupt duration
#define WAKE_UP_THS         0x34    // Tap/double tap selection, inactivity enable, wake-up threshold
#define WAKE_UP_DUR         0x35    // Wake-up duration
#define FREE_FALL           0x36    // Free-fall configuration
#define STATUS_DUP          0x37    // Status register
#define WAKE_UP_SRC         0x38    // Wake-up source
#define TAP_SRC             0x39    // Tap source
#define SIXD_SRC            0x3A    // 6D source
#define ALL_INT_SRC         0x3B
#define X_OFS_USR           0x3C
#define Y_OFS_USR           0x3D
#define Z_OFS_USR           0x3E
#define CTRL7               0x3F

#define I2C_ADDR_SA0_H      0b0011001
#define I2C_ADDR_SA0_L      0b0011000
#define DEVICE_ID           0x44    // Value stored in WHO_AM_I

/* Struct to hold LIS2DTW12 data */
struct st_lis2dtw12_data
{
    uint8_t chip_id;
};

struct st_lis2dtw12_data _data;

/* Struct to hold sensor config data */
struct st_lis2dtw12_config
{
    struct i2c_dt_spec i2c;
};

/* I2C */
static const struct i2c_dt_spec _i2c_accel = I2C_DT_SPEC_GET(DT_NODELABEL(lis2dtw12));

void lis2dtw12_reg_read_byte(uint8_t reg, uint8_t * data)
{
    int err;
    /* Store register in tx buffer */
    uint8_t _dev_registers[1] = { reg };

    /* Write register address to slave and read data back */
    err = i2c_write_read_dt(&_i2c_accel, _dev_registers, 1, data, 1);
    __ASSERT(err == 0, "Failed to read I2C device address 0x%02x at register 0x%02x (err: %d)\n\r", _i2c_accel.addr, _dev_registers[0], err);
}

void lis2dtw12_init()
{
    /* Make sure I2C bus is ready */
    __ASSERT(device_is_ready(_i2c_accel.bus), "I2C bus is not ready");
    /* Read device ID */
	lis2dtw12_reg_read_byte(WHO_AM_I, &(_data.chip_id));

    LOG_WRN("Chip ID: 0x%02x", _data.chip_id);
}