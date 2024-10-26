/**
 * @file button.c
 * @author your name (you@domain.com)
 * @brief File containing interface button logic
 * 
 * @date 2024-10-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "button.h"

#include "device.h"
#include "led.h"

#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(BUTTON, LOG_LEVEL_INF);

/* GPIO */
static const struct gpio_dt_spec _button_dt = GPIO_DT_SPEC_GET(DT_NODELABEL(user_gpio), button_gpios);

const struct gpio_dt_spec * button_get_dt_spec(void)
{
    return &_button_dt;
}

void button_thread(void)
{
    /* The button buffer works by shifting left and ORing the button state
        If the buffer is all 1s, then a long press occurred
        If the buffer is some 1s followed by a 0, then a press was released */
    uint32_t button_buffer = 0;

    while (1)
    {
        /* Place button state into buffer */
        button_buffer = (button_buffer << 1) | gpio_pin_get_dt(&_button_dt);
        /* Check for button events */
        if (button_buffer == 0x7FFFFFFF)
        {
            /* Long press occurred
                We use 0x7FFFFFFF so that it won't constantly detect long presses if still held */
            LOG_DBG("Long press");
            if (device_get_state() == DEVICE_STATE_POWEROFF)
            {
                /* Wake up device */
                device_wakeup();
            }
            else if (device_get_state() == DEVICE_STATE_RUN)
            {
                /* Power off device if running */
                device_poweroff();
            }
        }
        else if (button_buffer == 0xFFFFFFFE)
        {
            /* Long press released */
            LOG_DBG("Long press released");
        }
        else if ((button_buffer & 0x3) == 0x2)
        {
            /* Short press released */
            LOG_DBG("Short press released");
            if (device_get_state() == DEVICE_STATE_POWEROFF)
            {
                /* Button not held long enough, go back to sleep */
                device_poweroff();
            }
            else if (device_get_state() == DEVICE_STATE_RUN)
            {
                /* Toggle LED pattern */
                led_toggle_pattern();
            }
        }

        /* Delay until next poll interval */
        k_msleep(1000 / BUTTON_POLL_HZ);
    }
}

void button_init()
{
    int err;

    /* GPIO config */
    /* Initialize GPIO */
	__ASSERT(device_is_ready(_button_dt.port), "Button port not ready");
    /* Configure button as input pin */
	err = gpio_pin_configure_dt(&_button_dt, GPIO_INPUT);
	__ASSERT(err == 0, "Error configuring button as input");
}