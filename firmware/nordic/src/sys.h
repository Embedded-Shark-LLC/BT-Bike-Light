/**
 * @file sys.h
 * @author Ben Owen (ben@embeddedshark.com)
 * @brief Header file for sys.c
 * 
 * @date 2024-10-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef __SYS_H__
#define __SYS_H__

#include <zephyr/drivers/gpio.h>

/* ENUMERATION DEFINITIONS */

typedef enum
{
    SYS_STATE_POWEROFF,
    SYS_STATE_WAKEUP,
    SYS_STATE_RUN,
}
sys_state_t;

typedef enum
{
    SYS_RESET_SRC_GPIO_WAKEUP,
    SYS_RESET_SRC_CPU_LOCKUP,
    SYS_RESET_SRC_SOFT_RESET,
    SYS_RESET_SRC_WATCHDOG,
    SYS_RESET_SRC_RESET_PIN,
    SYS_RESET_SRC_OTHER,
}
sys_reset_src_t;

/**
 * LOCAL VARIABLES
 */

extern sys_state_t sys_state;

/**
 * FUNCTION DECLARATIONS
 */

/**
 * @brief Turn the system off
 *          Handles cleanup before shutting off
 * 
 * @param button_dt 
 */
void sys_power_off(const struct gpio_dt_spec * button_dt);

/**
 * @brief Get the system wakeup reason
 * 
 * @return sys_reset_src_t 
 */
sys_reset_src_t sys_get_reset_src(void);

#endif  /* __SYS_H__ */