/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
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
#include "usart.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_tx;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_tx;

/**
  * @brief buffer to receive 1 character
  */
uint8_t charRx;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 112500;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_EnableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }

}

/* USART2 init function */

void MX_USART2_UART_Init(void)
{
  huart2.Instance = USART1;
  huart2.Init.BaudRate = 112500;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_EnableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
  /** Initializes the peripherals clocks
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    PeriphClkInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_SYSCLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PB7     ------> USART1_RX
    PB6     ------> USART1_TX
    */
    GPIO_InitStruct.Pin = USARTx_RX_Pin|USARTx_TX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USART1 DMA Init */
    /* USART1_TX Init */
    hdma_usart1_tx.Instance = DMA1_Channel5;
    hdma_usart1_tx.Init.Request = DMA_REQUEST_USART1_TX;
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
    {
      Error_Handler();
    }

    // if (HAL_DMA_ConfigChannelAttributes(&hdma_usart1_tx, DMA_CHANNEL_NPRIV) != HAL_OK)
    // {
      // Error_Handler();
    // }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart1_tx);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
	  /** Initializes the peripherals clocks
	        */
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART2;
	PeriphClkInitStruct.Usart1ClockSelection = RCC_USART2CLKSOURCE_SYSCLK;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
	{
	  Error_Handler();
	}

	/* USART1 clock enable */
	__HAL_RCC_USART2_CLK_ENABLE();

	__HAL_RCC_GPIOA_CLK_ENABLE();
	/**USART1 GPIO Configuration
	PA3     ------> USART1_RX
	PA2     ------> USART1_TX
	*/
	GPIO_InitStruct.Pin = USART2_RX_Pin|USART2_TX_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* USART1 DMA Init */
	/* USART1_TX Init */
	hdma_usart2_tx.Instance = DMA1_Channel6;
	hdma_usart2_tx.Init.Request = DMA_REQUEST_USART2_TX;
	hdma_usart2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
	hdma_usart2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_usart2_tx.Init.MemInc = DMA_MINC_ENABLE;
	hdma_usart2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_usart2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_usart2_tx.Init.Mode = DMA_NORMAL;
	hdma_usart2_tx.Init.Priority = DMA_PRIORITY_LOW;
	if (HAL_DMA_Init(&hdma_usart2_tx) != HAL_OK)
	{
	  Error_Handler();
	}

	// if (HAL_DMA_ConfigChannelAttributes(&hdma_usart1_tx, DMA_CHANNEL_NPRIV) != HAL_OK)
	// {
	  // Error_Handler();
	// }

	__HAL_LINKDMA(uartHandle,hdmatx,hdma_usart2_tx);

	/* USART2 interrupt Init */
	HAL_NVIC_SetPriority(USART2_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(USART2_IRQn);
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PB7     ------> USART1_RX
    PB6     ------> USART1_TX
    */
    HAL_GPIO_DeInit(GPIOB, USARTx_RX_Pin|USARTx_TX_Pin);

    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
	/* USER CODE BEGIN USART2_MspDeInit 0 */

	/* USER CODE END USART2MspDeInit 0 */
	  /* Peripheral clock disable */
	  __HAL_RCC_USART2_CLK_DISABLE();

	  /**USART2 GPIO Configuration
	  PA3     ------> USART1_RX
	  PA2     ------> USART1_TX
	  */
	  HAL_GPIO_DeInit(GPIOA, USART2_RX_Pin|USART2_TX_Pin);

	  /* USART2 DMA DeInit */
	  HAL_DMA_DeInit(uartHandle->hdmatx);

	  /* USART2 interrupt Deinit */
	  HAL_NVIC_DisableIRQ(USART1_IRQn);
	/* USER CODE BEGIN USART2_MspDeInit 1 */

	/* USER CODE END USART2_MspDeInit 1 */
  }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  /* USER CODE BEGIN HAL_UART_TxCpltCallback_1 */

  /* USER CODE END HAL_UART_TxCpltCallback_1 */
  /* buffer transmission complete*/
  if (huart->Instance == USART1)
  {
    TxCpltCallback(NULL);
  }
  else if (huart->Instance == USART2)
  {
    TxCpltCallback(NULL);
  }
  /* USER CODE BEGIN HAL_UART_TxCpltCallback_2 */

  /* USER CODE END HAL_UART_TxCpltCallback_2 */
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* USER CODE BEGIN HAL_UART_RxCpltCallback_1 */

  /* USER CODE END HAL_UART_RxCpltCallback_1 */
  if (huart->Instance == USART1)
  {
    if ((NULL != RxCpltCallback) && (HAL_UART_ERROR_NONE == huart->ErrorCode))
    {
      RxCpltCallback(&charRx, 1, 0);
    }
    HAL_UART_Receive_IT(huart, &charRx, 1);
  }
  else if (huart->Instance == USART2)
  {
	if ((HAL_UART_ERROR_NONE == huart->ErrorCode))
	{
	}
	HAL_UART_Receive_IT(huart, &charRx, 1);
  }
  /* USER CODE BEGIN HAL_UART_RxCpltCallback_2 */

  /* USER CODE END HAL_UART_RxCpltCallback_2 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
