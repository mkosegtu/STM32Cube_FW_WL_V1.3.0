#ifndef SCIOSENSE_ENS21X_H
#define SCIOSENSE_ENS21X_H

#include <stdint.h>
#include "stdbool.h"

// System Timing Characteristics
enum SystemTiming
{
	BOOTING                 = 2,      // Booting time in ms (also after reset, or going to high power)
	CONVERSION_SINGLE_SHOT  = 130,    // Conversion time in ms for single shot T/H measurement
	CONVERSION_CONTINUOUS   = 238,    // Conversion time in ms for continuous T/H measurement
};

// System configuration (SYS_CTRL)
enum SystemControl
{
	DISABLE_LOW_POWER   = 0,
	ENABLE_LOW_POWER    = 1 << 0,
	SENSOR_RESET        = 1 << 7
};

// Addresses of the ENS21x registers
enum RegisterAddress
{
	PART_ID     = 0x00,     // Identifies the part as ENS21x
	DIE_REV     = 0x02,     // Identifies the die revision version
	UID         = 0x04,     // Unique identifier
	SYS_CTRL    = 0x10,     // System configuration
	SYS_STAT    = 0x11,     // System status
	SENS_RUN    = 0x21,     // The run mode (single shot or continuous)
	SENS_START  = 0x22,     // Start measurement
	SENS_STOP   = 0x23,     // Stop continuous measurement
	SENS_STAT   = 0x24,     // Sensor status (idle or measuring)
	T_VAL       = 0x30,     // Temperature readout
	H_VAL       = 0x33      // Relative humidity readout
};

enum Sensor
{
	TEMPERATURE                 = 1 << 0,
	HUMIDITY                    = 1 << 1,
	TEMPERATURE_AND_HUMIDITY    = TEMPERATURE | HUMIDITY
};

enum Result
{
	STATUS_I2C_ERROR = 4, // There was an I2C communication error, `read`ing the value.
	STATUS_CRC_ERROR = 3, // The value was read, but the CRC over the payload (valid and data) does not match.
	STATUS_INVALID   = 2, // The value was read, the CRC matches, but the data is invalid (e.g. the measurement was not yet finished).
	STATUS_OK        = 1  // The value was read, the CRC matches, and data is valid.
};


uint32_t ENS21x_update(uint64_t ms);
uint32_t ENS21x_singleShotMeasure        (enum Sensor sensor);     // invokes a single shot mode measuring for the given sensor
uint32_t ENS21x_startContinuousMeasure   (enum Sensor sensor);     // starts the continuous measure mode for the given sensor
uint32_t ENS21x_stopContinuousMeasure    (enum Sensor sensor);     // stops the continuous measure mode for the given sensor
uint32_t ENS21x_setLowPower(bool enable);                                                        // en-/disables the low power mode
uint32_t ENS21x_reset();                                                                         // reset the device

uint16_t ENS21x_getPartId();   // returns the PART_ID or zero if invalid
uint16_t ENS21x_getDieRev();   // returns the DIE_REV or zero if invalid
uint64_t ENS21x_getUid();      // returns the UID or zero if invalid

void ENS21x_setSolderCorrection(int16_t correction);  // Sets the solder correction (default is 50mK) - used to calculate temperature and humidity values
float ENS21x_getTempKelvin();                                          // Converts and returns measurement data in Kelvin
float ENS21x_getTempCelsius();                                         // Converts and returns measurement data in Celsius
float ENS21x_getTempFahrenheit();                                      // Converts and returns measurement data in Fahrenheit
float ENS21x_getHumidityPercent();                                     // Converts and returns measurement data in %RH
float ENS21x_getAbsoluteHumidityPercent();                             // Converts and returns measurement data in %aH

uint16_t ENS21x_getDataT(); // returns raw temperature data; e.g. to be used as temperature compensation for ENS160 (TEMP_IN)
uint16_t ENS21x_getDataH(); // returns raw humidity data; e.g. to be used as humidity compensation for ENS160 (RH_IN)
uint32_t ENS21x_getStatusT(); // returns the read status of the last read temperature data
uint32_t ENS21x_getStatusH(); // returns the read status of the last read humidity data

uint32_t ENS21x_read(enum RegisterAddress address, uint8_t* data, uint8_t num);
uint32_t ENS21x_write(enum RegisterAddress address, uint8_t* data, uint8_t num);

void readIdentifiers();

uint32_t ENS21x_crc7(uint32_t val);
uint32_t ENS21x_checkData(uint32_t data);


#endif //SCIOSENSE_ENS21X_H
