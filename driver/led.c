#include <stdbool.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "stm32f4xx_ll_gpio.h"
#include "led_desc.h"
#include "led.h"

void led_init(led_desc_t led)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {
        .Pin = led->pin,
        .Mode = LL_GPIO_MODE_OUTPUT,
        .OutputType = LL_GPIO_OUTPUT_PUSHPULL,
        .Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH,
        .Pull = LL_GPIO_PULL_NO,
    };
    LL_GPIO_Init(led->port, &GPIO_InitStruct);

    if (led->off_level)
        LL_GPIO_SetOutputPin(led->port, led->pin);
    else
        LL_GPIO_ResetOutputPin(led->port, led->pin);
}

void led_set(led_desc_t led, bool onoff)
{
    if (onoff)
        led_on(led);
    else
        led_off(led);
}

void led_on(led_desc_t led)
{
    if (led->on_level)
        LL_GPIO_SetOutputPin(led->port, led->pin);
    else
        LL_GPIO_ResetOutputPin(led->port, led->pin);
}

void led_off(led_desc_t led)
{
    if (led->off_level)
        LL_GPIO_SetOutputPin(led->port, led->pin);
    else
        LL_GPIO_ResetOutputPin(led->port, led->pin);
}
