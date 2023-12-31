/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    version.h
  * @author  MCD Application Team
  * @brief   Header with version of STM32WL Sensor Concentrator Demo.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __VERSION_H__
#define __VERSION_H__

/**
  * @brief Application version
  */
#define APP_VERSION_MAJOR          1
#define APP_VERSION_MINOR          0

#define VERSION_STR_CONVERT(a) VERSION_STR_HELPER(a)    /*Preprocessor trick to convert constant to string*/
#define VERSION_STR_HELPER(a)  #a

#define APP_VERSION_STR            "v" VERSION_STR_CONVERT(APP_VERSION_MAJOR) ":" VERSION_STR_CONVERT(APP_VERSION_MINOR)

/**
  * @brief Major data format version indicates major changes in behavior.
  * Used when regions/subregions are changed or incompatible behavior.
  * Only 0 ~ 7 fit inside packets.
  */
#define DATA_VERSION_MAJOR          1

/**
  * @brief Minor data format version indicated minor changes in behavior.
  * Used to change sensor data format or compatible changes.
  * Only 0 ~ 31 fit inside packets.
  */
#define DATA_VERSION_MINOR          1

#endif /* __VERSION_H__ */
