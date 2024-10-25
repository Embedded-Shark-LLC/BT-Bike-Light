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
#define CTRL4_INT1_PAD_CTRL 0x23
#define CTRL5_INT2_PAD_CTRL 0x24
#define CTRL6               0x25
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
    int16_t temp;
    int16_t accel_x;
    int16_t accel_y;
    int16_t accel_z;
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

/**
 * @brief Private function to read one or more registers from accelerometer over I2C
 * 
 * @param reg register to read
 * @param data buffer to hold read data
 * @param length number of bytes to read
 */
void _lis2dtw12_reg_read(uint8_t reg, void * data, uint8_t length)
{
    int err;
    /* Store register in tx buffer */
    uint8_t _dev_registers[1] = { reg };

    /* Write register address to slave and read data back
        LIS2DTW12 requires a write before read */
    err = i2c_write_read_dt(&_i2c_accel, _dev_registers, sizeof(_dev_registers), data, length);
    __ASSERT(err == 0, "Failed to read I2C device address 0x%02x at register 0x%02x (err: %d)\n\r", _i2c_accel.addr, _dev_registers[0], err);
}

/**
 * @brief Private function to write to one or more registers to accelerometer over I2C
 * 
 * @param reg register to write
 * @param data register data to write
 * @param length number of bytes to write
 */
void _lis2dtw12_reg_write(uint8_t reg, uint8_t data, uint8_t length)
{
    int err;
    /* Store register in tx buffer */
    uint8_t _write_data[length + 1];
    _write_data[0] = reg;
    _write_data[1] = data;

    /* Write register address to slave and read data back */
    err = i2c_write_dt(&_i2c_accel, _write_data, sizeof(_write_data));
    __ASSERT(err == 0, "Failed to read I2C device address 0x%02x at register 0x%02x (err: %d)\n\r", _i2c_accel.addr, _dev_registers[0], err);
}

void lis2dtw12_init()
{
    /* Make sure I2C bus is ready */
    __ASSERT(device_is_ready(_i2c_accel.bus), "I2C bus is not ready");
    /* Read device ID */
	_lis2dtw12_reg_read(WHO_AM_I, &(_data.chip_id), 1);
    LOG_WRN("Chip ID: 0x%02x", _data.chip_id);

    /* Boot procedure
        This ensures that the registers are all reset when the device is reset without power cycling */
    _lis2dtw12_reg_write(CTRL2, 0b01000000, 1);
    k_usleep(5);
    _lis2dtw12_reg_write(CTRL2, 0b10000000, 1);
    k_msleep(20);

    /* Set up accelerometer control registers */
    /* CTRL1:
        ODR[3:0] = 0011: Low-power mode 25 Hz
        MODE[1:0] = 00: Low-power mode (12/14-bit resolution)
        LP_MODE[1:0] = 00: Low-power mode 1 (12 bit resolution) */
    _lis2dtw12_reg_write(CTRL1, 0b01100100, 1);
    /* CTRL4_INT1_PAD_CTRL:
        INT1_6D = 0: 6D recognition interrupt disabled
        INT1_SINGLE_TAP = 0: Single-tap interrupt disabled
        INT1_WU = 1: Wake-up recognition interrupt enabled on INT1
        INT1_FF = 0: Free-fall recognition interrupt disabled
        INT1_TAP = 0: Double-tap recognition interrupt disabled
        INT1_DIFF5 = 0: FIFO full recognition interrupt disabled
        INT1_FTH = 0: FIFO threshold interrupt disabled
        INT1_DRDY = 0: Data-ready interrupt disabled */
    _lis2dtw12_reg_write(CTRL4_INT1_PAD_CTRL, 0b00100000, 1);
    /* CTRL5_INT2_PAD_CTRL:
        INT2_SLEEP_STATE = 1: Enable routing SLEEP_STATE to INT2
        INT2_SLEEP_CHG = 1: Sleep change status routed to INT2
        INT2_BOOT = 0: Boot state indication disabled
        INT2_DRDY_T = 0: Temperature data-ready interrupt disabled
        INT2_OVR = 0: FIFO overrun interrupt disabled
        INT2_DIFF5 = 0: FIFO full recognition interrupt disabled
        INT2_FTH = 0: FIFO threshold interrupt disabled
        INT2_DRDY = 0: Data-ready interrupt disabled */
    _lis2dtw12_reg_write(CTRL5_INT2_PAD_CTRL, 0b11000000, 1);
    _lis2dtw12_reg_write(CTRL6, 0b01001100, 1); // TODO ODR/4, +- 2g, low pass, low noise
    /* WAKE_UP_THS:
        SINGLE_DOUBLE_TAP = 0: Disable single/double-tap event
        SLEEP_ON = 1: Enable sleep (inactivity)
        WK_THS[5:0] = 000010: Wake-up threshold (62.5 mg for FS of 2 g) */
    _lis2dtw12_reg_write(WAKE_UP_THS, 0b01001000, 1); // TODO need to prevent waking up when rotated on another axis
    /* WAKE_UP_DUR:
        FF_DUR5 = 0: Free-fall duration 0
        WAKE_DUR[1:0] = 10: Wake-up duration = 0.01 seconds
        STATIONARY = 0: Stationary detection without ODR change disabled
        SLEEP_DUR[3:0] = 0010: Sleep duration of 10.24 seconds */
    _lis2dtw12_reg_write(WAKE_UP_DUR, 0b01000010, 1);
    /* CTRL7:
        DRDY_PULSED = 0: Use latched mode for DR interrupt
        INT2_ON_INT1 = 0: Don't route INT2 interrupts to INT1
        INTERRUPTS_ENABLE = 1: Enable interrupts
        USR_OFF_ON_OUT = 0: Disable user offset value in ODRs
        USR_OFF_ON_WU = 0: Disable user offset value on wake-up
        USR_OFF_W = 0: No user offset weight
        HP_REF_MODE = 1: Enable high-pass filter reference mode
        LPASS_ON6D = 0: ODR/2 low-pass filtered data sent to 6D interrupt function */
    _lis2dtw12_reg_write(CTRL7, 0b00100010, 1);
    _lis2dtw12_reg_read(ALL_INT_SRC, &(_data.accel_x), 1);
    /* Wait for device to poll some data */
    k_msleep(10);

    /* Read temperature */
    _lis2dtw12_reg_read(OUT_T_L, &(_data.temp), 2);
    /* Convert temperature to celsius (integer, no decimals) */
    _data.temp >>= 8;   // 16 LSB per C, need to shift 4 additional since its 12-bit left-aligned
    _data.temp += 25;   // 0 LSB = 25 C
}