/*
  ScioSense_ENS160.h - Library for the ENS160 sensor with I2C interface from ScioSense
  2023 Mar 23	v6	Christoph Friese	Bugfix measurement routine, prepare next release
  2021 Nov 25   v5	Martin Herold		Custom mode timing fixed
  2021 Feb 04	v4	Giuseppe de Pinto	Custom mode fixed
  2020 Apr 06	v3	Christoph Friese	Changed nomenclature to ScioSense as product shifted from ams
  2020 Feb 15	v2	Giuseppe Pasetti	Corrected firmware flash option
  2019 May 05	v1	Christoph Friese	Created
  based on application note "ENS160 Software Integration.pdf" rev 0.01
*/

#include "ens160.h"
#include "math.h"
#include "i2c.h"

uint8_t slaveaddr = ENS160_I2CADDR_0;

struct ens160 ens160_dev;

/**************************************************************************/

uint8_t ens160_read(uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t num) {
	uint8_t pos = 0;
	uint8_t result = 0;

//	if (debugENS160) {
//		Serial.print("I2C read address: 0x");
//		Serial.print(addr, HEX);
//		Serial.print(" - register: 0x");
//		Serial.println(reg, HEX);
//	}
	result = HAL_I2C_Mem_Read(&hi2c2, addr, (uint16_t)reg,
	      		I2C_MEMADD_SIZE_8BIT, buf, (uint16_t)num, 10000);

	return result;
}

uint8_t ens160_read8(uint8_t addr, uint8_t reg) {
	uint8_t ret = 0;
	ens160_read(addr, reg, &ret, 1);
	return ret;
}

/**************************************************************************/

uint8_t ens160_write(uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t num) {
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

	uint8_t result = HAL_I2C_Mem_Write(&hi2c2, addr, (uint16_t)reg,
	      		I2C_MEMADD_SIZE_8BIT, buf, (uint16_t)num, 10000);
	return result;
}

/**************************************************************************/

uint8_t ens160_write8(uint8_t addr, uint8_t reg, uint8_t value) {
	uint8_t result = ens160_write(addr, reg, &value, 1);
	return result;
}

// Sends a reset to the ENS160. Returns false on I2C problems.
bool ens160_reset(void)
{
	uint8_t result = ens160_write8(slaveaddr, ENS160_REG_OPMODE, ENS160_OPMODE_RESET);

//	if (debugENS160) {
//		Serial.print("reset() result: ");
//		Serial.println(result == 0 ? "ok" : "nok");
//	}
	TIMER_IF_DelayMs(ENS160_BOOTING);                   // Wait to boot after reset

	return result == 0;
}

// Reads the part ID and confirms valid sensor
bool ens160_checkPartID(void) {
	uint8_t i2cbuf[2];
	uint16_t part_id;
	bool result = false;
	
	ens160_read(slaveaddr, ENS160_REG_PART_ID, i2cbuf, 2);
	part_id = i2cbuf[0] | ((uint16_t)i2cbuf[1] << 8);
	
//	if (debugENS160) {
//		Serial.print("checkPartID() result: ");
//		if (part_id == ENS160_PARTID) Serial.println("ENS160 ok");
//		else if (part_id == ENS161_PARTID) Serial.println("ENS161 ok");
//		else Serial.println("nok");
//	}
	TIMER_IF_DelayMs(ENS160_BOOTING);                   // Wait to boot after reset

	if (part_id == ENS160_PARTID) { ens160_dev._revENS16x = 0; result = true; }
	else if (part_id == ENS161_PARTID) { ens160_dev._revENS16x = 1; result = true; }
	
	return result;
}

// Initialize idle mode and confirms 
bool ens160_clearCommand(void) {
	uint8_t status;
	uint8_t result;
	
	result = ens160_write8(slaveaddr, ENS160_REG_COMMAND, ENS160_COMMAND_NOP);
	result = ens160_write8(slaveaddr, ENS160_REG_COMMAND, ENS160_COMMAND_CLRGPR);
//	if (debugENS160) {
//		Serial.print("clearCommand() result: ");
//		Serial.println(result == 0 ? "ok" : "nok");
//	}
	TIMER_IF_DelayMs(ENS160_BOOTING);                   // Wait to boot after reset
	
	status = ens160_read8(slaveaddr, ENS160_REG_DATA_STATUS);
// 	if (debugENS160) {
//		Serial.print("clearCommand() status: 0x");
//		Serial.println(status, HEX);
//	}
	TIMER_IF_DelayMs(ENS160_BOOTING);                   // Wait to boot after reset
		
	return result == 0;
}

// Read firmware revisions
bool ens160_getFirmware() {
	uint8_t i2cbuf[3];
	uint8_t result;
	
	ens160_clearCommand();
	
	TIMER_IF_DelayMs(ENS160_BOOTING);                   // Wait to boot after reset
	
	result = ens160_write8(slaveaddr, ENS160_REG_COMMAND, ENS160_COMMAND_GET_APPVER);
	result = ens160_read(slaveaddr, ENS160_REG_GPR_READ_4, i2cbuf, 3);

	ens160_dev._fw_ver_major = i2cbuf[0];
	ens160_dev._fw_ver_minor = i2cbuf[1];
	ens160_dev._fw_ver_build = i2cbuf[2];
	
	if (ens160_dev._fw_ver_major > 6) ens160_dev._revENS16x = 1;
	else ens160_dev._revENS16x = 0;

//	if (debugENS160) {
//		Serial.println(ens160_dev._fw_ver_major);
//		Serial.println(ens160_dev._fw_ver_minor);
//		Serial.println(ens160_dev._fw_ver_build);
//		Serial.print("getFirmware() result: ");
//		Serial.println(result == 0 ? "ok" : "nok");
//	}
	TIMER_IF_DelayMs(ENS160_BOOTING);                   // Wait to boot after reset
	
	return result == 0;
}

// Set operation mode of sensor
bool ens160_setMode(uint8_t mode) {
	uint8_t result;
	
	//LP only valid for rev>0
	if ((mode == ENS160_OPMODE_LP) && (ens160_dev._revENS16x == 0)) result = 1;
	else result = ens160_write8(slaveaddr, ENS160_REG_OPMODE, mode);

//	if (debugENS160) {
//		Serial.print("setMode() activate result: ");
//		Serial.println(result == 0 ? "ok" : "nok");
//	}

	TIMER_IF_DelayMs(ENS160_BOOTING);                   // Wait to boot after reset
	
	return result == 0;
}

// Initialize definition of custom mode with <n> steps
bool ens160_initCustomMode(uint16_t stepNum) {
	uint8_t result;
	
	if (stepNum > 0) {
		ens160_dev._stepCount = stepNum;
		
		result = ens160_setMode(ENS160_OPMODE_IDLE);
		result = ens160_clearCommand();

		result = ens160_write8(slaveaddr, ENS160_REG_COMMAND, ENS160_COMMAND_SETSEQ);
	} else {
		result = 1;
	}
	TIMER_IF_DelayMs(ENS160_BOOTING);                   // Wait to boot after reset
	
	return result == 0;
}

// Add a step to custom measurement profile with definition of duration, enabled data acquisition and temperature for each hotplate
bool ens160_addCustomStep(uint16_t time, bool measureHP0, bool measureHP1, bool measureHP2, bool measureHP3, uint16_t tempHP0, uint16_t tempHP1, uint16_t tempHP2, uint16_t tempHP3) {
	uint8_t seq_ack;
	uint8_t temp;

//	if (debugENS160) {
//		Serial.print("setCustomMode() write step ");
//		Serial.println(ens160_dev._stepCount);
//	}
	TIMER_IF_DelayMs(ENS160_BOOTING);                   // Wait to boot after reset

	temp = (uint8_t)(((time / 24)-1) << 6); 
	if (measureHP0) temp = temp | 0x20;
	if (measureHP1) temp = temp | 0x10;
	if (measureHP2) temp = temp | 0x8;
	if (measureHP3) temp = temp | 0x4;
	ens160_write8(slaveaddr, ENS160_REG_GPR_WRITE_0, temp);

	temp = (uint8_t)(((time / 24)-1) >> 2); 
	ens160_write8(slaveaddr, ENS160_REG_GPR_WRITE_1, temp);

	ens160_write8(slaveaddr, ENS160_REG_GPR_WRITE_2, (uint8_t)(tempHP0/2));
	ens160_write8(slaveaddr, ENS160_REG_GPR_WRITE_3, (uint8_t)(tempHP1/2));
	ens160_write8(slaveaddr, ENS160_REG_GPR_WRITE_4, (uint8_t)(tempHP2/2));
	ens160_write8(slaveaddr, ENS160_REG_GPR_WRITE_5, (uint8_t)(tempHP3/2));

	ens160_write8(slaveaddr, ENS160_REG_GPR_WRITE_6, (uint8_t)(ens160_dev._stepCount - 1));

    if (ens160_dev._stepCount == 1) {
        ens160_write8(slaveaddr, ENS160_REG_GPR_WRITE_7, 128);
    } else {
        ens160_write8(slaveaddr, ENS160_REG_GPR_WRITE_7, 0);
    }
    TIMER_IF_DelayMs(ENS160_BOOTING);

	seq_ack = ens160_read8(slaveaddr, ENS160_REG_GPR_READ_7);
	TIMER_IF_DelayMs(ENS160_BOOTING);                   // Wait to boot after reset
	
	if ((ENS160_SEQ_ACK_COMPLETE | ens160_dev._stepCount) != seq_ack) {
		ens160_dev._stepCount = ens160_dev._stepCount - 1;
		return 0;
	} else {
		return 1;
	}
	
}

// Perform prediction measurement and stores result in internal variables
bool ens160_measure(bool waitForNew) {
	uint8_t i2cbuf[8];
	uint8_t status;
	bool newData = false;

	// Set default status for early bail out
//	if (debugENS160) Serial.println("Start measurement");
	
	if (waitForNew) {
		do {
			TIMER_IF_DelayMs(1);
			status = ens160_read8(slaveaddr, ENS160_REG_DATA_STATUS);
			
//			if (debugENS160) {
//				Serial.print("Status: ");
//				Serial.println(status);
//			}
			
		} while (!IS_NEWDAT(status));
	} else {
		status = ens160_read8(slaveaddr, ENS160_REG_DATA_STATUS);
	}
	
	// Read predictions
	if (IS_NEWDAT(status)) {
		newData = true;
		ens160_read(slaveaddr, ENS160_REG_DATA_AQI, i2cbuf, 7);
		ens160_dev._data_aqi = i2cbuf[0];
		ens160_dev._data_tvoc = i2cbuf[1] | ((uint16_t)i2cbuf[2] << 8);
		ens160_dev._data_eco2 = i2cbuf[3] | ((uint16_t)i2cbuf[4] << 8);
		if (ens160_dev._revENS16x > 0) ens160_dev._data_aqi500 = ((uint16_t)i2cbuf[5]) | ((uint16_t)i2cbuf[6] << 8);
		else ens160_dev._data_aqi500 = 0;
	}
	
	return newData;
}

// Perfrom raw measurement and stores result in internal variables
bool ens160_measureRaw(bool waitForNew) {
	uint8_t i2cbuf[8];
	uint8_t status;
	bool newData = false;

	// Set default status for early bail out
//	if (debugENS160) Serial.println("Start measurement");
	
	if (waitForNew) {
		do {
			TIMER_IF_DelayMs(1);
			status = ens160_read8(slaveaddr, ENS160_REG_DATA_STATUS);
			
//			if (debugENS160) {
//				Serial.print("Status: ");
//				Serial.println(status);
//			}
		} while (!IS_NEWGPR(status));
	} else {
		status = ens160_read8(slaveaddr, ENS160_REG_DATA_STATUS);
	}
	
	if (IS_NEWGPR(status)) {
		newData = true;		
		
		// Read raw resistance values
		ens160_read(slaveaddr, ENS160_REG_GPR_READ_0, i2cbuf, 8);
		ens160_dev._hp0_rs = CONVERT_RS_RAW2OHMS_F((uint32_t)(i2cbuf[0] | ((uint16_t)i2cbuf[1] << 8)));
		ens160_dev._hp1_rs = CONVERT_RS_RAW2OHMS_F((uint32_t)(i2cbuf[2] | ((uint16_t)i2cbuf[3] << 8)));
		ens160_dev._hp2_rs = CONVERT_RS_RAW2OHMS_F((uint32_t)(i2cbuf[4] | ((uint16_t)i2cbuf[5] << 8)));
		ens160_dev._hp3_rs = CONVERT_RS_RAW2OHMS_F((uint32_t)(i2cbuf[6] | ((uint16_t)i2cbuf[7] << 8)));
	
		// Read baselines
		ens160_read(slaveaddr, ENS160_REG_DATA_BL, i2cbuf, 8);
		ens160_dev._hp0_bl = CONVERT_RS_RAW2OHMS_F((uint32_t)(i2cbuf[0] | ((uint16_t)i2cbuf[1] << 8)));
		ens160_dev._hp1_bl = CONVERT_RS_RAW2OHMS_F((uint32_t)(i2cbuf[2] | ((uint16_t)i2cbuf[3] << 8)));
		ens160_dev._hp2_bl = CONVERT_RS_RAW2OHMS_F((uint32_t)(i2cbuf[4] | ((uint16_t)i2cbuf[5] << 8)));
		ens160_dev._hp3_bl = CONVERT_RS_RAW2OHMS_F((uint32_t)(i2cbuf[6] | ((uint16_t)i2cbuf[7] << 8)));

		ens160_read(slaveaddr, ENS160_REG_DATA_MISR, i2cbuf, 1);
		ens160_dev._misr = i2cbuf[0];
	}
	
	return newData;
}

// Writes t and h (in ENS210 format) to ENV_DATA. Returns false on I2C problems.
bool ens160_set_envdata210(uint16_t t, uint16_t h) {
	//uint16_t temp;
	uint8_t trh_in[4];
	
	//temp = (uint16_t)((t + 273.15f) * 64.0f);
	trh_in[0] = t & 0xff;
	trh_in[1] = (t >> 8) & 0xff;
	
	//temp = (uint16_t)(h * 512.0f);
	trh_in[2] = h & 0xff;
	trh_in[3] = (h >> 8) & 0xff;
	
	uint8_t result = ens160_write(slaveaddr, ENS160_REG_TEMP_IN, trh_in, 4);
	
	return result;
}


// Writes t (degC) and h (%rh) to ENV_DATA. Returns false on I2C problems.
bool ens160_set_envdata(float t, float h) {

	uint16_t t_data = (uint16_t)((t + 273.15f) * 64.0f);

	uint16_t rh_data = (uint16_t)(h * 512.0f);

	return ens160_set_envdata210(t_data, rh_data);
}

/**************************************************************************/
