/**
 * @file device.h
 * @author Ben Owen (ben@embeddedshark.com)
 * @brief Header file for device.c
 * 
 * @date 2024-10-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef __DEVICE_H__
#define __DEVICE_H__

#include <zephyr/drivers/gpio.h>

/* ENUMERATION DEFINITIONS */

typedef enum
{
    DEVICE_STATE_POWEROFF,
    DEVICE_STATE_WAKEUP,
    DEVICE_STATE_RUN,
}
device_state_t;

typedef enum
{
    DEVICE_RESET_SRC_GPIO_WAKEUP,
    DEVICE_RESET_SRC_CPU_LOCKUP,
    DEVICE_RESET_SRC_SOFT_RESET,
    DEVICE_RESET_SRC_WATCHDOG,
    DEVICE_RESET_SRC_RESET_PIN,
    DEVICE_RESET_SRC_OTHER,
}
device_reset_src_t;

/**
 * FUNCTION DECLARATIONS
 */

/**
 * @brief Get the device state
 * 
 */
device_state_t device_get_state(void);

/**
 * @brief Change the device state
 * 
 * @param new_state 
 */
void device_set_state(device_state_t new_state);

/**
 * @brief Device initialization when waking from sleep
 * 
 */
void device_wakeup(void);

/**
 * @brief Turn the device off
 *          Handles cleanup before shutting off
 * 
 */
void device_poweroff();

/**
 * @brief Get the device wakeup reason
 * 
 * @return device_reset_src_t 
 */
device_reset_src_t device_get_reset_src(void);

#endif  /* __DEVICE_H__ */