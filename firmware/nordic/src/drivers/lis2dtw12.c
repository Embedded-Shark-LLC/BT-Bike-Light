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

LOG_MODULE_REGISTER(LIS2DTW12, LOG_LEVEL_INF);

static const struct i2c_dt_spec _i2c_accel = I2C_DT_SPEC_GET(DT_NODELABEL(lis2dtw12));

void lis2dtw12_reg_read(uint8_t reg, void * data, uint8_t length)
{
    int err;
    /* Store register in tx buffer */
    uint8_t _dev_registers[1] = { reg };

    /* Write register address to slave and read data back
        LIS2DTW12 requires a write before read */
    err = i2c_write_read_dt(&_i2c_accel, _dev_registers, 1, data, length);
    __ASSERT(err == 0, "Failed to read I2C device address 0x%02x at register 0x%02x (err: %d)\n\r", _i2c_accel.addr, _dev_registers[0], err);
}

void lis2dtw12_reg_write(uint8_t reg, uint8_t data)
{
    int err;
    /* Store register in tx buffer */
    uint8_t _write_data[2];
    _write_data[0] = reg;
    _write_data[1] = data;

    /* Write register address to slave and read data back */
    err = i2c_write_dt(&_i2c_accel, _write_data, 2);
    __ASSERT(err == 0, "Failed to read I2C device address 0x%02x at register 0x%02x (err: %d)\n\r", _i2c_accel.addr, _dev_registers[0], err);
}

uint8_t lis2dtw12_get_device_id(void)
{
    uint8_t chip_id = 0;
    lis2dtw12_reg_read(LIS2DTW12_WHO_AM_I, &chip_id, 1);
    return chip_id;
}

void lis2dtw12_power_off(void)
{
    /* Put device in power-down
        init() function will need to be called as next function */
    lis2dtw12_reg_write(LIS2DTW12_CTRL1, CTRL1_ODR_POWER_DOWN | CTRL1_MODE_HP);
}

void lis2dtw12_reboot(void)
{
    /* Boot procedure
        This ensures that the registers are all reset when the device is reset without power cycling */
    lis2dtw12_reg_write(LIS2DTW12_CTRL2, CTRL2_SOFT_RESET | CTRL2_IF_ADD_INC);
    k_usleep(5);
    lis2dtw12_reg_write(LIS2DTW12_CTRL2, CTRL2_BOOT | CTRL2_IF_ADD_INC);
    k_msleep(20);
}

void lis2dtw12_init(uint8_t odr, uint8_t mode, uint8_t lp_mode, uint8_t bw_filt, uint8_t fs, uint8_t fds, uint8_t low_noise)
{
    /* Make sure I2C bus is ready */
    __ASSERT(device_is_ready(_i2c_accel.bus), "I2C bus is not ready");
    /* Reboot accelerometer */
    lis2dtw12_reboot();
    /* Set desired settings */
    lis2dtw12_reg_write(LIS2DTW12_CTRL1, odr | mode | lp_mode);
    lis2dtw12_reg_write(LIS2DTW12_CTRL6, bw_filt | fs | fds | low_noise);
}