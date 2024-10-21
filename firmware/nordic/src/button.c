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
#include "sys.h"

#include <nrfx_timer.h>

#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

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
int64_t _button_press_time_last = 0;

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
    /* Button debounce logic */
    /* Get current timestamp */
    int64_t _button_press_time = k_uptime_get();
    bool _button_bounce = false;
    /* Check for button bounce */
    if ((_button_press_time - _button_press_time_last) < BUTTON_DEBOUNCE_HOLDOFF_MS)
    {
        LOG_WRN("Button bounce detected");
        _button_bounce = true;
    }
    /* Update last pressed timestamp */
    _button_press_time_last = _button_press_time;
    /* Ignore bouncing during wakeup */
    if ((_button_bounce) && (sys_state == SYS_STATE_RUN))
    {
        return;
    }

    /* If we reach this point debouncing has been accounted for */
    /* Get button pressed state */
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
        if (sys_state == SYS_STATE_POWEROFF)
        {
            /* Button press wasn't long enough to wake up, go back to sleep */
            sys_power_off(&_button_dt);
        }
        else if (sys_state == SYS_STATE_WAKEUP)
        {
            /* Button press was long enough to wake up, so we need to ignore short press logic */
            sys_state = SYS_STATE_RUN;
        }
        else
        {
            /* Disable timer to avoid long press logic triggering */
            nrfx_timer_disable(&_timer_button);
            /* If we made it here, this is a short press */
            led_toggle_pattern();
        }
    }
}

/**
 * @brief Callback for timer expired interrupt (long button press)
 * 
 * @param event_type 
 * @param p_context 
 */
void _timer_callback(nrf_timer_event_t event_type, void * p_context)
{
    if (event_type == NRF_TIMER_EVENT_COMPARE0)
    {
        /* Timer expired (long press) */
        LOG_DBG("LONG PRESS");
        if ((sys_state == SYS_STATE_POWEROFF))
        {
            bool _button_pressed = gpio_pin_get_dt(&_button_dt);
            /* Need to check button state since if the user quickly presses and releases button,
                the release event will occur before the button GPIO interrupt is configured. */
            if (_button_pressed)
            {
                /* Button still pressed, continue with wake up */
                sys_state = SYS_STATE_WAKEUP;
                /* Set boot pattern */
                led_set_pattern(LED_PATTERN_PULSE);
            }
            else
            {
                /* Button not pressed, go back to sleep */
                sys_power_off(&_button_dt);
            }
        }
        else
        {
            /* Shut device off */
            sys_power_off(&_button_dt);
        }
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
    nrfx_timer_extended_compare(&_timer_button, NRF_TIMER_CC_CHANNEL0, nrfx_timer_ms_to_ticks(&_timer_button, BUTTON_LONGPRESS_MS), NRF_TIMER_SHORT_COMPARE0_STOP_MASK, true);
    /* Start timer (for long press wake from sleep) */
    nrfx_timer_clear(&_timer_button);
    nrfx_timer_enable(&_timer_button);
    /* Needed to handle timer compare interrupts */
    IRQ_CONNECT(BUTTON_TIMER_IRQN, 0, NRFX_TIMER_INST_HANDLER_GET(BUTTON_TIMER_INSTANCE), NULL, 0);  // priority must be higher than GPIOTE to avoid button release event immediately waking up device
}