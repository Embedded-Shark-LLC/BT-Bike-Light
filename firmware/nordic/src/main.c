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

#include "led.h"

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

    /* Set pattern */
    // led_set_pattern(LED_PATTERN_BRIGHT_BLINK);
    // led_set_pattern(LED_PATTERN_DIM_BLINK);
    // led_set_pattern(LED_PATTERN_BRIGHT_SOLID);
    // led_set_pattern(LED_PATTERN_DIM_SOLID);
    led_set_pattern(LED_PATTERN_PULSE);

    return 0;
}
