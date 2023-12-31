/**
  ******************************************************************************
  * @file    BSP/Inc/stm32wlxx_it.h 
  * @author  MCD Application Team
  * @brief   This file contains the headers of the interrupt handlers.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32WLxx_IT_H
#define __STM32WLxx_IT_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void NMI_Handler(void);
void HardFault_Handler(void);
void SVC_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void DMA1_Channel1_IRQHandler(void);
void USB_LP_IRQHandler(void);

#if !defined(CORE_CM0PLUS)
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void DebugMon_Handler(void);
#endif /* CORE_CM0PLUS */

#if defined(CORE_CM0PLUS)
void EXTI1_0_IRQHandler(void);
void EXTI15_4_IRQHandler(void);
#else
void EXTI0_IRQHandler(void);
void EXTI15_10_IRQHandler(void);
#endif /* CORE_CM0PLUS */

/* TMP */
void TIM1_TRG_COM_TIM17_IRQHandler(void);
/* TMP */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __STM32WLxx_IT_H */
