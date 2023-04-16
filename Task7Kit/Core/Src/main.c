/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "stdlib.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define READ 0x03
#define MEM_ERASE 0x60
#define BYTE_WRITE 0x02
#define RDSR 0x05
#define WREN 0x06
#define WRSR 0x01
#define EWSR 0x50

#define MAX_SECTORS 0x1ff
#define MEM_SIZE Ox1fffff
#define SECTOR_SIZE 0xfff
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define ARRAY_SIZE(x) (sizeof((x)) / sizeof((x)[0]))
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
enum
{
	SPIShift_ON,
	SPIShift_OFF
};

enum
{
	Read,
	Write
};

const uint8_t *capsule[] =

				{
						"From: Lezhenin Pavlo, lezheninpasha@gmail.com\n",
						"Mentor: Artem Dovhal, artem.dovhal@globallogic.com\n",
						"Date 14.04.23\n", "TIME CAPSULE\n",
						">>> The Litany of Ignition <<<\n",
						"The soul of the Machine God surrounds thee\n",
						"The power of the Machine God invests thee\n",
						"The hate of the Machine God drives thee\n",
						"The Machine God endows thee with life\n",
						"Live!\n"
				};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE BEGIN PFP */
int _write(int file, char *ptr, int len)
{
	HAL_UART_Transmit(&huart3, (uint8_t*) ptr, len, 10);
	return len;
}
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
  MX_SPI1_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  void SPIShift(_Bool status)
  {
	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, status);
  }

  void SPICommand(uint8_t* tx, uint8_t* rx, uint32_t sizeTx, uint32_t sizeRx, _Bool WR)
  {
	  uint8_t writeCommand = WREN;
	  if (WR == Write)
	  {
		  SPIShift(SPIShift_ON);
		  HAL_SPI_Transmit(&hspi1, &writeCommand, 1, 100);
		  SPIShift(SPIShift_OFF);

		  SPIShift(SPIShift_ON);
		  HAL_SPI_Transmit(&hspi1, tx, sizeTx, 100);
		  SPIShift(SPIShift_OFF);
	  }
	  else
	  {
		  SPIShift(SPIShift_ON);
		  HAL_SPI_Transmit(&hspi1, tx, sizeTx, 100);
		  HAL_SPI_Receive(&hspi1, rx, sizeRx, 100);
		  SPIShift(SPIShift_OFF);
	  }
  }

  void readMem(void)
  {
  	uint8_t readData;
  	uint16_t emptySector;
  	uint16_t addrSect = 0x00;
  	uint16_t addrCell = 0x00;
  	uint8_t readCommand[4];

  	for (addrSect = 0; addrSect <= MAX_SECTORS; addrSect++)
  	{
  		if (!emptySector)
  			printf("\r\nSector #0x%x (%dKb) //", addrSect, addrSect * 4);
  		for (addrCell = 0; addrCell <= SECTOR_SIZE; addrCell++)
  		{
  			readCommand[0] = READ;
  			readCommand[1] = (uint8_t) (addrSect >> 4);
  			readCommand[2] = (uint8_t) ((addrSect << 4) | (addrCell >> 8));
  			readCommand[3] = (uint8_t) (addrCell & 0xff);
  			SPICommand(readCommand, &readData, 4, 1, Read);
  			if (readData != 0xff)
  			{
  				switch (emptySector)
  				{
  					case 0:
  						printf("%c", readData);
  						emptySector = 0;
  						break;
  					case 1:
  						printf("Sector #0x%x (%dKb) //Empty\r\n", addrSect - 1, (addrSect - 1) * 4);
  						printf("\r\nsector #0x%x (%dKb) //%c", addrSect, addrSect * 4, readData);
  						emptySector = 0;
  						break;
  					default:
  						printf("Sector #0x%x...#0x%x //Empty\r\n", addrSect - emptySector, addrSect - 1);
  						printf("\r\nSector #0x%x (%dKb) //%c", addrSect, addrSect * 4, readData);
  						emptySector = 0;
  						break;
  				}
  			}
  			else if(addrCell == 0x00)
  			{
  				if(!emptySector)
  					printf("\r");
  				emptySector++;
  				break;
  			}

  			else
  				break;
  		}
  	}
  	if (emptySector)
  		{
  			printf("Sector #0x%x...#0x%x //Empty\r\n\n", (MAX_SECTORS - emptySector) + 1, MAX_SECTORS);
  		}
  }

  void isItReceive(void)
  {
  	uint8_t readCommand = RDSR;
  	uint8_t readRegister = 0;
  	do
  	SPICommand(&readCommand, &readRegister, 1,1 , Read);
  	while(readRegister & 0x01);
  }

  void eraseMem()
  {
	  uint8_t tx = MEM_ERASE;
	  SPICommand(&tx, NULL, 1, NULL, Write);
	  isItReceive();
	  printf("Memory clear\r\n");
  }

  void writeMem()
  {
	  eraseMem();
	  uint8_t* capsuleString;
	  uint16_t addrSect = 0x00;
	  uint16_t addrCell = 0x00;
	  uint8_t byteCommand[5] = { 0 };
	  for (addrSect = 0; addrSect < ARRAY_SIZE(capsule); addrSect++)
	  {
		  capsuleString = capsule[addrSect];
		  for (addrCell = 0; *capsuleString != '\0'; addrCell++, capsuleString++)
		  {
				byteCommand[0] = BYTE_WRITE;
				byteCommand[1] = (uint8_t)(addrSect >> 4);
				byteCommand[2] = (uint8_t)((addrSect << 4) | (addrCell >> 8));
				byteCommand[3] = (uint8_t)(addrCell & 0xff);
				byteCommand[4] = *capsuleString;
				SPICommand(byteCommand, NULL, 5, NULL, Write);

				if (*capsuleString == '\r')
				{
					addrCell++;
					byteCommand[0] = BYTE_WRITE;
					byteCommand[1] = (uint8_t) (addrSect >> 4);
					byteCommand[2] = (uint8_t) (addrSect << 4) | (addrCell >> 8);
					byteCommand[3] = (uint8_t) (addrCell & 0xff);
					byteCommand[4] = '\n';
					SPICommand(byteCommand, NULL, 5, NULL, Write);
				}
		  }
	  }
	  printf("Memory written\r\n");
  }


  uint8_t TxProtect [2] = { 0 };
  SPIShift(0);
  TxProtect[0] = EWSR;
  HAL_SPI_Transmit(&hspi1, TxProtect, 1, 100);
  SPIShift(1);
  SPIShift(0);
  TxProtect[0] = WRSR;
  TxProtect[1] = 0;
  HAL_SPI_Transmit(&hspi1, TxProtect, 2, 100);
  SPIShift(1);

  HAL_Delay(10);

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);

  printf("If you want read memory send 1\r\n");
  printf("If you want clear memory send 2\r\n");
  printf("If you want write memory send 3\r\n");
  while (1)
  {
	  uint8_t Buf[1];
  	  HAL_StatusTypeDef result;
  	  result = HAL_UART_Receive(&huart3, Buf, 1, 10);
  	  if (result == HAL_OK)
  	  {
  		  switch (Buf[0])
  		  {
  		  	  case '1':
  		  		readMem();
  		  		break;

  		  	  case '2':
  		  		eraseMem();
  		  		break;

  		  	  case '3':
  		  		writeMem();
  		  		break;
  		  	  default:
  		  		printf("UnexpCmd\r\n");
  		  		break;
  		  }
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin : PD7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

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

#ifdef  USE_FULL_ASSERT
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
