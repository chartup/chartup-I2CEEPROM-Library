/****************************************************************************

chartup_I2CEEPROM.cpp - This is the code file of the library for I2C EEPROM.

Designed specifically to work with the chartup.com Arduino I2C EEPROM Shield 
----> http://www.chartup.com/product_info.php?products_id=266

These EEPROM use I2C to communicate, 2 pins are required to interface.

chartup.com invests time and resources providing this open source code.
Please support our efforts and open-source hardware by purchasing the 
corresponding hardware from chartup.com System Engineering GmbH!

Distributed under the MIT license:
https://github.com/chartup/chartup-I2CEEPROM-Library/blob/master/LICENSE

#############################################################################
#
# PROJECT   : SE009_Arduino_EEPROM_Shield
# URL       : https://github.com/chartup/chartup-I2CEEPROM-Library/blob/master/chartup_I2CEEPROM.cpp
# AUTHOR    : Anton G. Mueckl (amueckl@chartup.de)
#
# DATE      : 2015-08-24
# CHANGES   : None.
# PURPOSE   :
##
#############################################################################

****************************************************************************/


#include "chartup_I2CEEPROM.h"

chartup_I2CEEPROM::chartup_I2CEEPROM(unsigned char interface, unsigned char max_bank_index)
{
	if (interface > chartup_I2CEEPROM_ABSOLUTE_MAX_I2C_INDEX){
		_last_error_code = chartup_I2CEEPROM_I2C_INDEX_OUT_OF_RANGE_ERROR;
		_interface = 255;
		_max_bank_index = 255;
		return;
	}
	if (max_bank_index > chartup_I2CEEPROM_ABSOLUTE_MAX_BANK_INDEX){
		_last_error_code = chartup_I2CEEPROM_MAX_BANK_INDEX_OUT_OF_RANGE_ERROR;
		_interface = 255;
		_max_bank_index = 255;
		return;
	}
    _interface = interface;
	_max_bank_index = max_bank_index;
	_last_bank_id = 0;
	if (_interface==0){
		Wire.begin();
	}
#if WIRE_INTERFACES_COUNT > 1
	else{
		Wire1.begin();
	}
#endif
}

void chartup_I2CEEPROM::write(unsigned char chip, unsigned int address, char data)
{
	if (chip > _max_bank_index){
		_last_error_code = chartup_I2CEEPROM_BANK_INDEX_OUT_OF_RANGE_ERROR;
		return;
		
	}
	if (address > chartup_I2CEEPROM_ABSOLUTE_MAX_ADDRESS_INDEX){
		_last_error_code = chartup_I2CEEPROM_ADRESS_OUT_OF_RANGE_ERROR;
		return;
	}
	if (_interface == 0){
		_write0(chip, address, data);
	}
#if WIRE_INTERFACES_COUNT > 1
	else {
		_write1(chip, address, data);
	}
#endif
	_last_bank_id = chip;
}

// This functions is similar to EEPROM.write()
void chartup_I2CEEPROM::write(unsigned int address, char data)
{
	write(_last_bank_id, address, data);
}


char chartup_I2CEEPROM::read(unsigned char chip, unsigned int address)
{
	if (chip > _max_bank_index){
		_last_error_code = chartup_I2CEEPROM_BANK_INDEX_OUT_OF_RANGE_ERROR;
		return 0;
	}
	if (address > chartup_I2CEEPROM_ABSOLUTE_MAX_ADDRESS_INDEX){
		_last_error_code = chartup_I2CEEPROM_ADRESS_OUT_OF_RANGE_ERROR;
		return 0;
	}
	_last_bank_id = chip;
	if (_interface == 0){
		return _read0(chip, address);
	}
	else {
#if WIRE_INTERFACES_COUNT > 1
		return _read1(chip, address);
#else
		return 0;
#endif
	}
}


// This functions is similar to EEPROM.read()
char chartup_I2CEEPROM::read(unsigned int address)
{
	return read(_last_bank_id, address);
}

unsigned char chartup_I2CEEPROM::set_bank(unsigned char chip)
{
	if (chip > _max_bank_index){
		_last_error_code = chartup_I2CEEPROM_BANK_INDEX_OUT_OF_RANGE_ERROR;
		return _last_error_code;
	}
	_last_bank_id = chip;
	return 0;
}

unsigned char chartup_I2CEEPROM::get_last_bank()
{
	return _last_bank_id;
}

unsigned char chartup_I2CEEPROM::get_last_error()
{
	return _last_error_code;
}

/******************************************************************/
/* PRIVATE FUNCTIONS                                              */
/******************************************************************/

void chartup_I2CEEPROM::_write0(unsigned char chip, unsigned int address, char data)
{
	char result;
	// wait for I2C-EEPROM to be ready
	do
	{
		Wire.beginTransmission((char)(chartup_I2CEEPROM_ID + chip));
		result = Wire.endTransmission();
	} while (result > 0);
	Wire.beginTransmission((char)(chartup_I2CEEPROM_ID + chip));
	Wire.write((int) highByte(address));
	Wire.write((int) lowByte(address));
	Wire.write(data);
	Wire.endTransmission();	
}


// This functions is similar to EEPROM.read()
char chartup_I2CEEPROM::_read0(unsigned char chip, unsigned int address)
{
	char data;
	char result;
	// wait for I2C-EEPROM to be ready
	do
	{
		Wire.beginTransmission((char)(chartup_I2CEEPROM_ID + chip));
		result = Wire.endTransmission();
	} while (result > 0);
	Wire.beginTransmission((char)(chartup_I2CEEPROM_ID + chip));
	Wire.write((int) highByte(address));
	Wire.write((int) lowByte(address));
	Wire.endTransmission();
	Wire.requestFrom((char)(chartup_I2CEEPROM_ID + chip), (char) 1);
	while(Wire.available()==0) // wait for data
		;
	data = Wire.read();
	return data;
}


#if WIRE_INTERFACES_COUNT > 1
void chartup_I2CEEPROM::_write1(unsigned char chip, unsigned int address, char data)
{
	char result;
	// wait for I2C-EEPROM to be ready
	do
	{
		Wire1.beginTransmission((char)(chartup_I2CEEPROM_ID + chip));
		result = Wire1.endTransmission();
	} while (result > 0);
	Wire1.beginTransmission((char)(chartup_I2CEEPROM_ID + chip));
	Wire1.write((int) highByte(address));
	Wire1.write((int) lowByte(address));
	Wire1.write(data);
	Wire1.endTransmission();
}


// This functions is similar to EEPROM.read()
char chartup_I2CEEPROM::_read1(unsigned char chip, unsigned int address)
{
	char data;
	char result;
	// wait for I2C-EEPROM to be ready
	do
	{
		Wire1.beginTransmission((char)(chartup_I2CEEPROM_ID + chip));
		result = Wire1.endTransmission();
	} while (result > 0);
	Wire1.beginTransmission((char)(chartup_I2CEEPROM_ID + chip));
	Wire1.write((int) highByte(address));
	Wire1.write((int) lowByte(address));
	Wire1.endTransmission();
	Wire1.requestFrom((char)(chartup_I2CEEPROM_ID + chip), (char) 1);
	while(Wire1.available()==0) // wait for data
		;
	data = Wire1.read();
	return data;
}

#endif