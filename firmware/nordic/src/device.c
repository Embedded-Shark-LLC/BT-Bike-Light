/**
 * @file device.c
 * @author Ben Owen (ben@embeddedshark.com)
 * @brief File containing global device-level functions
 *          Also contains global device-level state variables
 * 
 * @date 2024-10-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "device.h"

#include "button.h"
#include "led.h"

#include <drivers/lis2dtw12.h>

#include <nrfx_gpiote.h>
#include <nrfx_timer.h>

#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/poweroff.h>

LOG_MODULE_REGISTER(DEVICE, LOG_LEVEL_INF);

/**
 * LOCAL VARIABLES
 */

static device_state_t _device_state;

/**
 * FUNCTION DEFINITIONS
 */

device_state_t device_get_state(void)
{
    return _device_state;
}

void device_set_state(device_state_t new_state)
{
    _device_state = new_state;
}

void device_wakeup(void)
{
    /* Set boot LED pattern */
    led_set_pattern(LED_PATTERN_PULSE);
    /* Set devicetem state */
    device_set_state(DEVICE_STATE_WAKEUP);
}

void device_poweroff(const struct gpio_dt_spec * button_dt)
{
    nrfx_err_t err;

    /* Change device state */
    device_set_state(DEVICE_STATE_POWEROFF);
    /* Clear LED */
    led_set_pattern(LED_PATTERN_OFF);
    /* Turn off accelerometer */
    lis2dtw12_power_off();  // TODO not working since called from interrupt function
    /* Wait until button released */
    while (gpio_pin_get_dt(button_dt));
    k_msleep(250);  // delay a small period for button bouncing
    /* Configure interrupt for button (wakeup source) */
    err = gpio_pin_interrupt_configure_dt(button_dt, GPIO_INT_LEVEL_ACTIVE);
    NRFX_ASSERT(err == 0);
    /* Put microcontroller to sleep */
    LOG_WRN("Powering devicetem off");
    sys_poweroff();
}

device_reset_src_t device_get_reset_src(void)
{
    /* Get reset source */
    uint32_t reset_src = NRF_POWER->RESETREAS;
    /* Clear reset source register */
    NRF_POWER->RESETREAS = 0xFFFFFFFF;

    /* Determine reset reason */
    if (reset_src & POWER_RESETREAS_OFF_Msk)
    {
        return DEVICE_RESET_SRC_GPIO_WAKEUP;
    }
    else if (reset_src & POWER_RESETREAS_LOCKUP_Msk)
    {
        return DEVICE_RESET_SRC_CPU_LOCKUP;
    }
    else if (reset_src & POWER_RESETREAS_SREQ_Msk)
    {
        return DEVICE_RESET_SRC_SOFT_RESET;
    }
    else if (reset_src & POWER_RESETREAS_DOG_Msk)
    {
        return DEVICE_RESET_SRC_WATCHDOG;
    }
    else if (reset_src & POWER_RESETREAS_RESETPIN_Msk)
    {
        return DEVICE_RESET_SRC_RESET_PIN;
    }
    else
    {
        return DEVICE_RESET_SRC_OTHER;
    }
}