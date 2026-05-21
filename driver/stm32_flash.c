#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_hal_flash.h"
#include "stm32f4xx_hal_flash_ex.h"
#include "utils.h"

#define LOG_TAG    "flash"
#define LOG_LVL    ELOG_LVL_INFO
#include "elog.h"

#define FLASH_BASE_ADDRESS  0x08000000

typedef struct
{
    uint32_t sector;
    uint32_t size;
} sector_desc_t;

static const sector_desc_t sector_descs[] =
{
    {FLASH_SECTOR_0, 16 * 1024},
    {FLASH_SECTOR_1, 16 * 1024},
    {FLASH_SECTOR_2, 16 * 1024},
    {FLASH_SECTOR_3, 16 * 1024},
    {FLASH_SECTOR_4, 64 * 1024},
    {FLASH_SECTOR_5, 128 * 1024},
    {FLASH_SECTOR_6, 128 * 1024},
    {FLASH_SECTOR_7, 128 * 1024},
    {FLASH_SECTOR_8, 128 * 1024},
    {FLASH_SECTOR_9, 128 * 1024},
    {FLASH_SECTOR_10, 128 * 1024},
    {FLASH_SECTOR_11, 128 * 1024},
};

void stm32_flash_lock(void)
{
    HAL_FLASH_Lock();
}

void stm32_flash_unlock(void)
{
    HAL_FLASH_Unlock();
}

void stm32_flash_erase(uint32_t address, uint32_t size)
{
    uint32_t addr = FLASH_BASE_ADDRESS;

    for (uint32_t i = 0; i < ARRAY_SIZE(sector_descs); i++)
    {
        if (addr >= address && addr < address + size)
        {
            log_i("erasing sector %lu at address 0x%08lX size %lu", i, addr, sector_descs[i].size);

            FLASH_EraseInitTypeDef eraseInit = {
                .TypeErase = FLASH_TYPEERASE_SECTORS,
                .Sector = sector_descs[i].sector,
                .NbSectors = 1,
                .VoltageRange = FLASH_VOLTAGE_RANGE_3,
            };
            uint32_t sectorError;
            if (HAL_FLASHEx_Erase(&eraseInit, &sectorError) != HAL_OK)
            {
                log_e("flash erase error at sector %lu", i);
            }
        }

        addr += sector_descs[i].size;
    }
}

void stm32_flash_program(uint32_t address, const uint8_t *data, uint32_t size)
{
    for (uint32_t i = 0; i < size; i += 4)
    {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address + i, *(uint32_t *)(data + i)) != HAL_OK)
        {
            log_e("flash program error at address 0x%08lX", address + i);
        }
    }
}
