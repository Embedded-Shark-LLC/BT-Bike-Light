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
#include "device.h"
#include "led.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <nrfx_gpiote.h>

LOG_MODULE_REGISTER(MAIN, LOG_LEVEL_DBG);

/* Define task to handle button events */
#define BUTTON_THREAD_PRIORITY  4
K_THREAD_DEFINE(button_task_id, 1024, button_thread, NULL, NULL, NULL, BUTTON_THREAD_PRIORITY, 0, K_TICKS_FOREVER);  // Delay start to allow GPIO time to initialize

/**
 * @brief Application entry point
 * 
 * @return int 
 */
int main(void)
{
    /* Initialize peripherals */
    NRF_GPIO->DETECTMODE = GPIO_DETECTMODE_DETECTMODE_LDETECT;
    
    /* Get reset source */
    device_reset_src_t reset_src = device_get_reset_src();

    /* Initialize drivers */
    led_init();
    button_init();  // Needs to be last to enable all other wakeup sources before sleeping

    /* Start threads */
    k_thread_start(button_task_id);

    /* Put device to sleep if reset from flashing */
    if (reset_src == DEVICE_RESET_SRC_RESET_PIN)
    {
        device_poweroff();
    }

    return 0;
}
