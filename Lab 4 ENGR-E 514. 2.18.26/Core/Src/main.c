/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "retarget.h"
#include "stm32l4xx_ll_usart.h"
#include "uart_rx_buffer.h"
#include "stm32l4xx_hal_rtc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdlib.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct command {
  char *cmd_string;
  void (*cmd_function)(char *arg);
} command_t;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LINE_MAX 128
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;

UART_HandleTypeDef huart2;
circ_buf_t g_rxbuf;
static uint8_t linebuf[LINE_MAX];
static uint16_t linelen = 0;
volatile uint32_t g_rx_irq_count = 0;
volatile uint8_t alarm_flag = 0;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_RTC_Init(void);
/* USER CODE BEGIN PFP */
static void print_prompt(void);
static void help_command(char *arguments);
static void lon_command(char *arguments);
static void lof_command(char *arguments);
static void time_set_command(char *arguments);
static void date_set_command(char *arguments);
static void test_command(char *arguments);
command_t commands[] = {
  {"help", help_command},
  {"lof",  lof_command},
  {"lon",  lon_command},
  {"test", test_command},
  {"ts", time_set_command},
  {"ds", date_set_command},
  {0, 0}
};
static int parse_command(uint8_t *line, uint8_t **command, uint8_t **args);
static int execute_command(uint8_t *line);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  LL_USART_EnableIT_RXNE(USART2);
  RetargetInit(&huart2);
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */
  rxbuf_init(&g_rxbuf);
  printf("System Up and Running\n\r");
  //printf("> ");
  print_prompt();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  uint8_t ch;
	  while (rxbuf_pop(&g_rxbuf, &ch) == 0)
	  {
		  if (ch == '\r')
		  {
			  printf("\r\n");
			  if (linelen < LINE_MAX) linebuf[linelen] = 0;
			  if (linelen > 0)
			  {
				  if (execute_command(linebuf) == 0)
				  {
					  printf("OK\r\n");

				  }
				  else
				  {
					  printf("NOK\r\n");
				  }
			  }
			  linelen = 0;
			  //printf("> ");
			  print_prompt();
			  continue;
		  }
		  if (ch == 0x7F || ch == '\b')
		  {
			  if (linelen > 0)
			  {
				  linelen--;
				  printf("\b \b");
			  }
			  continue;
		  }
		  if (linelen < (LINE_MAX - 1))
		  {
			  linebuf[linelen++] = ch;
			  printf("%c", ch);
		  }
	  }
	  if(alarm_flag)
	  {
		  alarm_flag = 0;
		  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
		  HAL_Delay(100);
		  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_LSE
                              |RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
  RTC_AlarmTypeDef sAlarm = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable the Alarm A
  */
  sAlarm.AlarmTime.Hours = 0x0;
  sAlarm.AlarmTime.Minutes = 0x0;
  sAlarm.AlarmTime.Seconds = 0x30;
  sAlarm.AlarmTime.SubSeconds = 0x0;
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 0x1;
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LD3_Pin */
  GPIO_InitStruct.Pin = LD3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD3_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
static int parse_command(uint8_t *line, uint8_t **command, uint8_t **args)
{
  if (!line || !command || !args) return -1;

  while (*line == ' ' || *line == '\t') line++;

  if (*line == 0) return -1;

  *command = line;

  uint8_t *p = line;
  while (*p && *p != ',') p++;

  if (*p == ',') {
    *p = 0;
    p++;
    *args = p;
  } else {
    *args = (uint8_t*)"";
  }

  return 0;
}

static int execute_command(uint8_t *line)
{
  uint8_t *cmd;
  uint8_t *arg;
  command_t *p = commands;
  int success = 0;

  if (!line) return -1;

  if (parse_command(line, &cmd, &arg) == -1) {
    printf("Error with parse command\r\n");
    return -1;
  }

  while (p->cmd_string) {
    if (!strcmp(p->cmd_string, (char*)cmd)) {
      if (!p->cmd_function) return -1;
      (*p->cmd_function)((char*)arg);
      success = 1;
      break;
    }
    p++;
  }

  if (success) return 0;
  return -1;
}

static void help_command(char *arguments)
{
  (void)arguments;
  printf("Available Commands:\r\n");
  for (command_t *p = commands; p->cmd_string; p++) {
    printf("%s\r\n", p->cmd_string);
  }
//  printf("rx_irq_count=%lu\r\n", g_rx_irq_count);
}

static void lon_command(char *arguments)
{
  (void)arguments;
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
}

static void lof_command(char *arguments)
{
  (void)arguments;
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
}
static void time_set_command(char *arguments)
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
	}
	int hour = atoi(hourStr);
	int min = atoi(minStr);
	int sec = atoi(secStr);

//	printf("Parsed hour: '%d'\r\n", hour);
//	printf("Parsed min: '%d'\r\n", min);
//	printf("Parsed sec: '%d'\r\n", sec);

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

//	printf("Built RTC struct:\r\n");
//	printf(" Hours: '%d'\r\n", t.Hours);
//	printf(" Minute: '%d'\r\n", t.Minutes);
//	printf(" Seconds: '%d'\r\n", t.Seconds);

	if(HAL_RTC_SetTime(&hrtc, &clk, RTC_FORMAT_BIN) != HAL_OK)
	{
		printf("NOK\r\n");
		return;
	}
}
static void date_set_command(char *arguments)
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
	}
	int month  = atoi(monthStr);
	int day = atoi(dayStr);
	int year = atoi(yearStr);

//	printf("Parsed Month Token: '%d'\r\n", month);
//	printf("Parse Day Token: '%d'\r\n", day);
//	printf("Parsed Year Token: '%d'\r\n", year);

	if(month < 0 || month >12)
	{
		printf("Month input is out of range\r\n");
		return;
	}
	if(day < 0 || day > 31)
	{
		printf("day input is out of range\r\n");
	}
	if(year < 0 || year > 2099)
	{
		printf("Year input is out of range\r\n");
		return;
	}
	//printf("validation OK\r\n");

	RTC_DateTypeDef d = {0};
	d.Month = (uint8_t)month;
	d.Date = (uint8_t)day;
	d.Year = (uint8_t)(year-2000);

	if(HAL_RTC_SetDate(&hrtc, &d, RTC_FORMAT_BIN) != HAL_OK)
	{
		printf("NOK\r\n");
		return;
	}
//	printf(" Built RTC date Structure: \r\n");
//	printf(" Month = %d\r\n", d.Month);
//	printf(" Day = %d\r\n", d.Date);
//	printf(" Year = %d\r\n", d.Year);

}

static void test_command(char *arguments)
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

static void print_prompt(void)
{
	RTC_TimeTypeDef clk;
	RTC_DateTypeDef dt;
	HAL_RTC_GetTime(&hrtc, &clk, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &dt, RTC_FORMAT_BIN);
	printf("%02d/%02d/20%2d %02d:%02d:%02d IULS> ",
			dt.Month,
			dt.Date,
			dt.Year,
			clk.Hours,
			clk.Minutes,
			clk.Seconds);
}
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	alarm_flag = 1;
	//printf("ALARM Fired");
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
