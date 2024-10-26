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

#include "accel.h"
#include "button.h"
#include "led.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(MAIN, LOG_LEVEL_INF);

/* Define task to handle button events */
K_THREAD_DEFINE(button_task_id, 1024, button_thread, NULL, NULL, NULL, 4, 0, 0);

/**
 * @brief Application entry point
 * 
 * @return int 
 */
int main(void)
{
    /* Initialize drivers */
    led_init();
    accel_init();
    button_init();  // Needs to be last to enable all other wakeup sources before sleeping

    return 0;
}
