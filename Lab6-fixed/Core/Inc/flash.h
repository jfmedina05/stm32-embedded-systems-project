/*
 * flash.h
 *
 *  Created on: Mar 25, 2026
 *      Author: bradyadams
 */

#ifndef FLASH_H
#define FLASH_H

#include "stm32l4xx_hal.h"
#include <stdint.h>

typedef struct {
    uint8_t  watermark;
    uint8_t  status;
    uint16_t record_number;
    uint32_t timestamp;
    uint16_t battery_voltage;
    int16_t  temperature;
    uint32_t sensor_period;
} sensordata_t;

typedef struct {
    uint8_t  watermark;
    uint8_t  status;
    uint16_t record_number;
    uint32_t timestamp;
    uint8_t  msg[8];
} logdata_t;

typedef struct {
    uint64_t data0;
    uint64_t data1;
} raw_t;

int  flash_erase(void);
int  flash_write_sample(void);
int  flash_write_log(char *msg);
void flash_print_data(void);
void flash_print_log(void);

uint32_t pack_time(RTC_TimeTypeDef *time, RTC_DateTypeDef *date);
void     unpack_time(uint32_t timeval, RTC_TimeTypeDef *time, RTC_DateTypeDef *date);

#endif

//#include "stm32l4xx_hal.h"
//
//#define FLASH_START_ADDR  0x0801F800
//
//void flash_write_record(uint32_t timestamp);
//void flash_read_all(void);
//void flash_erase(void);
//
//#endif
