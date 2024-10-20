#include <zephyr/device.h>

#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>

#include <zephyr/kernel.h>

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(MAIN, LOG_LEVEL_INF);

/* GPIO */
static const struct gpio_dt_spec gpio_led_en = GPIO_DT_SPEC_GET(DT_NODELABEL(user_gpio), led_en_gpios);

/* PWM */
#define PWM_LED_PERIOD_US   10000  /* 10000 us = 100 KHz */
static const struct pwm_dt_spec pwm_led = PWM_DT_SPEC_GET(DT_NODELABEL(pwm_led0));

int main(void)
{
    int err;

    /* GPIO setup */
    if (!gpio_is_ready_dt(&gpio_led_en))
    {
        LOG_ERR("GPIO device not ready");
		return -1;
	}
    /* Configure LED enable pin */
    gpio_pin_configure_dt(&gpio_led_en, GPIO_OUTPUT);
    /* Set default state (OFF) */
    gpio_pin_set_dt(&gpio_led_en, 0);

    /* PWM setup */
    if (!pwm_is_ready_dt(&pwm_led))
    {
        LOG_ERR("PWM device %s not ready", pwm_led.dev->name);
        return -1;
    }
    /* Set duty cycle (testing) */
    while (1)
    {
        /* Blink HIGH (0.1 s) */
        err = gpio_pin_set_dt(&gpio_led_en, 1);
        err = pwm_set_dt(&pwm_led, PWM_LED_PERIOD_US, PWM_LED_PERIOD_US);
        if (err)
        {
            LOG_ERR("Error in pwm_set_dt(), err: %d", err);
            return -1;
        }
        k_msleep(10);
        err = gpio_pin_set_dt(&gpio_led_en, 0);
        // err = pwm_set_dt(&pwm_led, PWM_LED_PERIOD_US, 0);
        if (err)
        {
            LOG_ERR("Error in pwm_set_dt(), err: %d", err);
            return -1;
        }
        k_msleep(80);
        err = gpio_pin_set_dt(&gpio_led_en, 1);
        err = pwm_set_dt(&pwm_led, PWM_LED_PERIOD_US, PWM_LED_PERIOD_US);
        if (err)
        {
            LOG_ERR("Error in pwm_set_dt(), err: %d", err);
            return -1;
        }
        k_msleep(10);
        /* Blink LOW (0.4 s) */
        err = gpio_pin_set_dt(&gpio_led_en, 0);
        // err = pwm_set_dt(&pwm_led, PWM_LED_PERIOD_US, 0);
        if (err)
        {
            LOG_ERR("Error in pwm_set_dt(), err: %d", err);
            return -1;
        }
        k_msleep(900);
    }

    return 0;
}
