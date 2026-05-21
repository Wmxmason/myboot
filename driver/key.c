#include <stdbool.h>
#include <stddef.h>
#include "stm32f4xx.h"
#include "stm32f4xx_ll_gpio.h"
#include "key.h"
#include "key_desc.h"

void key_init(key_desc_t key)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {
        .Pin = key->pin,
        .Mode = LL_GPIO_MODE_INPUT,
        .Pull = key->pull,
    };
    LL_GPIO_Init(key->port, &GPIO_InitStruct);
}

bool key_read(key_desc_t key)
{
    return LL_GPIO_IsInputPinSet(key->port, key->pin)
        == (key->press_level ? 1 : 0);
}
