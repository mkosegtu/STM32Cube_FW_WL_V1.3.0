/*!
 * @file DFRobot_LTR390UV.h
 * @brief This is the user manual of LTR390UV
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [TangJie](jie.tang@dfrobot.com)
 * @version  V1.0
 * @date  2022-05-17
 * @url https://github.com/DFRobor/DFRobot_LTR390UV
 */
#ifndef LTR390UV_H
#define LTR390UV_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stdint.h"
#include "stddef.h"
#include "i2c.h"

#define LTR390UV_I2CADDR_DEFAULT 0x53 ///< I2C address
#define LTR390UV_MAIN_CTRL 0x00       ///< Main control register
#define LTR390UV_MEAS_RATE 0x04       ///< Resolution and data rate
#define LTR390UV_GAIN 0x05            ///< ALS and UVS gain range
#define LTR390UV_PART_ID 0x06         ///< Part id/revision register
#define LTR390UV_MAIN_STATUS 0x07     ///< Main status register
#define LTR390UV_ALSDATA 0x0D         ///< ALS data lowest byte
#define LTR390UV_UVSDATA 0x10         ///< UVS data lowest byte
#define LTR390UV_INT_CFG 0x19         ///< Interrupt configuration
#define LTR390UV_INT_PST 0x1A         ///< Interrupt persistance config
#define LTR390UV_THRESH_UP 0x21       ///< Upper threshold, low byte
#define LTR390UV_THRESH_LOW 0x24      ///< Lower threshold, low byte

/**
 * @enum enum 
 * @brief Set data-collecting mode of module
 */
typedef enum{
  eALSMode = 0x02,
  eUVSMode = 0x0A
}eModel_t;

/**
 * @enum enum 
 * @brief Set module gain
 */
typedef enum{
  eGain1 = 0,
  eGain3,
  eGain6,
  eGain9,
  eGain18
}eGainRange;
/**
 * @enum enum 
 * @brief Set resolution
 */
typedef enum{
  e20bit = 0,
  e19bit = 16,
  e18bit = 32,
  e17bit = 48,
  e16bit = 64,
  e13bit = 80
}eResolution;

/**
 * @enum enum 
 * @brief Set sampling time
 */
typedef enum{
  e25ms = 0,
  e50ms = 1,
  e100ms = 2,
  e200ms = 3,
  e500ms = 4,
  e1000ms = 5,
  e2000ms = 6
}eMeasurementRate;

/**
 * @fn setMode
 * @brief Set data-collecting mode of module
 * @param mode Data-collecting mode select
 * @return NONE
 */
void ltr390uv_set_mode(eModel_t data);

/**
 * @fn setALSOrUVSMeasRate
 * @brief Set resolution and sampling time of module, the sampling time must be greater than the time for collecting resolution
 * @n --------------------------------------------------------------------------------------------------------
 * @n |    bit7    |    bit6    |    bit5    |    bit4    |    bit3    |    bit2    |    bit1    |    bit0    |
 * @n ---------------------------------------------------------------------------------------------------------
 * @n |  Reserved  |        ALS/UVS Resolution            |  Reserved  |   ALS/UVS Measurement Rate           |
 * @n ---------------------------------------------------------------------------------------------------------
 * @n | ALS/UVS Resolution       |000|20 Bit, Conversion time = 400ms                                         |
 * @n |                          |001|19 Bit, Conversion time = 200ms                                         |
 * @n |                          |010|18 Bit, Conversion time = 100ms(default)                                |
 * @n |                          |011|17 Bit, Conversion time = 50ms                                          |
 * @n |                          |100|16 Bit, Conversion time = 25ms                                          |
 * @n |                          |101|13 Bit, Conversion time = 12.5ms                                          |
 * @n |                          |110/111|Reserved                                                            |
 * @n ---------------------------------------------------------------------------------------------------------
 * @n | ALS/UVS Measurement Rate |000|25ms                                                                    |
 * @n |                          |001|50ms                                                                    |
 * @n |                          |010|100ms (default)                                                         |
 * @n |                          |011|200ms                                                                   |
 * @n |                          |100|500ms                                                                   |
 * @n |                          |101|1000ms                                                                  |
 * @n |                          |110/111|2000ms                                                              |
 * @n ---------------------------------------------------------------------------------------------------------
 * @param bit Set bit depth
 * @param time Set sampling time
 * @return None
 */
void ltr390uv_set_als_or_uvs_meas_rate(eResolution bit, eMeasurementRate time);

/**
 * @fn setALSOrUVSGain
 * @brief Set sensor gain
 * @n ---------------------------------------------------------------------------------------------------------
 * @n |    bit7    |    bit6    |    bit5    |    bit4    |    bit3    |    bit2    |    bit1    |    bit0    |
 * @n ---------------------------------------------------------------------------------------------------------
 * @n |                                    Reserved                    |          ALS/UVS Gain Range          |
 * @n ---------------------------------------------------------------------------------------------------------
 * @n | ALS/UVS Gain Range       |000|Gain Range: 1                                                           |
 * @n |                          |001|Gain Range: 3 (default)                                                 |
 * @n |                          |010|Gain Range: 6                                                           |
 * @n |                          |011|Gain Range: 9                                                           |
 * @n |                          |100|Gain Range: 18                                                          |
 * @n |                          |110/111|Reserved                                                            |
 * @n ---------------------------------------------------------------------------------------------------------                  
 * @param data Control data 
 * @return None
 */
void ltr390uv_set_als_or_uvs_gain(uint8_t data);

/**
 * @fn readData
 * @brief Get raw data
 * @return Return the obtained raw data
 */
uint32_t ltr390uv_read_original_data(void);

/**
 * @fn readALSTransformData
 * @brief Get the converted ALS data
 * @return Return the converted data
 */
void ltr390uv_read_als_transform_data(float* data);

int ltr390uv_init(void);

int ltr390uv_read_id(void);

void ltr390uv_read_uvs_data(float* data);

#endif
