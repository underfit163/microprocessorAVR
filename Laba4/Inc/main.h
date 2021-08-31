/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define S2_Pin GPIO_PIN_2
#define S2_GPIO_Port GPIOA
#define S3_Pin GPIO_PIN_3
#define S3_GPIO_Port GPIOA
#define S4_Pin GPIO_PIN_4
#define S4_GPIO_Port GPIOA
#define S5_Pin GPIO_PIN_5
#define S5_GPIO_Port GPIOA
#define Q2_Pin GPIO_PIN_10
#define Q2_GPIO_Port GPIOB
#define Q3_Pin GPIO_PIN_11
#define Q3_GPIO_Port GPIOB
#define Q4_Pin GPIO_PIN_12
#define Q4_GPIO_Port GPIOB
#define Q5_Pin GPIO_PIN_13
#define Q5_GPIO_Port GPIOB
#define Q6_Pin GPIO_PIN_14
#define Q6_GPIO_Port GPIOB
#define Q7_Pin GPIO_PIN_15
#define Q7_GPIO_Port GPIOB
#define CLK_Pin GPIO_PIN_4
#define CLK_GPIO_Port GPIOB
#define HOLD_Pin GPIO_PIN_5
#define HOLD_GPIO_Port GPIOB
#define Q0_Pin GPIO_PIN_8
#define Q0_GPIO_Port GPIOB
#define Q1_Pin GPIO_PIN_9
#define Q1_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define HAL_UART_ENABLE_IT(huart1,UART_IT_TXE) for(int i=0;i<8;i++){int8_t byte = (int8_t)ReadElement(0x20000180+i);HAL_UART_Transmit(&huart1,&byte,1,1000);}
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
