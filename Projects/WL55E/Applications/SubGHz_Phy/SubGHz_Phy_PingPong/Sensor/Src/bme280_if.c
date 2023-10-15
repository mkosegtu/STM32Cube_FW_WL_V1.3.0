/*! @file bme280.c
 * @brief Sensor driver for BME280 sensor
 */
#include "bme280.h"
#include "i2c.h"

#define SAMPLE_COUNT  UINT8_C(50)


static uint8_t dev_addr;

static int8_t get_humidity(uint32_t period, struct bme280_dev *dev);
static int8_t get_pressure(uint32_t period, struct bme280_dev *dev);
static int8_t get_temperature(uint32_t period, struct bme280_dev *dev);
uint32_t period;
struct bme280_dev dev;
/*!
 * I2C read function map to COINES platform
 */
BME280_INTF_RET_TYPE bme280_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t length, void *intf_ptr)
{
	HAL_StatusTypeDef ret;
	ret = HAL_I2C_Mem_Read(&hi2c2, BME280_I2C_ADDR_SEC << 1, (uint16_t)reg_addr,
      		I2C_MEMADD_SIZE_8BIT, reg_data, (uint16_t)length, 10000);
    if(ret == HAL_OK)
		return BME280_INTF_RET_SUCCESS;
	return BME280_E_NULL_PTR;
}

/*!
 * I2C write function map to COINES platform
 */
BME280_INTF_RET_TYPE bme280_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t length, void *intf_ptr)
{
	HAL_StatusTypeDef ret;
    ret = HAL_I2C_Mem_Write(&hi2c2, BME280_I2C_ADDR_SEC << 1, (uint16_t)reg_addr,
          		I2C_MEMADD_SIZE_8BIT, reg_data, (uint16_t)length, 10000);
    if(ret == HAL_OK)
    	return BME280_INTF_RET_SUCCESS;
    return BME280_E_NULL_PTR;
}

/*!
 * Delay function map to COINES platform
 */
void bme280_delay_us(uint32_t period, void *intf_ptr)
{
    TIMER_IF_DelayMs(period/1000);
}

/* This function starts the execution of program. */
int8_t bme280_init_sensor(void)
{
    int8_t rslt;
    uint32_t period;
    struct bme280_settings settings;

    /* Interface selection is to be updated as parameter
     * For I2C :  BME280_I2C_INTF
     * For SPI :  BME280_SPI_INTF
     */
    /* Bus configuration : I2C */

	dev_addr = BME280_I2C_ADDR_PRIM;
	dev.read = bme280_i2c_read;
	dev.write = bme280_i2c_write;
	dev.intf = BME280_I2C_INTF;

	/* Holds the I2C device addr or SPI chip selection */
	dev.intf_ptr = &dev_addr;

	/* Configure delay in microseconds */
	dev.delay_us = bme280_delay_us;

    rslt = bme280_init(&dev);
    //bme280_error_codes_print_result("bme280_init", rslt);

    /* Always read the current settings before writing, especially when all the configuration is not modified */
    rslt = bme280_get_sensor_settings(&settings, &dev);
    //bme280_error_codes_print_result("bme280_get_sensor_settings", rslt);

    /* Configuring the over-sampling rate, filter coefficient and standby time */
    /* Overwrite the desired settings */
    settings.filter = BME280_FILTER_COEFF_2;

    /* Over-sampling rate for humidity, temperature and pressure */
    settings.osr_h = BME280_OVERSAMPLING_1X;
    settings.osr_p = BME280_OVERSAMPLING_1X;
    settings.osr_t = BME280_OVERSAMPLING_1X;

    /* Setting the standby time */
    settings.standby_time = BME280_STANDBY_TIME_0_5_MS;

    rslt = bme280_set_sensor_settings(BME280_SEL_ALL_SETTINGS, &settings, &dev);
    //bme280_error_codes_print_result("bme280_set_sensor_settings", rslt);

    /* Always set the power mode after setting the configuration */
    rslt = bme280_set_sensor_mode(BME280_POWERMODE_NORMAL, &dev);
    //bme280_error_codes_print_result("bme280_set_power_mode", rslt);

    /* Calculate measurement time in microseconds */
    rslt = bme280_cal_meas_delay(&period, &settings);
    //bme280_error_codes_print_result("bme280_cal_meas_delay", rslt);

    return 0;
}

int8_t bme280_get_data(struct bme280_data* comp_data)
{
	int8_t rslt = BME280_E_NULL_PTR;
	int8_t idx = 0;
	uint8_t status_reg;
	uint8_t sensor_comp = BME280_PRESS | BME280_TEMP | BME280_HUM;

	rslt = bme280_get_regs(BME280_REG_STATUS, &status_reg, 1, &dev);
	//bme280_error_codes_print_result("bme280_get_regs", rslt);

	if (status_reg & BME280_STATUS_MEAS_DONE)
	{
		/* Measurement time delay given to read sample */
		dev.delay_us(period, dev.intf_ptr);

		/* Read compensated data */
		rslt = bme280_get_sensor_data(sensor_comp, &comp_data, &dev);
		//bme280_error_codes_print_result("bme280_get_sensor_data", rslt);

#ifndef BME280_DOUBLE_ENABLE
		comp_data.humidity = comp_data.humidity / 1000;
#endif
	}

	return rslt;
}

/*!
 *  @brief This internal API is used to get compensated humidity data.
 */
static int8_t get_humidity(uint32_t period, struct bme280_dev *dev)
{
    int8_t rslt = BME280_E_NULL_PTR;
    int8_t idx = 0;
    uint8_t status_reg;
    struct bme280_data comp_data;

    while (idx < SAMPLE_COUNT)
    {
        rslt = bme280_get_regs(BME280_REG_STATUS, &status_reg, 1, dev);
        //bme280_error_codes_print_result("bme280_get_regs", rslt);

        if (status_reg & BME280_STATUS_MEAS_DONE)
        {
            /* Measurement time delay given to read sample */
            dev->delay_us(period, dev->intf_ptr);

            /* Read compensated data */
            rslt = bme280_get_sensor_data(BME280_HUM, &comp_data, dev);
            //bme280_error_codes_print_result("bme280_get_sensor_data", rslt);

#ifndef BME280_DOUBLE_ENABLE
            comp_data.humidity = comp_data.humidity / 1000;
#endif

//#ifdef BME280_DOUBLE_ENABLE
//            printf("Humidity[%d]:   %lf %%RH\n", idx, comp_data.humidity);
//#else
//            printf("Humidity[%d]:   %lu %%RH\n", idx, (long unsigned int)comp_data.humidity);
//#endif
            idx++;
        }
    }

    return rslt;
}

/*!
 *  @brief This internal API is used to get compensated pressure data.
 */
static int8_t get_pressure(uint32_t period, struct bme280_dev *dev)
{
    int8_t rslt = BME280_E_NULL_PTR;
    int8_t idx = 0;
    uint8_t status_reg;
    struct bme280_data comp_data;

    while (idx < SAMPLE_COUNT)
    {
        rslt = bme280_get_regs(BME280_REG_STATUS, &status_reg, 1, dev);
        //bme280_error_codes_print_result("bme280_get_regs", rslt);

        if (status_reg & BME280_STATUS_MEAS_DONE)
        {
            /* Measurement time delay given to read sample */
            dev->delay_us(period, dev->intf_ptr);

            /* Read compensated data */
            rslt = bme280_get_sensor_data(BME280_PRESS, &comp_data, dev);
            //bme280_error_codes_print_result("bme280_get_sensor_data", rslt);

#ifdef BME280_64BIT_ENABLE
            comp_data.pressure = comp_data.pressure / 100;
#endif

//#ifdef BME280_DOUBLE_ENABLE
//            printf("Pressure[%d]:  %lf Pa\n", idx, comp_data.pressure);
//#else
//            printf("Pressure[%d]:   %lu Pa\n", idx, (long unsigned int)comp_data.pressure);
//#endif
            idx++;
        }
    }

    return rslt;
}


/*!
 *  @brief This internal API is used to get compensated temperature data.
 */
static int8_t get_temperature(uint32_t period, struct bme280_dev *dev)
{
    int8_t rslt = BME280_E_NULL_PTR;
    int8_t idx = 0;
    uint8_t status_reg;
    struct bme280_data comp_data;

    while (idx < SAMPLE_COUNT)
    {
        rslt = bme280_get_regs(BME280_REG_STATUS, &status_reg, 1, dev);
        //bme280_error_codes_print_result("bme280_get_regs", rslt);

        if (status_reg & BME280_STATUS_MEAS_DONE)
        {
            /* Measurement time delay given to read sample */
            dev->delay_us(period, dev->intf_ptr);

            /* Read compensated data */
            rslt = bme280_get_sensor_data(BME280_TEMP, &comp_data, dev);
            //bme280_error_codes_print_result("bme280_get_sensor_data", rslt);

#ifndef BME280_DOUBLE_ENABLE
            comp_data.temperature = comp_data.temperature / 100;
#endif

//#ifdef BME280_DOUBLE_ENABLE
//            printf("Temperature[%d]:   %lf deg C\n", idx, comp_data.temperature);
//#else
//            printf("Temperature[%d]:   %ld deg C\n", idx, (long int)comp_data.temperature);
//#endif
            idx++;
        }
    }

    return rslt;
}
