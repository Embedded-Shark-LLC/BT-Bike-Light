/**
 * @file button.h
 * @author Ben Owen (ben@embeddedshark.com)
 * @brief Header file for button.c
 * 
 * @date 2024-10-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef __BUTTON_H__
#define __BUTTON_H__

#define BUTTON_GPIOTE_INSTANCE      0
#define BUTTON_TIMER_INSTANCE       1

#define BUTTON_DEBOUNCE_HOLDOFF_MS  10
#define BUTTON_LONGPRESS_MS         1500

/**
 * @brief Initialize all peripherals for interface button
 * 
 */
void button_init();

#endif  /* __BUTTON_H__ */