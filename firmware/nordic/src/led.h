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
    LED_PATTERN_BRIGHT_BLINK,
    LED_PATTERN_DIM_BLINK,
    LED_PATTERN_BRIGHT_SOLID,
    LED_PATTERN_DIM_SOLID,
    LED_PATTERN_PULSE,
    LED_PATTERN_OFF,
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