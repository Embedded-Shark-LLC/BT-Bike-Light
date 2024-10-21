/**
 * @file main.c
 * @author Ben Owen (ben@embeddedshark.com)
 * @brief Application entry file
 * 
 * @date 2024-10-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "button.h"
#include "led.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(MAIN, LOG_LEVEL_INF);

/**
 * @brief Application entry point
 * 
 * @return int 
 */
int main(void)
{
    /* Initialize drivers */
    led_init();
    button_init();

    /* Set boot pattern */
    led_set_pattern(LED_PATTERN_PULSE);

    return 0;
}
