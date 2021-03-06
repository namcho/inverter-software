/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
#include "main.h"
#include <stm32f3xx_ll_utils.h>
#include "stm32f3xx_it.h"
#include "../Indicators/LedIndicator.h"
#include "../../PwmGenerator/PwmGenerator.h"

// Extern variables
extern uint8_t signal_ms;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

LedIndicator leds;
PwmGenerator pwmGenerator;

int main(void)
{

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  SystemClock_Config();
  MX_GPIO_Init();

  leds.hardwareInit();
  leds.setLedBlinkPeriod(500);

  pwmGenerator.hardwareInit();


  while (1){

	  if(signal_ms){

		  leds.indicationStateMachine();

		  signal_ms = 0;

//		  (getTicker() % 2 ==  0) ?
//		  HRTIM1->sTimerxRegs[0].SETx1R = HRTIM_SET1R_SST:
//		  HRTIM1->sTimerxRegs[0].RSTx1R = HRTIM_RST1R_SRT;
	  }

  }

}
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_2)
  {
  }
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {

  }

//  LL_RCC_HSI_Enable();
//  while(LL_RCC_HSI_IsReady() != 1){
//
//  }

  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_4, LL_RCC_PLL_MUL_16);	// PLL-CLK 72MHz eger HSE 16MHz ise
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_Init1msTick(64000000);
  LL_SetSystemCoreClock(64000000);
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

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

void DelayMs(uint32_t ms){

	uint32_t ticker;
	uint32_t tick_end;

	tick_end = getTicker() + ms;

	for(;;){

		ticker = getTicker();
		if(ticker >= tick_end){
			break;
		}
	}
}

void HRTIM1_TIMA_IRQHandler(void){

	static uint32_t toggle;
	// Clear repetation flag
	HRTIM1->sTimerxRegs[0].TIMxICR |= (1 << 4);

//	if(toggle % 2){
//		GPIOA->BSRR = (1 << 8);
//		GPIOA->BSRR = (1 << 9);
//		GPIOB->BSRR = (1 << 4);
//	}
//	else{
//		GPIOA->BSRR = (1 << 24);
//		GPIOA->BSRR = (1 << 25);
//		GPIOB->BSRR = (1 << 20);
//	}
	pwmGenerator.runController();
	toggle++;
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
