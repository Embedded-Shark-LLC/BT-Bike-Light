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
static struct gpio_callback _button_cb_data;

/**
 * @brief Callback for button pressed interrupt
 * 
 * @param dev 
 * @param cb 
 * @param pins 
 */
static void _button_callback(const struct device * port, struct gpio_callback * cb, gpio_port_pins_t pins)
{
    /* Nothing to do here, thread will handle button events */
    return;
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
                device_poweroff(&_button_dt);
            }
        }
        else if (button_buffer == 0xFFFFFFFE)
        {
            /* Long press released */
            LOG_DBG("Long press released");
            /* Change device state */
            if (device_get_state() == DEVICE_STATE_WAKEUP)
            {
                device_set_state(DEVICE_STATE_RUN);
            }
        }
        else if ((button_buffer & 0x3) == 0x2)
        {
            /* Short press released */
            LOG_DBG("Short press released");
            if (device_get_state() == DEVICE_STATE_POWEROFF)
            {
                /* Button not held long enough, go back to sleep */
                device_poweroff(&_button_dt);
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
    nrfx_err_t err;

    /* GPIO config */
    /* Initialize GPIO */
	NRFX_ASSERT(device_is_ready(_button_dt.port));
    /* Configure button as input pin */
	err = gpio_pin_configure_dt(&_button_dt, GPIO_INPUT);
	NRFX_ASSERT(err == 0);
    /* Configure interrupt for button */
    err = gpio_pin_interrupt_configure_dt(&_button_dt, GPIO_INT_EDGE_BOTH);
    NRFX_ASSERT(err == 0);
    /* Add callback */
	gpio_init_callback(&_button_cb_data, _button_callback, BIT(_button_dt.pin));
	err = gpio_add_callback(_button_dt.port, &_button_cb_data);
    NRFX_ASSERT(err == 0);
}