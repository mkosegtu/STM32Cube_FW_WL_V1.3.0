/*!
 * @file DFRobot_LTR390UV.cpp
 * @brief This is the method implementation file of LTR390UV
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [TangJie](jie.tang@dfrobot.com)
 * @version  V1.0
 * @date  2022-05-17
 * @url https://github.com/DFRobor/DFRobot_LTR390UV
 */

#include "ltr390uv.h"

uint8_t a_gain[5] = {1,3,6,9,18};
double a_int[6] = {4.,2.,1.,0.5,0.25,0.03125};
uint8_t gain = 0;
uint8_t resolution = 0;
uint8_t mode = 0;
uint8_t addr = 0xA6;

static uint8_t ltr390uv_read_reg(uint8_t reg, uint8_t *pBuf, uint16_t size);

static uint8_t ltr390uv_write_reg(uint8_t reg, uint8_t *pBuf, uint16_t size);

void ltr390uv_set_mode(eModel_t data)
{
  mode = data;
  ltr390uv_write_reg(LTR390UV_MAIN_CTRL, &mode, 1);
}

void ltr390uv_set_als_or_uvs_meas_rate(eResolution bit, eMeasurementRate time)
{
  uint8_t send_data;
  uint8_t data = bit+time;
  resolution = (data&0xf0)>>4;
  send_data = data;
  ltr390uv_write_reg(LTR390UV_MEAS_RATE, &send_data, 1);
}

void ltr390uv_set_als_or_uvs_gain(uint8_t data)
{
  gain = data;
  ltr390uv_write_reg(LTR390UV_GAIN, &gain, 1);
}

uint32_t ltr390uv_read_original_data(void)
{
  float data = 0;
  uint32_t original_data = 0;
  uint8_t buffer[3];

  if(mode == eALSMode)
	ltr390uv_read_reg(LTR390UV_ALSDATA, buffer, 3);
  else
	ltr390uv_read_reg(LTR390UV_UVSDATA, buffer, 3);

  original_data = (buffer[2]<<16|buffer[1]<<8|buffer[0]) & 0xFFFFF;
  data = original_data;

  return data;
}

float ltr390uv_read_als_transform_data(void)
{
  float data=0.0;
  uint32_t original_data = 0;
  uint8_t buffer[3];
  if(mode == eALSMode){
	ltr390uv_read_reg(LTR390UV_ALSDATA, buffer, 3);
	original_data = (buffer[2]<<16|buffer[1]<<8|buffer[0]) & 0xFFFFF;
    data = (0.6*original_data)/(a_gain[gain]*a_int[resolution]);
  }
  return data;
}

static uint8_t ltr390uv_read_reg(uint8_t reg, uint8_t *pBuf, uint16_t size)
{
  uint8_t ret = 0;
  ret = HAL_I2C_Mem_Read(&hi2c2, (uint16_t)addr, (uint16_t)reg,
  		I2C_MEMADD_SIZE_8BIT, pBuf, size, 10000);
  return ret;
}

static uint8_t ltr390uv_write_reg(uint8_t reg, uint8_t *pBuf, uint16_t size)
{
  uint8_t ret = 0;
  ret = HAL_I2C_Mem_Write(&hi2c2, (uint16_t)addr, (uint16_t)reg,
  		I2C_MEMADD_SIZE_8BIT, pBuf, size, 10000);
  return ret;
}
