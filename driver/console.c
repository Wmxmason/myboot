#include <stdint.h>
#include <string.h>
#include "stm32f4xx.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_usart.h"
#include "console.h"

static void console_io_init(void)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {
        .Pin = LL_GPIO_PIN_9 | LL_GPIO_PIN_10,
        .Mode = LL_GPIO_MODE_ALTERNATE,
        .Speed = LL_GPIO_SPEED_FREQ_HIGH,
        .OutputType = LL_GPIO_OUTPUT_PUSHPULL,
        .Pull = LL_GPIO_PULL_NO,
        .Alternate = LL_GPIO_AF_7,
    };
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

static void console_usart_init(void)
{
    LL_USART_InitTypeDef USART_InitStruct = {
        .BaudRate = 115200,
        .DataWidth = LL_USART_DATAWIDTH_8B,
        .StopBits = LL_USART_STOPBITS_1,
        .Parity = LL_USART_PARITY_NONE,
        .TransferDirection = LL_USART_DIRECTION_TX_RX,
        .HardwareFlowControl = LL_USART_HWCONTROL_NONE,
    };
    LL_USART_Init(USART1, &USART_InitStruct);
    LL_USART_Enable(USART1);
}

void console_init(void)
{
    console_usart_init();
    console_io_init();
}

void console_write(const char str[], uint32_t length)
{
    for (uint32_t i = 0; i < length; i++)
    {
        while (!LL_USART_IsActiveFlag_TXE(USART1));
        LL_USART_TransmitData8(USART1, str[i]);
    }
}
