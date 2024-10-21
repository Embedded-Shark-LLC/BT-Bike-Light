/**
 * @file led.h
 * @author Ben Owen (ben@embeddedshark.com)
 * @brief Header file for led.c
 * 
 * @date 2024-10-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef __LED_H__
#define __LED_H__

#define LED_GPIOTE_INSTANCE 0
#define LED_PWM_INSTANCE    1
#define LED_TIMER_INSTANCE  0

typedef enum
{
    LED_PATTERN_BRIGHT_BLINK    = 0,
    LED_PATTERN_DIM_BLINK       = 1,
    LED_PATTERN_BRIGHT_SOLID    = 2,
    LED_PATTERN_DIM_SOLID       = 3,
    LED_PATTERN_PULSE           = 4,
    LED_PATTERN_NUM_PATTERNS,   // TOTAL NUMBER OF PATTERNS TO CYCLE
    LED_PATTERN_OFF,            // WILL NOT CYCLE TO OFF
}
led_pattern_t;

#endif  /* __LED_H__ */

/**
 * @brief Initialize all peripherals for LED control
 * 
 */
void led_init();

/**
 * @brief Set blink pattern of LED
 * 
 * @param pattern 
 */
void led_set_pattern(led_pattern_t pattern);

/**
 * @brief Toggle blink pattern (cycle through all patterns)
 * 
 */
void led_toggle_pattern();