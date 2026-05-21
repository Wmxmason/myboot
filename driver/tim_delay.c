#include <stdint.h>
#include <string.h>
#include "stm32f4xx.h"
#include "stm32f4xx_ll_tim.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_hal_rcc.h"
#include "tim_delay.h"

static volatile uint64_t tim_tick_count;
static tim_periodic_callback_t periodic_callback;

void tim_delay_init(void)
{
    uint32_t pclk1 = HAL_RCC_GetPCLK1Freq();
    uint32_t apb1_tim_freq_mhz = pclk1 / 500000;  // STM32F4: APB1 timer clock = 2x PCLK1, /500000 gives timer MHz

    LL_TIM_InitTypeDef TIM_InitStruct = {
        .Prescaler = apb1_tim_freq_mhz - 1,
        .Autoreload = 999,
        .CounterMode = LL_TIM_COUNTERMODE_UP,
        .ClockDivision = LL_TIM_CLOCKDIVISION_DIV1,
    };
    LL_TIM_Init(TIM6, &TIM_InitStruct);
    LL_TIM_EnableIT_UPDATE(TIM6);
    LL_TIM_EnableCounter(TIM6);

    NVIC_SetPriority(TIM6_DAC_IRQn, 5);
    NVIC_EnableIRQ(TIM6_DAC_IRQn);
}

uint64_t tim_now(void)
{
    uint64_t now, last_count;
    do {
        last_count = tim_tick_count;
        now = tim_tick_count + LL_TIM_GetCounter(TIM6);
    } while (last_count != tim_tick_count);
    return now;
}

uint64_t tim_get_us(void)
{
    return tim_now();
}

uint64_t tim_get_ms(void)
{
    return tim_now() / 1000;
}

void tim_delay_us(uint32_t us)
{
    uint64_t now = tim_now();
    while (tim_now() - now < (uint64_t)us);
}

void tim_delay_ms(uint32_t ms)
{
    uint64_t now = tim_now();
    while (tim_now() - now < (uint64_t)ms * 1000);
}

void tim_register_periodic_callback(tim_periodic_callback_t callback)
{
    periodic_callback = callback;
}

void TIM6_DAC_IRQHandler(void)
{
    if (LL_TIM_IsActiveFlag_UPDATE(TIM6))
    {
        LL_TIM_ClearFlag_UPDATE(TIM6);
        tim_tick_count += 1000;
        if (periodic_callback)
            periodic_callback();
    }
}
