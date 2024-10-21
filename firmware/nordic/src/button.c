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
#include "led.h"

#include <nrfx_gpiote.h>
#include <nrfx_ppi.h>
#include <nrfx_timer.h>

#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/poweroff.h>

#if (BUTTON_TIMER_INSTANCE == 0)
#define BUTTON_TIMER_IRQN       TIMER0_IRQn
#elif (BUTTON_TIMER_INSTANCE == 1)
#define BUTTON_TIMER_IRQN       TIMER1_IRQn
#elif (BUTTON_TIMER_INSTANCE == 2)
#define BUTTON_TIMER_IRQN       TIMER2_IRQn
#elif (BUTTON_TIMER_INSTANCE == 3)
#define BUTTON_TIMER_IRQN       TIMER3_IRQn
#elif (BUTTON_TIMER_INSTANCE == 4)
#define BUTTON_TIMER_IRQN       TIMER4_IRQn
#endif

LOG_MODULE_REGISTER(BUTTON, LOG_LEVEL_INF);

/* GPIO */
static const struct gpio_dt_spec _button_dt = GPIO_DT_SPEC_GET(DT_NODELABEL(user_gpio), button_gpios);
static struct gpio_callback _button_cb_data;
static nrfx_gpiote_t _gpiote = NRFX_GPIOTE_INSTANCE(BUTTON_GPIOTE_INSTANCE);
#define BUTTON_PIN DT_GPIO_PIN(DT_NODELABEL(user_gpio), button_gpios)

/* Timer */
static nrfx_timer_t _timer_button = NRFX_TIMER_INSTANCE(BUTTON_TIMER_INSTANCE);

/**
 * @brief Callback for button pressed interrupt
 * 
 * @param dev 
 * @param cb 
 * @param pins 
 */
void _button_callback(const struct device * port, struct gpio_callback * cb, gpio_port_pins_t pins)
{
    bool _button_pressed = gpio_pin_get_dt(&_button_dt);
    if (_button_pressed)
    {
        /* Need to start a timer to see if this is a long press or a short press */
        nrfx_timer_clear(&_timer_button);
        nrfx_timer_enable(&_timer_button);
    }
    else
    {
        /* Button released before timer expired (short press) */
        LOG_DBG("SHORT PRESS");
        /* Disable timer to avoid long press logic triggering */
        nrfx_timer_disable(&_timer_button);
        /* If we made it here, this is a short press */
        led_toggle_pattern();
    }
}

void _timer_callback(nrf_timer_event_t event_type, void * p_context)
{
    nrfx_err_t err;

    if (event_type == NRF_TIMER_EVENT_COMPARE0)
    {
        /* Timer expired (long press) */
        LOG_DBG("LONG PRESS");
        /* Clear LED */
        led_set_pattern(LED_PATTERN_OFF);
        /* Wait until button released */
        while (gpio_pin_get_dt(&_button_dt));
        k_msleep(250);  // delay a small period for button bouncing
        /* Configure interrupt for button (wakeup source) */
        err = gpio_pin_interrupt_configure_dt(&_button_dt, GPIO_INT_LEVEL_ACTIVE);
        NRFX_ASSERT(err == 0);
        /* Put microcontroller to sleep */
        sys_poweroff();
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

    /* PPI config */
    /* Allocate PPI channels */
    nrf_ppi_channel_t _ppi_timer_expire_ch;
    err = nrfx_ppi_channel_alloc(&_ppi_timer_expire_ch);
    NRFX_ASSERT(err == NRFX_SUCCESS);
    /* Assign PPI channels to disable LED driver if timer expires */
    err = nrfx_ppi_channel_assign(_ppi_timer_expire_ch, nrfx_timer_event_address_get(&_timer_button, NRF_TIMER_EVENT_COMPARE0), nrfx_gpiote_clr_task_address_get(&_gpiote, 12));
    NRFX_ASSERT(err == NRFX_SUCCESS);
    /* Enable PPI channels */
    err = nrfx_ppi_channel_enable(_ppi_timer_expire_ch);
    NRFX_ASSERT(err == NRFX_SUCCESS);

    /* Timer config */
    /* Configure timer parameters */
    nrfx_timer_config_t _timer_button_config =
    {
        .frequency          = NRFX_MHZ_TO_HZ(1),
        .mode               = NRF_TIMER_MODE_TIMER,
        .bit_width          = NRF_TIMER_BIT_WIDTH_32,
        .interrupt_priority = NRFX_TIMER_DEFAULT_CONFIG_IRQ_PRIORITY,
        .p_context          = NULL,
    };
    /* Initialize timer */
    err = nrfx_timer_init(&_timer_button, &_timer_button_config, _timer_callback);
    NRFX_ASSERT(err == NRFX_SUCCESS);
    /* Setup compare channels */
    nrfx_timer_compare(&_timer_button, NRF_TIMER_CC_CHANNEL0, nrfx_timer_ms_to_ticks(&_timer_button, BUTTON_LONGPRESS_MS), true);
    /* Needed to handle timer compare interrupts */
    IRQ_CONNECT(BUTTON_TIMER_IRQN, 0, NRFX_TIMER_INST_HANDLER_GET(BUTTON_TIMER_INSTANCE), NULL, 0);  // priority must be higher than GPIOTE to avoid button release event immediately waking up device
}