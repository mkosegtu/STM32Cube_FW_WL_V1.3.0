/*
  ScioSense_ENS160.h - Library for the ENS160 sensor with I2C interface from ScioSense
  2023 Mar 23	v6	Christoph Friese	Bugfix measurement routine, prepare next release
  2021 July 29	v4	Christoph Friese	Changed nomenclature to ScioSense as product shifted from ams
  2020 Apr 06	v3	Christoph Friese	Changed nomenclature to ScioSense as product shifted from ams
  2020 Feb 15	v2	Giuseppe Pasetti	Corrected firmware flash option
  2019 May 05	v1	Christoph Friese	Created
  based on application note "ENS160 Software Integration.pdf" rev 0.01
*/

#ifndef __SCIOSENSE_ENS160_H_
#define __SCIOSENSE_ENS160_H_

#include "stdbool.h"
#include "stdint.h"

// Chip constants
#define ENS160_PARTID				0x0160
#define ENS161_PARTID				0x0161
#define ENS160_BOOTING				10

// 7-bit I2C slave address of the ENS160
#define ENS160_I2CADDR_0          	0x52		//ADDR low
#define ENS160_I2CADDR_1          	0x53		//ADDR high

// ENS160 registers for version V0
#define ENS160_REG_PART_ID          	0x00 		// 2 byte register
#define ENS160_REG_OPMODE		0x10
#define ENS160_REG_CONFIG		0x11
#define ENS160_REG_COMMAND		0x12
#define ENS160_REG_TEMP_IN		0x13
#define ENS160_REG_RH_IN		0x15
#define ENS160_REG_DATA_STATUS		0x20
#define ENS160_REG_DATA_AQI		0x21
#define ENS160_REG_DATA_TVOC		0x22
#define ENS160_REG_DATA_ECO2		0x24			
#define ENS160_REG_DATA_BL		0x28
#define ENS160_REG_DATA_T		0x30
#define ENS160_REG_DATA_RH		0x32
#define ENS160_REG_DATA_MISR		0x38
#define ENS160_REG_GPR_WRITE_0		0x40
#define ENS160_REG_GPR_WRITE_1		ENS160_REG_GPR_WRITE_0 + 1
#define ENS160_REG_GPR_WRITE_2		ENS160_REG_GPR_WRITE_0 + 2
#define ENS160_REG_GPR_WRITE_3		ENS160_REG_GPR_WRITE_0 + 3
#define ENS160_REG_GPR_WRITE_4		ENS160_REG_GPR_WRITE_0 + 4
#define ENS160_REG_GPR_WRITE_5		ENS160_REG_GPR_WRITE_0 + 5
#define ENS160_REG_GPR_WRITE_6		ENS160_REG_GPR_WRITE_0 + 6
#define ENS160_REG_GPR_WRITE_7		ENS160_REG_GPR_WRITE_0 + 7
#define ENS160_REG_GPR_READ_0		0x48
#define ENS160_REG_GPR_READ_4		ENS160_REG_GPR_READ_0 + 4
#define ENS160_REG_GPR_READ_6		ENS160_REG_GPR_READ_0 + 6
#define ENS160_REG_GPR_READ_7		ENS160_REG_GPR_READ_0 + 7

//ENS160 data register fields
#define ENS160_COMMAND_NOP		0x00
#define ENS160_COMMAND_CLRGPR		0xCC
#define ENS160_COMMAND_GET_APPVER	0x0E 
#define ENS160_COMMAND_SETTH		0x02
#define ENS160_COMMAND_SETSEQ		0xC2

#define ENS160_OPMODE_RESET		0xF0
#define ENS160_OPMODE_DEP_SLEEP		0x00
#define ENS160_OPMODE_IDLE		0x01
#define ENS160_OPMODE_STD		0x02
#define ENS160_OPMODE_LP		0x03	
#define ENS160_OPMODE_CUSTOM		0xC0

#define ENS160_BL_CMD_START		0x02
#define ENS160_BL_CMD_ERASE_APP		0x04
#define ENS160_BL_CMD_ERASE_BLINE	0x06
#define ENS160_BL_CMD_WRITE		0x08
#define ENS160_BL_CMD_VERIFY		0x0A
#define ENS160_BL_CMD_GET_BLVER		0x0C
#define ENS160_BL_CMD_GET_APPVER	0x0E
#define ENS160_BL_CMD_EXITBL		0x12

#define ENS160_SEQ_ACK_NOTCOMPLETE	0x80
#define ENS160_SEQ_ACK_COMPLETE		0xC0

#define IS_ENS160_SEQ_ACK_NOT_COMPLETE(x) 	(ENS160_SEQ_ACK_NOTCOMPLETE == (ENS160_SEQ_ACK_NOTCOMPLETE & (x)))
#define IS_ENS160_SEQ_ACK_COMPLETE(x) 		(ENS160_SEQ_ACK_COMPLETE == (ENS160_SEQ_ACK_COMPLETE & (x)))

#define ENS160_DATA_STATUS_NEWDAT	0x02
#define ENS160_DATA_STATUS_NEWGPR	0x01

#define IS_NEWDAT(x) 			(ENS160_DATA_STATUS_NEWDAT == (ENS160_DATA_STATUS_NEWDAT & (x)))
#define IS_NEWGPR(x) 			(ENS160_DATA_STATUS_NEWGPR == (ENS160_DATA_STATUS_NEWGPR & (x)))
#define IS_NEW_DATA_AVAILABLE(x) 	(0 != ((ENS160_DATA_STATUS_NEWDAT | ENS160_DATA_STATUS_NEWGPR ) & (x)))

#define CONVERT_RS_RAW2OHMS_I(x) 	(1 << ((x) >> 11))
#define CONVERT_RS_RAW2OHMS_F(x) 	(pow (2, (float)(x) / 2048))

struct ens160 {
	bool				_available;						// ENS160 available
	uint8_t				_revENS16x;							// ENS160 or ENS161 connected? (FW >7)

	uint8_t				_fw_ver_major;
	uint8_t 			_fw_ver_minor;
	uint8_t				_fw_ver_build;

	uint16_t			_stepCount;							// Counter for custom sequence

	uint8_t				_data_aqi;
	uint16_t			_data_tvoc;
	uint16_t			_data_eco2;
	uint16_t			_data_aqi500;
	uint32_t			_hp0_rs;
	uint32_t			_hp0_bl;
	uint32_t			_hp1_rs;
	uint32_t			_hp1_bl;
	uint32_t			_hp2_rs;
	uint32_t			_hp2_bl;
	uint32_t			_hp3_rs;
	uint32_t			_hp3_bl;
	uint16_t			_temp;
	int  				_slaveaddr;							// Slave address of the ENS160
	uint8_t				_misr;
};

struct ens160_data_str {
	uint8_t				_data_aqi;
	uint16_t			_data_tvoc;
	uint16_t			_data_eco2;
	uint16_t			_data_aqi500;
};

bool ens160_checkPartID(void);
int ens160_measure(struct ens160_data_str* ens160_data);
int ens160_init(void);

#endif
