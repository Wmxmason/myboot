#ifndef __LED_DESC_H__
#define __LED_DESC_H__

#include <stdint.h>
#include "stm32f4xx.h"

struct led_desc
{
    GPIO_TypeDef *port;
    uint32_t pin;
    uint32_t on_level;
    uint32_t off_level;
};

#endif /* __LED_DESC_H__ */
