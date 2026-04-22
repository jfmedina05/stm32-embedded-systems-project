/*
 * flash.c
 *
 *  Created on: Mar 25, 2026
 *      Author: bradyadams
 */
/*
 * flash.c
 */
#include "flash.h"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_ll_adc.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern RTC_HandleTypeDef  hrtc;
extern volatile uint32_t  g_period_ticks;
extern volatile uint8_t   g_capture_ready;
extern uint16_t ADC1_ReadChannel(uint32_t channel);


// these are defined in the linker script
extern int _edata;
extern int _sdata;
extern int __fini_array_end;

// ── time packing ─────────────────────────────────────────────────
uint32_t pack_time(RTC_TimeTypeDef *time, RTC_DateTypeDef *date)
{
    uint32_t out = 0, temp = 0;
    temp = (((uint32_t)date->Date)    & 0b00011111) << 27;
    out  = temp;
    temp = (((uint32_t)date->Month)   & 0b00001111) << 22;
    out |= temp;
    temp = ((((uint32_t)date->Year) - 19) & 0b00000111) << 19;
    out |= temp;
    temp = (((uint32_t)time->Hours)   & 0b00011111) << 13;
    out |= temp;
    temp = (((uint32_t)time->Minutes) & 0b00111111) << 6;
    out |= temp;
    temp = (((uint32_t)time->Seconds) & 0b00111111);
    out |= temp;
    return out;
}

void unpack_time(uint32_t timeval, RTC_TimeTypeDef *time, RTC_DateTypeDef *date)
{
    uint32_t temp;

    temp = timeval & 0x3F;
    time->Seconds = (uint8_t)temp;

    temp = (timeval >> 6) & 0x3F;
    time->Minutes = (uint8_t)temp;

    temp = (timeval >> 13) & 0x1F;
    time->Hours = (uint8_t)temp;

    temp = (timeval >> 19) & 0x07;
    date->Year = (uint8_t)temp + 19;

    temp = (timeval >> 22) & 0x0F;
    date->Month = (uint8_t)temp;

    temp = (timeval >> 27) & 0x1F;
    date->Date = (uint8_t)temp;
}

// ── private helpers ──────────────────────────────────────────────
static uint32_t GetPage(uint32_t Addr)
{
    if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
        return (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;
    else
        return (Addr - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
}

static uint32_t GetBank(uint32_t Addr)
{
    (void)Addr;
    return FLASH_BANK_1;
}

static uint32_t flash_get_bottom_address(void)
{
    uint32_t program_end = ((uint32_t)&__fini_array_end
      + (uint32_t)&_edata
      - (uint32_t)&_sdata);
    return (((uint32_t)program_end & ~0x7FF) + 0x800);
}

static uint32_t flash_get_next_open_address(void)
{
    uint32_t addr = flash_get_bottom_address();
    while (addr < FLASH_END)
    {
        if (*(uint8_t *)addr == 0xFF) return addr;
        addr += sizeof(raw_t);
    }
    return 0;
}

static uint16_t flash_get_next_record_number(void)
{
    uint32_t addr = flash_get_bottom_address();
    uint16_t record_number = 0;
    while (addr < FLASH_END)
    {
        if (*(uint8_t *)addr == 0xFF) break;
        record_number++;
        addr += sizeof(raw_t);
    }
    return record_number;
}

// ── public functions ─────────────────────────────────────────────
int flash_erase(void)
{
    uint32_t FirstPage, NbOfPages, BankNumber, PAGEError;
    static FLASH_EraseInitTypeDef EraseInitStruct;

    uint32_t bottom = flash_get_bottom_address();
    uint32_t top    = FLASH_END;

    HAL_FLASH_Unlock();
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);

    FirstPage  = GetPage(bottom);
    NbOfPages  = GetPage(top) - FirstPage + 1;
    BankNumber = GetBank(bottom);

    EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.Banks     = BankNumber;
    EraseInitStruct.Page      = FirstPage;
    EraseInitStruct.NbPages   = NbOfPages;

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
    {
        HAL_FLASH_Lock();
        return -1;
    }

    HAL_FLASH_Lock();
    return 0;
}

int flash_write_sample(void)
{
    sensordata_t data;
    raw_t raw;
    RTC_TimeTypeDef clk;
    RTC_DateTypeDef dt;

    uint16_t adc_ts      = ADC1_ReadChannel(LL_ADC_CHANNEL_TEMPSENSOR);
    uint16_t adc_vref    = ADC1_ReadChannel(LL_ADC_CHANNEL_VREFINT);
    uint16_t ts_cal1     = *TEMPSENSOR_CAL1_ADDR;
    uint16_t ts_cal2     = *TEMPSENSOR_CAL2_ADDR;
    uint16_t vrefint_cal = *VREFINT_CAL_ADDR;

    float vdda      = 3.0f * (float)vrefint_cal / (float)adc_vref;
    float adc_ts_3v = (float)adc_ts * (vdda / 3.0f);
    float temp_c    = 30.0f + (adc_ts_3v - (float)ts_cal1)
                      * (130.0f - 30.0f)
                      / ((float)ts_cal2 - (float)ts_cal1);

    // wait for light sensor reading
    g_capture_ready = 0;
    for (uint32_t i = 0; i < 2000000; i++)
        if (g_capture_ready) break;

    if (!g_capture_ready || g_period_ticks == 0) return -1;

    HAL_RTC_GetTime(&hrtc, &clk, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &dt,  RTC_FORMAT_BIN);

    data.watermark       = 0x01;
    data.status          = 0x01;
    data.record_number   = flash_get_next_record_number();
    data.timestamp       = pack_time(&clk, &dt);
    data.battery_voltage = (uint16_t)(vdda * 1000.0f);
    data.temperature     = (int16_t)(temp_c);
    data.sensor_period   = g_period_ticks;

    memcpy(&raw, &data, sizeof(raw_t));

    uint32_t addr = flash_get_next_open_address();
    if (addr == 0) return -1;

    HAL_FLASH_Unlock();
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr,     raw.data0) != HAL_OK)
        { HAL_FLASH_Lock(); return -1; }
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr + 8, raw.data1) != HAL_OK)
        { HAL_FLASH_Lock(); return -1; }
    HAL_FLASH_Lock();
    return 0;
}

int flash_write_log(char *msg)
{
    logdata_t data;
    raw_t raw;
    RTC_TimeTypeDef clk;
    RTC_DateTypeDef dt;

    if (!msg) return -1;

    HAL_RTC_GetTime(&hrtc, &clk, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &dt,  RTC_FORMAT_BIN);

    memset(&data, 0xFF, sizeof(data));
    data.watermark     = 0x01;
    data.status        = 0x02;
    data.record_number = flash_get_next_record_number();
    data.timestamp     = pack_time(&clk, &dt);
    memset(data.msg, 0, sizeof(data.msg));
    strncpy((char *)data.msg, msg, 8);

    memcpy(&raw, &data, sizeof(raw_t));

    uint32_t addr = flash_get_next_open_address();
    if (addr == 0) return -1;

    HAL_FLASH_Unlock();
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr,     raw.data0) != HAL_OK)
        { HAL_FLASH_Lock(); return -1; }
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr + 8, raw.data1) != HAL_OK)
        { HAL_FLASH_Lock(); return -1; }
    HAL_FLASH_Lock();
    return 0;
}

void flash_print_data(void)
{
    uint32_t addr = flash_get_bottom_address();
    while (addr < FLASH_END)
    {
        if (*(uint8_t *)addr == 0xFF) break;
        if (*(uint8_t *)(addr + 1) == 0x01)
        {
            sensordata_t data;
            RTC_TimeTypeDef clk = {0};
            RTC_DateTypeDef dt  = {0};
            memcpy(&data, (void *)addr, sizeof(sensordata_t));
            unpack_time(data.timestamp, &clk, &dt);
            printf("D,%d,%02d/%02d/20%02d,%02d:%02d:%02d,%.3f,%d,%lu\r\n",
                data.record_number,
                dt.Month, dt.Date, dt.Year,
                clk.Hours, clk.Minutes, clk.Seconds,
                ((float)data.battery_voltage) / 1000.0f,
                data.temperature,
                data.sensor_period);
        }
        addr += sizeof(raw_t);
    }
}

void flash_print_log(void)
{
    uint32_t addr = flash_get_bottom_address();
    while (addr < FLASH_END)
    {
        if (*(uint8_t *)addr == 0xFF) break;
        if (*(uint8_t *)(addr + 1) == 0x02)
        {
            logdata_t data;
            RTC_TimeTypeDef clk = {0};
            RTC_DateTypeDef dt  = {0};
            char msg[9];
            memcpy(&data, (void *)addr, sizeof(logdata_t));
            unpack_time(data.timestamp, &clk, &dt);
            memset(msg, 0, sizeof(msg));
            memcpy(msg, data.msg, 8);
            msg[8] = 0;
            printf("L,%d,%02d/%02d/20%02d,%02d:%02d:%02d,%s\r\n",
                data.record_number,
                dt.Month, dt.Date, dt.Year,
                clk.Hours, clk.Minutes, clk.Seconds,
                msg);
        }
        addr += sizeof(raw_t);
    }
}
//#include "flash.h"
//#include <string.h>
//#include <stdio.h>
//
//extern UART_HandleTypeDef huart2;
//
//static uint32_t current_address = FLASH_START_ADDR;
//
//void flash_erase(void)
//{
//    HAL_FLASH_Unlock();
//
//    FLASH_EraseInitTypeDef erase;
//    uint32_t page_error;
//
//    erase.TypeErase = FLASH_TYPEERASE_PAGES;
//    erase.Page = (FLASH_START_ADDR - FLASH_BASE) / FLASH_PAGE_SIZE;
//    erase.NbPages = 1;
//
//    HAL_FLASHEx_Erase(&erase, &page_error);
//
//    HAL_FLASH_Lock();
//
//    current_address = FLASH_START_ADDR;
//
//    char msg[] = "Flash Erased\r\n";
//    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);
//}
//
//void flash_write_record(uint32_t timestamp)
//{
//    HAL_FLASH_Unlock();
//
//    uint64_t data = timestamp;
//
//    HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, current_address, data);
//
//    HAL_FLASH_Lock();
//
//    current_address += 8;
//
//    char msg[] = "Sample Stored\r\n";
//    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);
//}
//
//void flash_read_all(void)
//{
//    uint32_t addr = FLASH_START_ADDR;
//    char buffer[64];
//
//    while (addr < current_address)
//    {
//        uint64_t raw = *(uint64_t*)addr;
//        uint32_t data = (uint32_t)raw;
//
//        sprintf(buffer, "Time: %lu\r\n", data);
//        HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);
//
//        addr += 8;  // match write size
//    }
//}
