#include "stm32f4xx.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_usart.h"
#include "bl_usart.h"

// USE USART3
// RX: PB11
// TX: PB10
// MODE: 8-N-1
// BAUD: 115200

static bl_usart_rx_callback_t rx_callback;

static void usart_io_init(void)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {
        .Pin = LL_GPIO_PIN_10 | LL_GPIO_PIN_11,
        .Mode = LL_GPIO_MODE_ALTERNATE,
        .Speed = LL_GPIO_SPEED_FREQ_HIGH,
        .OutputType = LL_GPIO_OUTPUT_PUSHPULL,
        .Pull = LL_GPIO_PULL_UP,
        .Alternate = LL_GPIO_AF_7,
    };
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

static void usart_it_config(void)
{
    NVIC_SetPriority(USART3_IRQn, 5);
    NVIC_EnableIRQ(USART3_IRQn);
}

static void usart_lowlevel_init(void)
{
    LL_USART_InitTypeDef USART_InitStruct = {
        .BaudRate = 115200,
        .DataWidth = LL_USART_DATAWIDTH_8B,
        .StopBits = LL_USART_STOPBITS_1,
        .Parity = LL_USART_PARITY_NONE,
        .TransferDirection = LL_USART_DIRECTION_TX_RX,
        .HardwareFlowControl = LL_USART_HWCONTROL_NONE,
    };
    LL_USART_Init(USART3, &USART_InitStruct);
    LL_USART_EnableIT_RXNE(USART3);
    LL_USART_Enable(USART3);
}

void bl_usart_init(void)
{
    usart_it_config();
    usart_lowlevel_init();
    usart_io_init();
}

void bl_usart_write(const uint8_t *data, uint32_t size)
{
    while (size--)
    {
        while (!LL_USART_IsActiveFlag_TXE(USART3));
        LL_USART_TransmitData8(USART3, *data++);
        while (!LL_USART_IsActiveFlag_TC(USART3));
    }
}

void bl_usart_register_rx_callback(bl_usart_rx_callback_t callback)
{
    rx_callback = callback;
}

void USART3_IRQHandler(void)
{
    if (LL_USART_IsActiveFlag_RXNE(USART3))
    {
        if (rx_callback)
        {
            uint8_t data = LL_USART_ReceiveData8(USART3);
            rx_callback(&data, 1);
        }
    }
}
