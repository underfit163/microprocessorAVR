/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdlib.h"
#include "limits.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define s2() HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_2)
#define s3() HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_3)
#define s4() HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)
#define s5() HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
uint32_t adress = 0x20000160;//адрес дл€ его записи
uint32_t newadress = 0x20000180;//адрес дл€ нового массива
int16_t k = 0x4422;//константа
uint8_t len = 4;//размер массива
uint8_t tx_index=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//загружаем массив в пам€ть

void LoadArray(uint32_t adress, int8_t* array, uint8_t length){
	for(int i=0;i<length;i++){
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,adress+i,array[i]);
	}
}

//читаем элемент
int16_t ReadElement(int32_t adress) {
	return (*(__IO int16_t*) adress);
}

//вычитаем из массива константу с учетом знака
void SubMas(){
	int16_t temp = 0;
	int16_t tempMas = 0;
	int counter = 0;
	for(int i=0; i<len; i++){
		temp = ReadElement(adress + i*2);
		tempMas = temp - k;
		if (tempMas < SHRT_MAX && tempMas > SHRT_MIN) {
			HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,newadress + counter*2,tempMas);
			counter++;
		}
	}
}

int8_t ReadPort(void){
	return HAL_GPIO_ReadPin(Q0_GPIO_Port,Q0_Pin)|
			(HAL_GPIO_ReadPin(Q1_GPIO_Port,Q1_Pin)<<1)|
			(HAL_GPIO_ReadPin(Q2_GPIO_Port,Q2_Pin)<<2)|
			(HAL_GPIO_ReadPin(Q3_GPIO_Port,Q3_Pin)<<3)|
			(HAL_GPIO_ReadPin(Q4_GPIO_Port,Q4_Pin)<<4)|
			(HAL_GPIO_ReadPin(Q5_GPIO_Port,Q5_Pin)<<5)|
			(HAL_GPIO_ReadPin(Q6_GPIO_Port,Q6_Pin)<<6)|
			(HAL_GPIO_ReadPin(Q7_GPIO_Port,Q7_Pin)<<7);
}

void Input(){
	int8_t temp[len*2];
	HAL_GPIO_WritePin(HOLD_GPIO_Port,HOLD_Pin,GPIO_PIN_RESET);//разрешаем передачу ѕ√
	for(int i=0;i<len*2;i++){
		while(HAL_GPIO_ReadPin(CLK_GPIO_Port,CLK_Pin)){
			//ждем clkout
		}
		while(!HAL_GPIO_ReadPin(CLK_GPIO_Port,CLK_Pin)){
			//ждем clkout
		}
		temp[i] = ReadPort();//читаем байт
	}
	HAL_GPIO_WritePin(HOLD_GPIO_Port,HOLD_Pin,GPIO_PIN_SET);//останавливаем передачу ѕ√
	LoadArray(adress, temp, len*2);//загружаем прин€тый массив в ќ«”
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance == USART1 && tx_index<len*2-1){
		HAL_UART_Transmit_IT(&huart1,(int8_t)ReadElement(newadress+tx_index),1);//передаем байт через uart
		tx_index++;
	}
}
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
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  while(!s2() || !s3() || s4() || !s5()) {}//ждем нужного значени€ на входах порта
  Input();
  SubMas();
  HAL_UART_ENABLE_IT(huart1,UART_IT_TXE);//разрешаем прерывани€ uart
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
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
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 2400;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_HalfDuplex_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(HOLD_GPIO_Port, HOLD_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : S2_Pin S3_Pin S4_Pin S5_Pin */
  GPIO_InitStruct.Pin = S2_Pin|S3_Pin|S4_Pin|S5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : Q2_Pin Q3_Pin Q4_Pin Q5_Pin 
                           Q6_Pin Q7_Pin CLK_Pin Q0_Pin 
                           Q1_Pin */
  GPIO_InitStruct.Pin = Q2_Pin|Q3_Pin|Q4_Pin|Q5_Pin 
                          |Q6_Pin|Q7_Pin|CLK_Pin|Q0_Pin 
                          |Q1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : HOLD_Pin */
  GPIO_InitStruct.Pin = HOLD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(HOLD_GPIO_Port, &GPIO_InitStruct);

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
