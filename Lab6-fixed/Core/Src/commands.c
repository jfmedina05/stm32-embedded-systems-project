/*
 * commands.c
 *
 *  Created on: Mar 25, 2026
 *      Author: bradyadams
 */
#include "main.h"
#include "commands.h"
#include "flash.h"
#include "stm32l4xx_hal.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim2;
extern RTC_HandleTypeDef hrtc;
extern volatile uint32_t  g_period_ticks;
extern volatile uint8_t   g_capture_ready;
extern uint16_t ADC1_ReadChannel(uint32_t channel);
extern TIM_HandleTypeDef htim2;

typedef struct{
	char *name;
	void(*fn)(char *);
} command_t;

static void help_command(char *arguments);
static void lon_command(char *arguments);
static void lof_command(char *arguments);
static void time_set_command(char *arguments);
static void date_set_command(char *arguments);
static void temp_command(char *arguments);
static void battery_command(char *arguments);
static void tsl237_command(char *arguments);
static void cmd_sample (char *arguments);
static void cmd_data (char *arguments);
static void cmd_log (char *arguments);
static void cmd_erase (char *arguments);
static void test_command(char *arguments);

static command_t commands[] = {
		{"help", help_command},
		{"lon", lon_command},
		{"lof", lof_command},
		{"Set_Time", time_set_command},
		{"Set_Date", date_set_command},
		{"temp", temp_command},
		{"battery", battery_command},
		{"tsl237", tsl237_command},
		{"sample", cmd_sample},
		{"data", cmd_data},
		{"log", cmd_log},
		{"ef", cmd_erase},
		{"test", test_command},
		{0,0}

};
int execute_command(char *line)
{
//    // remove newline if present
//    cmd[strcspn(cmd, "\r\n")] = 0;
//
//    if (strcmp(cmd, "help") == 0)
//    {
//        printf("Available commands:\r\n");
//        printf("help - show commands\r\n");
//        return 0;
//    }
//    else if (strcmp(cmd, "ping") == 0)
//    {
//        printf("pong\r\n");
//        return 0;
//    }
//    else
//    {
//        printf("Unknown command: %s\r\n", cmd);
//        return -1;
//    }
	   if (!line) return -1;

	    // strip newline
	    line[strcspn(line, "\r\n")] = 0;

	    // split "command,arg" into two parts
	    char *arg = strchr(line, ',');
	    if (arg != NULL) { *arg = 0; arg++; }
	    else             { arg = ""; }

	    // walk the table looking for a match
	    for (command_t *p = commands; p->name; p++)
	    {
	        if (strcmp(p->name, line) == 0)
	        {
	            p->fn(arg);
	            return 0;
	        }
	    }

	    printf("Unknown command: %s\r\n", line);
	    return -1;
}
static void help_command(char *arguments)
{
	(void)arguments;
	printf("Available Commands:\r\n");
	for (command_t *p = commands; p->name; p++)
	{
		printf("%s\r\n", p->name);
	}
}
static void lon_command(char*arguments)
{
	(void)arguments;
	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
}
void lof_command(char*arguments)
{
	(void)arguments;
	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
}
void time_set_command(char *arguments)
{
	if(!arguments || arguments[0] == 0)
	{
		printf("NOK\r\n");
		return;
	}

	char *hourStr = strtok(arguments, ",");
	char *minStr = strtok(NULL, ",");
	char *secStr = strtok(NULL, ",");

	if(!hourStr || !minStr || !secStr) //|| hourStr[0] == 0
			//|| minStr[0] == 0 || secStr[0] == 0)
	{
		printf("NOK\r\n");
		return;
	}
	int hour = atoi(hourStr);
	int min = atoi(minStr);
	int sec = atoi(secStr);

	if(hour < 0 || hour >23)
	{
		printf("Hour input is out of range\r\n");
		return;
	}
	if(min < 0 || min > 59)
	{
		printf("Minutes input is out of range\r\n");
		return;
	}
	if(sec < 0 || sec > 59)
	{
		printf("Seconds input is out of range\r\n");
		return;
	}
	RTC_TimeTypeDef clk = {0};
	clk.Hours = (uint8_t)hour;
	clk.Minutes = (uint8_t)min;
	clk.Seconds = (uint8_t)sec;
	clk.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	clk.StoreOperation = RTC_STOREOPERATION_RESET;

	if(HAL_RTC_SetTime(&hrtc, &clk, RTC_FORMAT_BIN) != HAL_OK)
	{
		printf("NOK\r\n");
		return;
	}
}
void date_set_command(char *arguments)
{
	if(!arguments || arguments[0] == 0)
	{
		printf("NOK\r\n");
		return;
	}
	char *monthStr = strtok(arguments, ",");
	char *dayStr = strtok(NULL, ",");
	char *yearStr = strtok(NULL, ",");

	if(!monthStr || !dayStr || !yearStr)
	{
		printf("NOK\r\n");
		return;
	}
	int month  = atoi(monthStr);
	int day = atoi(dayStr);
	int year = atoi(yearStr);

	if(month < 1 || month >12)
	{
		printf("Month input is out of range\r\n");
		return;
	}
	if(day < 1 || day > 31)
	{
		printf("day input is out of range\r\n");
		return;
	}
	if(year < 0 || year > 2099)
	{
		printf("Year input is out of range\r\n");
		return;
	}

	RTC_DateTypeDef d = {0};
	d.WeekDay = RTC_WEEKDAY_MONDAY;
	d.Month = (uint8_t)month;
	d.Date = (uint8_t)day;
	d.Year = (uint8_t)(year-2000);

	if(HAL_RTC_SetDate(&hrtc, &d, RTC_FORMAT_BIN) != HAL_OK)
	{
		printf("NOK\r\n");
		return;
	}
}
void temp_command(char *arguments)
{
	(void)arguments;

	uint16_t adc_ts = ADC1_ReadChannel(LL_ADC_CHANNEL_TEMPSENSOR);
	uint16_t adc_vref = ADC1_ReadChannel(LL_ADC_CHANNEL_VREFINT);
	//temperature calibration
	uint16_t ts_cal1 = *TEMPSENSOR_CAL1_ADDR;
	uint16_t ts_cal2 = *TEMPSENSOR_CAL2_ADDR;
	uint16_t vrefint_cal = *VREFINT_CAL_ADDR;

	float vdda = 3.0f * (float)vrefint_cal / (float)adc_vref;
	float adc_ts_3v = (float)adc_ts * (vdda/3.0f);
	float temp_c = 30.0f + (adc_ts_3v - (float)ts_cal1) *(130.0f-30.0f)
			/((float)ts_cal2 - (float)ts_cal1);

	//float t1 = 30.0f;
	//float t2 = 130.0f;

	//float temp_c = t1 + ((float)adc_ts - (float)ts_cal1) * (t2 - t1)
	//					/ ((float)ts_cal2 - (float)ts_cal1);

	int temp_int = (int)(temp_c + 0.5f);
	printf("%d C\r\n", temp_int);
}
void battery_command(char *arguments)
{
	(void)arguments;

	uint16_t adc_vref = ADC1_ReadChannel(LL_ADC_CHANNEL_VREFINT);
	uint16_t vrefint_cal = *VREFINT_CAL_ADDR;

	//3 V
	float vdda = 3.0f * (float)vrefint_cal / (float)adc_vref;

	printf("%.2f V\r\n", vdda);
}
void tsl237_command(char *arguments)
{
	(void)arguments;

	// wait for measurement
	g_capture_ready = 0;

	//timeout loop
	for (uint32_t i = 0; i < 2000000; i++)
	{
		if (g_capture_ready) break;
	}

	if (!g_capture_ready || g_period_ticks == 0)
	{
		printf("ERR timeout\r\n");
		return;
	}

	//SYSCLK = 32 MHz and APB1 prescaler = 1, so TIM2CLK = 32 MHz
	float hz = 32000000.0f / (float)g_period_ticks;
	printf("%.2f hz\r\n", hz);
}
void test_command(char *arguments)
{
	  if (!arguments || arguments[0] == 0) {
	    return;
	  }

	  char *tok = strtok(arguments, ",");
	  while (tok) {
	    printf("%s\r\n", tok);
	    tok = strtok(NULL, ",");
	  }
}

void cmd_sample(char*arguments)
{
	(void)arguments;
    //uint32_t time = __HAL_TIM_GET_COUNTER(&htim2);
    //flash_write_record(time);
	if(flash_write_sample() !=0)
	{
		printf("NOK\r\n");
	}
}

void cmd_log(char *arguments)
{
	(void)arguments;
    //flash_read_all();
	flash_print_log();
}

void cmd_data(char *arguments)
{
	(void)arguments;
    //flash_read_all();
	flash_print_data();
}

void cmd_erase(char *arguments)
{
	if(!arguments||strcmp(arguments,"all")!=0)
	{
		printf("NOK\r\n");
		return;
	}
    if(flash_erase()!=0)
    {
    	printf("NOK\r\n");
    }
}
