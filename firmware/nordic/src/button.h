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

#define BUTTON_POLL_HZ              20

/**
 * @brief This thread reads button inputs and handles short/long presses
 * 
 */
void button_thread(void);

/**
 * @brief Initialize all peripherals for interface button
 * 
 */
void button_init(void);

#endif  /* __BUTTON_H__ */