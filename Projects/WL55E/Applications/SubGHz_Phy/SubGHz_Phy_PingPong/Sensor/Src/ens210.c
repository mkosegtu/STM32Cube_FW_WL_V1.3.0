/*
 * ens210.c
 *
 *  Created on: Apr 8, 2024
 *      Author: Kadir.Kose
 */

#include "ens210.h"
#include "sys_app.h"
#include "math.h"
#include "i2c.h"
#include "stdbool.h"

uint16_t ENS21x_slaveAddress = 0x43;
int16_t solderCorrection;

uint16_t partId;
uint16_t dieRev;
uint64_t uid;

uint16_t ENS21x_tData;
uint16_t ENS21x_hData;
uint32_t ENS21x_tStatus;
uint32_t ENS21x_hStatus;

uint32_t ENS21x_read(enum RegisterAddress reg, uint8_t *buf, uint8_t num)
{
	uint8_t pos = 0;
	uint8_t result = 0;

//	if (debugENS160) {
//		Serial.print("I2C read address: 0x");
//		Serial.print(addr, HEX);
//		Serial.print(" - register: 0x");
//		Serial.println(reg, HEX);
//	}
	result = HAL_I2C_Mem_Read(&hi2c2, ENS21x_slaveAddress << 1, (uint16_t)reg,
	      		I2C_MEMADD_SIZE_8BIT, buf, (uint16_t)num, 10000);

	if(result == HAL_OK)
		return STATUS_OK;
	return STATUS_I2C_ERROR;
}

uint32_t ENS21x_write(enum RegisterAddress reg, uint8_t *buf, uint8_t num) {
//	if (debugENS160) {
//		Serial.print("I2C write address: 0x");
//		Serial.print(addr, HEX);
//		Serial.print(" - register: 0x");
//		Serial.print(reg, HEX);
//		Serial.print(" -  value:");
//		for (int i = 0; i<num; i++) {
//			Serial.print(" 0x");
//			Serial.print(buf[i], HEX);
//		}
//		Serial.println();
//	}
//

	uint8_t result = HAL_I2C_Mem_Write(&hi2c2, ENS21x_slaveAddress << 1, (uint16_t)reg,
	      		I2C_MEMADD_SIZE_8BIT, buf, (uint16_t)num, 10000);
	if(result == HAL_OK)
		return STATUS_OK;
	return STATUS_I2C_ERROR;
}


uint32_t ENS21x_update(uint64_t ms)
{
	HAL_Delay(ms);

	uint8_t buffer[6];
	uint32_t check_data;
	enum Result result = ENS21x_read(T_VAL, buffer, 6);
	if (result == STATUS_OK)
	{
		memcpy(&ENS21x_tData, &buffer[0], 2);
		memcpy(&check_data, &buffer[0], 3);
		ENS21x_tStatus = ENS21x_checkData(check_data);

		memcpy(&ENS21x_hData, &buffer[3], 2);
		memcpy(&check_data, &buffer[3], 3);
		ENS21x_hStatus = ENS21x_checkData(check_data);
	}

	//debug(__func__, result);
	return result;
}

uint32_t ENS21x_singleShotMeasure(uint8_t sensor)
{
	enum Result result = ENS21x_write(SENS_START, &sensor, 1);
	if (result == STATUS_OK)
	{
		result = ENS21x_update(CONVERSION_SINGLE_SHOT);
		if (result == STATUS_OK)
		{
			switch (sensor)
			{
				case TEMPERATURE:   result = ENS21x_tStatus; break;
				case HUMIDITY:      result = ENS21x_hStatus; break;
				case TEMPERATURE_AND_HUMIDITY:
				{
					result = ENS21x_tStatus != STATUS_OK ? ENS21x_tStatus : ENS21x_hStatus;
					break;
				}
			}
		}
	}

	//debug(__func__, result);
	return result;
}

uint32_t ENS21x_startContinuousMeasure(uint8_t sensor)
{
	enum Result result = ENS21x_write(SENS_RUN, &sensor, 1);
	if (result == STATUS_OK)
	{
		result = singleShotMeasure(sensor);
	}

	//debug(__func__, result);
	return result;
}

uint32_t ENS21x_stopContinuousMeasure(uint8_t sensor)
{
	enum Result result = ENS21x_write(SENS_STOP, &sensor, 1);

	//debug(__func__, result);
	return result;
}

uint32_t ENS21x_setLowPower(bool enable)
{
	enum Result result;
	uint8_t buf;
	if (enable)
	{
		buf = ENABLE_LOW_POWER;
	}
	else
	{
		buf = DISABLE_LOW_POWER;
	}
	result = ENS21x_write(SYS_CTRL, &buf, 1);
	return result;
}

uint32_t ENS21x_reset()
{
	uint8_t buf = SENSOR_RESET;
	enum Result result = ENS21x_write(SYS_CTRL, &buf, 1);
	if (result == STATUS_OK)
	{
		HAL_Delay(BOOTING);
	}

	//debug(__func__, result);
	return result;
}

uint16_t ENS21x_getPartId()
{
	return partId;
}

uint16_t ENS21x_getDieRev()
{
	return dieRev;
}

uint64_t ENS21x_getUid()
{
	return uid;
}

void ENS21x_setSolderCorrection(int16_t correction)
{
	solderCorrection = correction;
}

float ENS21x_getTempKelvin()
{
	return (ENS21x_tData - solderCorrection) / 64.f;
}

float ENS21x_getTempCelsius()
{
	return ENS21x_getTempKelvin() - (27315L / 100);
}

float ENS21x_getTempFahrenheit()
{
	// Return m*F. This equals m*(1.8*(K-273.15)+32) = m*(1.8*K-273.15*1.8+32) = 1.8*m*K-459.67*m = 9*m*K/5 - 45967*m/100 = 9*m*t/320 - 45967*m/100
	// Note m is the multiplier, F is temperature in Fahrenheit, K is temperature in Kelvin, t is raw t_data value.
	// Uses F=1.8*(K-273.15)+32 and K=t/64.
	// The first multiplication stays below 32 bits (t:16, multiplier:11, 9:4)
	// The second multiplication stays below 32 bits (multiplier:10, 45967:16)

	return (9.f * (ENS21x_tData - solderCorrection) / 320.f) - 459.67f;
}

float ENS21x_getHumidityPercent()
{
	// Return m*H. This equals m*(h/512) = (m*h)/512
	// Note m is the multiplier, H is the relative humidity in %RH, h is raw h_data value.
	// Uses H=h/512.

	return ENS21x_hData / 512.f;
}

float ENS21x_getAbsoluteHumidityPercent()//TODO cleanup
{
	// taken from https://carnotcycle.wordpress.com/2012/08/04/how-to-convert-relative-humidity-to-absolute-humidity/
	// precision is about 0.1°C in range -30 to 35°C
	// August-Roche-Magnus   6.1094 exp(17.625 x T)/(T + 243.04)
	// Buck (1981)     6.1121 exp(17.502 x T)/(T + 240.97)
	// reference https://www.eas.ualberta.ca/jdwilson/EAS372_13/Vomel_CIRES_satvpformulae.html    // Use Buck (1981)

	static const float MOLAR_MASS_OF_WATER      = 18.01534;
	static const float UNIVERSAL_GAS_CONSTANT   = 8.21447215;
	return (6.1121 * pow(2.718281828,(17.67* ENS21x_getTempCelsius())/(ENS21x_getTempCelsius() + 243.5))* ENS21x_getHumidityPercent() *MOLAR_MASS_OF_WATER)/((273.15+ ENS21x_getTempCelsius() )*UNIVERSAL_GAS_CONSTANT);
}

uint16_t ENS21x_getDataT()
{
	return ENS21x_tData;
}

uint16_t ENS21x_getDataH()
{
	return ENS21x_hData;
}

uint32_t ENS21x_getStatusT()
{
	return ENS21x_tStatus;
}

uint32_t ENS21x_getStatusH()
{
	return ENS21x_hStatus;
}

void ENS21x_readIdentifiers()
{
	//debug(__fu//debug(

	ENS21x_setLowPower(false);
	HAL_Delay(BOOTING);

	ENS21x_read(PART_ID, partId, 2);
	//debug("PART_ID: ", partId);

	ENS21x_read(DIE_REV, dieRev, 2);
	//debug("DIE_REV: ", dieRev);

	ENS21x_read(UID, uid, 8);
	//debug("UID:     ", uid);

	ENS21x_setLowPower(true);
}

// Compute the CRC-7 of 'val' (should only have 17 bits)
// https://en.wikipedia.org/wiki/Cyclic_redundancy_check#Computation
uint32_t ENS21x_crc7(uint32_t val)
{
	static const uint8_t CRC7WIDTH  = 7;                            // A 7 bits CRC has polynomial of 7th order, which has 8 terms
	static const uint8_t CRC7POLY   = 0x89;                         // The 8 coefficients of the polynomial
	static const uint8_t CRC7IVEC   = 0x7F;                         // Initial vector has all 7 bits high
	static const uint8_t DATA7WIDTH = 17;
	static const uint32_t DATA7MASK = ((1UL << DATA7WIDTH) - 1);    // 0b 0 1111 1111 1111 1111
	static const uint32_t DATA7MSB  = (1UL << (DATA7WIDTH - 1));    // 0b 1 0000 0000 0000 0000

	// Setup polynomial
	uint32_t pol = CRC7POLY;

	// Align polynomial with data
	pol = pol << (DATA7WIDTH - CRC7WIDTH - 1);

	// Loop variable (indicates which bit to test, start with highest)
	uint32_t bit = DATA7MSB;

	// Make room for CRC value
	val = val << CRC7WIDTH;
	bit = bit << CRC7WIDTH;
	pol = pol << CRC7WIDTH;

	// Insert initial vector
	val |= CRC7IVEC;

	// Apply division until all bits done
	while (bit & (DATA7MASK << CRC7WIDTH))
	{
		if (bit & val)
		{
			val ^= pol;
		}

		bit >>= 1;
		pol >>= 1;
	}
	return val;
}

uint32_t ENS21x_checkData(uint32_t data)
{
	enum Result result = STATUS_CRC_ERROR;

	data            &= 0xffffff;
	uint8_t valid    = (data>>16) & 0x01;
	uint32_t crc     = (data>>17) & 0x7f;
	uint32_t payload = data       & 0x1ffff;

	if (ENS21x_crc7(payload) == crc)
	{
		result = valid == 1 ? STATUS_OK : STATUS_INVALID;
	}

	//debug(__func__, result);
	if (result != STATUS_OK)
	{
		//debug("Data   : ", data);
		//debug("payload: ", payload);
		//debug("CRC    : ", crc);
		//debug("Valid  : ", valid);
	}

	return result;
}
