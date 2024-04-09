#include "i2c.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "bmp3.h"

struct bmp3_dev bmp3_dev;

/*! BMP3 shuttle board ID */
#define BMP3_SHUTTLE_ID  0xD3

/* Variable to store the device address */
static uint8_t dev_addr;

/*!
 * I2C read function map to COINES platform
 */
BMP3_INTF_RET_TYPE bmp3_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr)
{
    HAL_StatusTypeDef ret;
	ret = HAL_I2C_Mem_Read(&hi2c2, dev_addr << 1, (uint16_t)reg_addr,
			I2C_MEMADD_SIZE_8BIT, reg_data, (uint16_t)len, 10000);
	if(ret == HAL_OK)
		return BMP3_OK;
	return BMP3_E_COMM_FAIL;
}

/*!
 * I2C write function map to COINES platform
 */
BMP3_INTF_RET_TYPE bmp3_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr)
{
	HAL_StatusTypeDef ret;
	ret = HAL_I2C_Mem_Write(&hi2c2, dev_addr << 1, (uint16_t)reg_addr,
				I2C_MEMADD_SIZE_8BIT, reg_data, (uint16_t)len, 10000);
	if(ret == HAL_OK)
		return BMP3_OK;
	return BMP3_E_COMM_FAIL;
}

/*!
 * SPI read function map to COINES platform
 */
BMP3_INTF_RET_TYPE bmp3_spi_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr)
{

}

/*!
 * SPI write function map to COINES platform
 */
BMP3_INTF_RET_TYPE bmp3_spi_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr)
{
}

/*!
 * Delay function map to COINES platform
 */
void bmp3_delay_us(uint32_t period, void *intf_ptr)
{
    TIMER_IF_DelayMs(period/1000);
}

void bmp3_check_rslt(const char api_name[], int8_t rslt)
{
    switch (rslt)
    {
        case BMP3_OK:

            /* Do nothing */
            break;
        case BMP3_E_NULL_PTR:
            printf("API [%s] Error [%d] : Null pointer\r\n", api_name, rslt);
            break;
        case BMP3_E_COMM_FAIL:
            printf("API [%s] Error [%d] : Communication failure\r\n", api_name, rslt);
            break;
        case BMP3_E_INVALID_LEN:
            printf("API [%s] Error [%d] : Incorrect length parameter\r\n", api_name, rslt);
            break;
        case BMP3_E_DEV_NOT_FOUND:
            printf("API [%s] Error [%d] : Device not found\r\n", api_name, rslt);
            break;
        case BMP3_E_CONFIGURATION_ERR:
            printf("API [%s] Error [%d] : Configuration Error\r\n", api_name, rslt);
            break;
        case BMP3_W_SENSOR_NOT_ENABLED:
            printf("API [%s] Error [%d] : Warning when Sensor not enabled\r\n", api_name, rslt);
            break;
        case BMP3_W_INVALID_FIFO_REQ_FRAME_CNT:
            printf("API [%s] Error [%d] : Warning when Fifo watermark level is not in limit\r\n", api_name, rslt);
            break;
        default:
            printf("API [%s] Error [%d] : Unknown error code\r\n", api_name, rslt);
            break;
    }
}

BMP3_INTF_RET_TYPE bmp3_interface_init(struct bmp3_dev *bmp3, uint8_t intf)
{
    int8_t rslt = BMP3_OK;

    if (bmp3 != NULL)
    {

        /* Bus configuration : I2C */
        if (intf == BMP3_I2C_INTF)
        {
            printf("I2C Interface\n");
            dev_addr = BMP3_ADDR_I2C_SEC;
            bmp3->read = bmp3_i2c_read;
            bmp3->write = bmp3_i2c_write;
            bmp3->intf = BMP3_I2C_INTF;
        }
        /* Bus configuration : SPI */
        else if (intf == BMP3_SPI_INTF)
        {
            printf("SPI Interface\n");
            dev_addr = 0;
            bmp3->read = bmp3_spi_read;
            bmp3->write = bmp3_spi_write;
            bmp3->intf = BMP3_SPI_INTF;
        }

        bmp3->delay_us = bmp3_delay_us;
        bmp3->intf_ptr = &dev_addr;
    }
    else
    {
        rslt = BMP3_E_NULL_PTR;
    }

    return rslt;
}

int bmp3_sensor_init(void)
{
    int8_t rslt;
    uint16_t settings_sel;
    struct bmp3_settings settings = { 0 };

    /* Interface reference is given as a parameter
     *         For I2C : BMP3_I2C_INTF
     *         For SPI : BMP3_SPI_INTF
     */
    rslt = bmp3_interface_init(&bmp3_dev, BMP3_I2C_INTF);
    bmp3_check_rslt("bmp3_interface_init", rslt);

    rslt = bmp3_init(&bmp3_dev);
    bmp3_check_rslt("bmp3_init", rslt);

    settings.int_settings.drdy_en = BMP3_ENABLE;
    settings.press_en = BMP3_ENABLE;
    settings.temp_en = BMP3_ENABLE;

    settings.odr_filter.press_os = BMP3_OVERSAMPLING_2X;
    settings.odr_filter.temp_os = BMP3_OVERSAMPLING_2X;
    settings.odr_filter.odr = BMP3_ODR_100_HZ;

    settings_sel = BMP3_SEL_PRESS_EN | BMP3_SEL_TEMP_EN | BMP3_SEL_PRESS_OS | BMP3_SEL_TEMP_OS | BMP3_SEL_ODR |
                   BMP3_SEL_DRDY_EN;

    rslt = bmp3_set_sensor_settings(settings_sel, &settings, &bmp3_dev);
    bmp3_check_rslt("bmp3_set_sensor_settings", rslt);

    settings.op_mode = BMP3_MODE_NORMAL;
    rslt = bmp3_set_op_mode(&settings, &bmp3_dev);
    bmp3_check_rslt("bmp3_set_op_mode", rslt);
    return rslt;
}

int8_t bmp3_get_data(struct bmp3_data *comp_data)
{
	struct bmp3_status status = { { 0 } };
	int8_t rslt = bmp3_get_status(&status, &bmp3_dev);
	bmp3_check_rslt("bmp3_get_status", rslt);

	/* Read temperature and pressure data iteratively based on data ready interrupt */
	if ((rslt == BMP3_OK) && (status.intr.drdy == BMP3_ENABLE))
	{
		/*
		 * First parameter indicates the type of data to be read
		 * BMP3_PRESS_TEMP : To read pressure and temperature data
		 * BMP3_TEMP       : To read only temperature data
		 * BMP3_PRESS      : To read only pressure data
		 */
		rslt = bmp3_get_sensor_data(BMP3_PRESS_TEMP, comp_data, &bmp3_dev);
		bmp3_check_rslt("bmp3_get_sensor_data", rslt);
	}
	return rslt;
}
