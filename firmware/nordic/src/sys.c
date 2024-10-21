/**
 * @file sys.c
 * @author Ben Owen (ben@embeddedshark.com)
 * @brief File containing global system-level functions
 *          Also contains global system-level state variables
 * 
 * @date 2024-10-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "sys.h"

#include "button.h"
#include "led.h"

#include <nrfx_gpiote.h>
#include <nrfx_timer.h>

#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/poweroff.h>

LOG_MODULE_REGISTER(SYS, LOG_LEVEL_INF);

/**
 * LOCAL VARIABLES
 */

sys_state_t sys_state = SYS_STATE_POWEROFF;

/**
 * FUNCTION DEFINITIONS
 */

void sys_power_off(const struct gpio_dt_spec * button_dt)
{
    nrfx_err_t err;

    /* Clear LED */
    led_set_pattern(LED_PATTERN_OFF);
    /* Wait until button released */
    while (gpio_pin_get_dt(button_dt));
    k_msleep(250);  // delay a small period for button bouncing
    /* Configure interrupt for button (wakeup source) */
    err = gpio_pin_interrupt_configure_dt(button_dt, GPIO_INT_LEVEL_ACTIVE);
    NRFX_ASSERT(err == 0);
    /* Put microcontroller to sleep */
    LOG_WRN("Powering system off");
    sys_poweroff();
}

sys_reset_src_t sys_get_reset_src(void)
{
    /* Get reset source */
    uint32_t reset_src = NRF_POWER->RESETREAS;
    /* Clear reset source register */
    NRF_POWER->RESETREAS = 0xFFFFFFFF;

    /* Determine reset reason */
    if (reset_src & POWER_RESETREAS_OFF_Msk)
    {
        return SYS_RESET_SRC_GPIO_WAKEUP;
    }
    else if (reset_src & POWER_RESETREAS_LOCKUP_Msk)
    {
        return SYS_RESET_SRC_CPU_LOCKUP;
    }
    else if (reset_src & POWER_RESETREAS_SREQ_Msk)
    {
        return SYS_RESET_SRC_SOFT_RESET;
    }
    else if (reset_src & POWER_RESETREAS_DOG_Msk)
    {
        return SYS_RESET_SRC_WATCHDOG;
    }
    else if (reset_src & POWER_RESETREAS_RESETPIN_Msk)
    {
        return SYS_RESET_SRC_RESET_PIN;
    }
    else
    {
        return SYS_RESET_SRC_OTHER;
    }
}