/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */
/* USER CODE END Header */

#include "main.h"

/* Private includes ----------------------------------------------------------*/
#include <retarget.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "ring_buffer.h"
#include "stm32l4xx_ll_usart.h"

/* Private define ------------------------------------------------------------*/
#define RX_BUFFER_SIZE 128
#define CLI_BUFFER_SIZE 128

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

ring_buffer_t rx_buffer;

static uint8_t cli_buffer[CLI_BUFFER_SIZE];
static uint16_t cli_index;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);

/* CLI prototypes */
static void print_prompt(void);
static int execute_command(uint8_t *line);
static int parse_command(uint8_t *line, uint8_t **command, uint8_t **args);

static void help_command(char *args);
static void lof_command(char *args);
static void lon_command(char *args);
static void test_command(char *args);

/* Command table */
typedef struct
{
    const char *cmd_string;
    void (*cmd_function)(char *arg);
} command_t;

static command_t commands[] =
{
    {"help", help_command},
    {"lof",  lof_command},
    {"lon",  lon_command},
    {"test", test_command},
    {0,0}
};

/**
  * @brief  The application entry point.
  */
int main(void)
{
  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_USART2_UART_Init();

  RetargetInit(&huart2);

  rb_init(&rx_buffer);

  printf("System Up and Running\r\n");
  print_prompt();

  while (1)
  {
    uint8_t ch;

    if (rb_get(&rx_buffer, &ch) == 0)
    {
      if (ch == '\r' || ch == '\n')
      {
        printf("\r\n");
        cli_buffer[cli_index] = '\0';

        if (cli_index > 0)
        {
          if (execute_command(cli_buffer) == 0)
            printf("OK\r\n");
          else
            printf("NOK\r\n");
        }

        cli_index = 0;
        print_prompt();
      }
      else if ((ch == '\b') || (ch == 127))
      {
        if (cli_index > 0)
        {
          cli_index--;
          printf("\b \b");
        }
      }
      else if (isprint(ch))
      {
        if (cli_index < CLI_BUFFER_SIZE - 1)
        {
          cli_buffer[cli_index++] = ch;
          printf("%c", ch);
        }
      }
    }
  }
}

/* CLI FUNCTIONS ------------------------------------------------------------*/

static void print_prompt(void)
{
  printf("> ");
}

static int parse_command(uint8_t *line, uint8_t **command, uint8_t **args)
{
  *command = line;

  while (*line && !isspace(*line))
    line++;

  if (*line)
  {
    *line = '\0';
    line++;
    while (isspace(*line)) line++;
    *args = line;
  }
  else
  {
    *args = NULL;
  }

  return 0;
}

static int execute_command(uint8_t *line)
{
  uint8_t *cmd;
  uint8_t *args;

  parse_command(line, &cmd, &args);

  for (int i = 0; commands[i].cmd_string != NULL; i++)
  {
    if (strcmp((char*)cmd, commands[i].cmd_string) == 0)
    {
      commands[i].cmd_function((char*)args);
      return 0;
    }
  }

  printf("Unknown command\r\n");
  return -1;
}

static void help_command(char *args)
{
  printf("Available commands:\r\n");
  printf(" help\r\n");
  printf(" lof\r\n");
  printf(" lon\r\n");
  printf(" test\r\n");
}

static void lof_command(char *args)
{
  printf("LED OFF command received\r\n");
}

static void lon_command(char *args)
{
  printf("LED ON command received\r\n");
}

static void test_command(char *args)
{
  printf("Test command executed\r\n");
}

/* HARDWARE INIT ------------------------------------------------------------*/

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;

  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType =
      RCC_CLOCKTYPE_HCLK |
      RCC_CLOCKTYPE_SYSCLK |
      RCC_CLOCKTYPE_PCLK1 |
      RCC_CLOCKTYPE_PCLK2;

  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);
}

static void MX_USART2_UART_Init(void)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;

  HAL_UART_Init(&huart2);

  __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
}

static void MX_GPIO_Init(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
}

/* Error Handler ------------------------------------------------------------*/

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}
