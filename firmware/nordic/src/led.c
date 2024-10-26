/**
 * @file led.c
 * @author Ben Owen (ben@embeddedshark.com)
 * @brief File to initialize and change LED blink modes
 * 
 * @date 2024-10-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "led.h"

#include <nrfx_gpiote.h>
#include <nrfx_ppi.h>
#include <nrfx_pwm.h>
#include <nrfx_timer.h>

#include <zephyr/logging/log.h>

#if (LED_TIMER_INSTANCE == 0)
#define LED_TIMER_IRQN       TIMER0_IRQn
#elif (LED_TIMER_INSTANCE == 1)
#define LED_TIMER_IRQN       TIMER1_IRQn
#elif (LED_TIMER_INSTANCE == 2)
#define LED_TIMER_IRQN       TIMER2_IRQn
#elif (LED_TIMER_INSTANCE == 3)
#define LED_TIMER_IRQN       TIMER3_IRQn
#elif (LED_TIMER_INSTANCE == 4)
#define LED_TIMER_IRQN       TIMER4_IRQn
#endif

LOG_MODULE_REGISTER(LED, LOG_LEVEL_INF);

/**
 * @brief Pattern for constant max brightness
 * 
 */
nrf_pwm_values_common_t _pwm_pattern_on[] =
{
    100,
};
nrf_pwm_sequence_t _pwm_seq_on =
{
    .values = { _pwm_pattern_on },
    .length = NRFX_ARRAY_SIZE(_pwm_pattern_on),
    .repeats = 10,  // 10 ticks per value
    .end_delay = 0  // no delay between repeats
};

/**
 * @brief Pattern for constant medium brightness
 * 
 */
nrf_pwm_values_common_t _pwm_pattern_mid[] =
{
    600,
};
nrf_pwm_sequence_t _pwm_seq_mid =
{
    .values = { _pwm_pattern_mid },
    .length = NRFX_ARRAY_SIZE(_pwm_pattern_mid),
    .repeats = 10,  // 10 ticks per value
    .end_delay = 0  // no delay between repeats
};

/**
 * @brief Pattern for constant low brightness
 * 
 */
nrf_pwm_values_common_t _pwm_pattern_dim[] =
{
    1000,
};
nrf_pwm_sequence_t _pwm_seq_dim =
{
    .values = { _pwm_pattern_dim },
    .length = NRFX_ARRAY_SIZE(_pwm_pattern_dim),
    .repeats = 10,  // 10 ticks per value
    .end_delay = 0  // no delay between repeats
};

/**
 * @brief Pattern for dim blinking
 * 
 */
nrf_pwm_values_common_t _pwm_pattern_dim_blink[] =
{
    600,  1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 600,
    1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000,
    1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000,
    1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000,
    1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000,
    1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000,
    1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000,
    1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000,
    1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000,
    1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000,
};
nrf_pwm_sequence_t _pwm_seq_dim_blink =
{
    .values = { _pwm_pattern_dim_blink },
    .length = NRFX_ARRAY_SIZE(_pwm_pattern_dim_blink),
    .repeats = 10,  // 10 ticks per value
    .end_delay = 0  // no delay between repeats
};

/**
 * @brief Pattern for pulsing
 * 
 */
nrf_pwm_values_common_t _pwm_pattern_pulse[] =
{
    990,  980,  970,  960,  950,  940,  930,  920,  910,  900,
    890,  880,  870,  860,  850,  840,  830,  820,  810,  800,
    790,  780,  770,  760,  750,  740,  730,  720,  710,  700,
    690,  680,  670,  660,  650,  640,  630,  620,  610,  600,
    590,  580,  570,  560,  550,  540,  530,  520,  510,  500,
    500,  500,  500,  500,  500,  500,  500,  500,  500,  500,
    500,  510,  520,  530,  540,  550,  560,  570,  580,  590,
    600,  610,  620,  630,  640,  650,  660,  670,  680,  690,
    700,  710,  720,  730,  740,  750,  760,  770,  780,  790,
    800,  810,  820,  830,  840,  850,  860,  870,  880,  890,
    900,  910,  920,  930,  940,  950,  960,  970,  980,  990,
    1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000,
};
nrf_pwm_sequence_t _pwm_seq_pulse =
{
    .values = { _pwm_pattern_pulse },
    .length = NRFX_ARRAY_SIZE(_pwm_pattern_pulse),
    .repeats = 25,  // 10 ticks per value
    .end_delay = 1000  // no delay between repeats
};

/* GPIO */
static nrfx_gpiote_t _gpiote = NRFX_GPIOTE_INSTANCE(LED_GPIOTE_INSTANCE);
#define LED_EN_PIN DT_GPIO_PIN(DT_NODELABEL(user_gpio), led_en_gpios)

/* Timer */
static nrfx_timer_t _timer_led = NRFX_TIMER_INSTANCE(LED_TIMER_INSTANCE);

/* PPI */
nrf_ppi_channel_t _ppi_first_blink_start_ch;
nrf_ppi_channel_t _ppi_first_blink_end_ch;
nrf_ppi_channel_t _ppi_second_blink_start_ch;
nrf_ppi_channel_t _ppi_second_blink_end_ch;

/* PWM */
static nrfx_pwm_t _pwm_led = NRFX_PWM_INSTANCE(LED_PWM_INSTANCE);
#define LED_PWM_PIN 17

/* Other locals */
led_pattern_t _current_pattern;

void led_init(void)
{
    nrfx_err_t err;

    /* GPIO config */
    /* Initialize GPIO */
    err = nrfx_gpiote_init(&_gpiote, NRFX_GPIOTE_DEFAULT_CONFIG_IRQ_PRIORITY);
    NRFX_ASSERT(err == NRFX_SUCCESS);
    /* Generate output configurations */
    nrfx_gpiote_output_config_t _led_en_config = NRFX_GPIOTE_DEFAULT_OUTPUT_CONFIG;
    /* Allocate GPIOTE channels for tasks */
    uint8_t _gpiote_led_en_ch;
    err = nrfx_gpiote_channel_alloc(&_gpiote, &_gpiote_led_en_ch);
    NRFX_ASSERT(err == NRFX_SUCCESS);
    /* Create tasks */
    nrfx_gpiote_task_config_t _led_en_task =
    {
        .task_ch = _gpiote_led_en_ch,
        .polarity = NRF_GPIOTE_POLARITY_TOGGLE,
        .init_val = NRF_GPIOTE_INITIAL_VALUE_LOW,
    };
    /* Configure LED enable pin with defined configurations */
    err = nrfx_gpiote_output_configure(&_gpiote, LED_EN_PIN, &_led_en_config, &_led_en_task);
    NRFX_ASSERT(err == NRFX_SUCCESS);
    /* Enable GPIOTE task */
    nrfx_gpiote_out_task_enable(&_gpiote, LED_EN_PIN);

    /* Timer config */
    /* Configure timer parameters */
    nrfx_timer_config_t _timer_led_config =
    {
        .frequency          = NRFX_MHZ_TO_HZ(1),
        .mode               = NRF_TIMER_MODE_TIMER,
        .bit_width          = NRF_TIMER_BIT_WIDTH_32,
        .interrupt_priority = NRFX_TIMER_DEFAULT_CONFIG_IRQ_PRIORITY,
        .p_context          = NULL,
    };
    /* Initialize timer */
    err = nrfx_timer_init(&_timer_led, &_timer_led_config, NULL);
    NRFX_ASSERT(err == NRFX_SUCCESS);
    /* Needed to handle timer compare interrupts */
    IRQ_CONNECT(LED_TIMER_IRQN, IRQ_PRIO_LOWEST, NRFX_TIMER_INST_HANDLER_GET(LED_TIMER_INSTANCE), NULL, 0);

    /* PWM config */
    /* Initialize PWM */
    nrfx_pwm_config_t config = NRFX_PWM_DEFAULT_CONFIG(LED_PWM_PIN, NRF_PWM_PIN_NOT_CONNECTED, NRF_PWM_PIN_NOT_CONNECTED, NRF_PWM_PIN_NOT_CONNECTED);
    err = nrfx_pwm_init(&_pwm_led, &config, NULL, &_pwm_led);
    NRFX_ASSERT(err == NRFX_SUCCESS);
    /* Handle PWM interrupt */
    IRQ_CONNECT(NRFX_IRQ_NUMBER_GET(NRF_PWM_INST_GET(LED_PWM_INSTANCE)), IRQ_PRIO_LOWEST, NRFX_PWM_INST_HANDLER_GET(LED_PWM_INSTANCE), 0, 0);

    /* PPI config */
    /* Allocate PPI channels */
    err = nrfx_ppi_channel_alloc(&_ppi_first_blink_start_ch);
    NRFX_ASSERT(err == NRFX_SUCCESS);
    err = nrfx_ppi_channel_alloc(&_ppi_first_blink_end_ch);
    NRFX_ASSERT(err == NRFX_SUCCESS);
    err = nrfx_ppi_channel_alloc(&_ppi_second_blink_start_ch);
    NRFX_ASSERT(err == NRFX_SUCCESS);
    err = nrfx_ppi_channel_alloc(&_ppi_second_blink_end_ch);
    NRFX_ASSERT(err == NRFX_SUCCESS);
}

void led_set_pattern(led_pattern_t pattern)
{
    nrfx_err_t err;
    /* Disable LED driver while setting up new pattern */
    nrfx_gpiote_clr_task_trigger(&_gpiote, LED_EN_PIN);
    /* Disable timer */
    nrfx_timer_disable(&_timer_led);
    nrfx_timer_clear(&_timer_led);
    /* Disable PPI channels */
    err = nrfx_ppi_channel_disable(_ppi_first_blink_start_ch);
    NRFX_ASSERT(err == NRFX_SUCCESS);
    err = nrfx_ppi_channel_disable(_ppi_first_blink_end_ch);
    NRFX_ASSERT(err == NRFX_SUCCESS);
    err = nrfx_ppi_channel_disable(_ppi_second_blink_start_ch);
    NRFX_ASSERT(err == NRFX_SUCCESS);
    err = nrfx_ppi_channel_disable(_ppi_second_blink_end_ch);
    NRFX_ASSERT(err == NRFX_SUCCESS);
    /* Disable PWM sequence */
    nrfx_pwm_stop(&_pwm_led, true);
    /* Store new pattern */
    _current_pattern = pattern;

    /* Set up pattern */
    switch (pattern)
    {
        case (LED_PATTERN_BRIGHT_BLINK):
        {
            /* Pattern is 2 quick blinks, otherwise off */
            /* Set timer compare channels */
            nrfx_timer_compare(&_timer_led, NRF_TIMER_CC_CHANNEL0, nrfx_timer_ms_to_ticks(&_timer_led, 10), false);
            nrfx_timer_compare(&_timer_led, NRF_TIMER_CC_CHANNEL1, nrfx_timer_ms_to_ticks(&_timer_led, 90), false);
            nrfx_timer_compare(&_timer_led, NRF_TIMER_CC_CHANNEL2, nrfx_timer_ms_to_ticks(&_timer_led, 100), false);
            nrfx_timer_extended_compare(&_timer_led, NRF_TIMER_CC_CHANNEL3, nrfx_timer_ms_to_ticks(&_timer_led, 1000), NRF_TIMER_SHORT_COMPARE3_CLEAR_MASK, false);
            /* Assign PPI channels to set/clear LED enable pin */
            err = nrfx_ppi_channel_assign(_ppi_first_blink_start_ch, nrfx_timer_event_address_get(&_timer_led, NRF_TIMER_EVENT_COMPARE3), nrfx_gpiote_set_task_address_get(&_gpiote, LED_EN_PIN));
            NRFX_ASSERT(err == NRFX_SUCCESS);
            err = nrfx_ppi_channel_assign(_ppi_first_blink_end_ch, nrfx_timer_event_address_get(&_timer_led, NRF_TIMER_EVENT_COMPARE0), nrfx_gpiote_clr_task_address_get(&_gpiote, LED_EN_PIN));
            NRFX_ASSERT(err == NRFX_SUCCESS);
            err = nrfx_ppi_channel_assign(_ppi_second_blink_start_ch, nrfx_timer_event_address_get(&_timer_led, NRF_TIMER_EVENT_COMPARE1), nrfx_gpiote_set_task_address_get(&_gpiote, LED_EN_PIN));
            NRFX_ASSERT(err == NRFX_SUCCESS);
            err = nrfx_ppi_channel_assign(_ppi_second_blink_end_ch, nrfx_timer_event_address_get(&_timer_led, NRF_TIMER_EVENT_COMPARE2), nrfx_gpiote_clr_task_address_get(&_gpiote, LED_EN_PIN));
            NRFX_ASSERT(err == NRFX_SUCCESS);
            /* Enable PPI channels */
            err = nrfx_ppi_channel_enable(_ppi_first_blink_start_ch);
            NRFX_ASSERT(err == NRFX_SUCCESS);
            err = nrfx_ppi_channel_enable(_ppi_first_blink_end_ch);
            NRFX_ASSERT(err == NRFX_SUCCESS);
            err = nrfx_ppi_channel_enable(_ppi_second_blink_start_ch);
            NRFX_ASSERT(err == NRFX_SUCCESS);
            err = nrfx_ppi_channel_enable(_ppi_second_blink_end_ch);
            NRFX_ASSERT(err == NRFX_SUCCESS);
            /* Set PWM sequence to max brightness (will control blinks with enable GPIO) */
            nrfx_pwm_simple_playback(&_pwm_led, &_pwm_seq_on, 1, NRFX_PWM_FLAG_LOOP);
            /* Start timer */
            nrfx_timer_enable(&_timer_led);
            break;
        }

        case (LED_PATTERN_DIM_BLINK):
        {
            /* Pattern is 2 quick blinks, otherwise dim */
            /* Enable LED driver constantly (PWM pattern will handle blinks) */
            nrfx_gpiote_set_task_trigger(&_gpiote, LED_EN_PIN);
            /* Set PWM sequence to dim blink pattern */
            nrfx_pwm_simple_playback(&_pwm_led, &_pwm_seq_dim_blink, 1, NRFX_PWM_FLAG_LOOP);
            break;
        }

        case (LED_PATTERN_BRIGHT_SOLID):
        {
            /* Pattern is solid bright */
            /* Enable LED driver constantly (PWM pattern will handle brightness) */
            nrfx_gpiote_set_task_trigger(&_gpiote, LED_EN_PIN);
            /* Set PWM sequence to bright solid pattern */
            nrfx_pwm_simple_playback(&_pwm_led, &_pwm_seq_mid, 1, NRFX_PWM_FLAG_LOOP);
            break;
        }

        case (LED_PATTERN_DIM_SOLID):
        {
            /* Pattern is solid dim */
            /* Enable LED driver constantly (PWM pattern will handle brightness) */
            nrfx_gpiote_set_task_trigger(&_gpiote, LED_EN_PIN);
            /* Set PWM sequence to dim solid pattern */
            nrfx_pwm_simple_playback(&_pwm_led, &_pwm_seq_dim, 1, NRFX_PWM_FLAG_LOOP);
            break;
        }

        case (LED_PATTERN_PULSE):
        {
            /* Pattern is pulse from dim to bright to dim */
            /* Enable LED driver constantly (PWM pattern will handle pulse) */
            nrfx_gpiote_set_task_trigger(&_gpiote, LED_EN_PIN);
            /* Set PWM sequence to pulse pattern */
            nrfx_pwm_simple_playback(&_pwm_led, &_pwm_seq_pulse, 1, NRFX_PWM_FLAG_LOOP);
            break;
        }

        case (LED_PATTERN_OFF):
        {
            /* Pattern is off */
            /* Disable LED driver */
            nrfx_gpiote_clr_task_trigger(&_gpiote, LED_EN_PIN);
            // /* Set PWM sequence to blink pattern */
            // nrfx_pwm_simple_playback(&_pwm_led, &_pwm_seq_mid, 1, NRFX_PWM_FLAG_LOOP);
            break;
        }

        default:
        {
            /* Invalid pattern */
            NRFX_ASSERT(0);
            break;
        }
    }
}

void led_toggle_pattern(void)
{
    _current_pattern = (_current_pattern + 1) % LED_PATTERN_NUM_PATTERNS;
    led_set_pattern(_current_pattern);
}