/****************************************************************************

chartup_I2CEEPROM.h - This is the header file of the library for I2C EEPROM.

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
# URL       : https://github.com/chartup/chartup-I2CEEPROM-Library/blob/master/chartup_I2CEEPROM.h
# AUTHOR    : Anton G. Mueckl (amueckl@chartup.de)
#
# DATE      : 2015-08-24
# CHANGES   : None.
# PURPOSE   :
##
#############################################################################

****************************************************************************/


#ifndef chartup_I2CEEPROM_h
#define chartup_I2CEEPROM_h

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Wire.h>

#define chartup_I2CEEPROM_ON_I2C_0							0
#define chartup_I2CEEPROM_ON_I2C_1							1

#define chartup_I2CEEPROM_NO_ERROR							0
#define chartup_I2CEEPROM_BANK_INDEX_OUT_OF_RANGE_ERROR		1
#define chartup_I2CEEPROM_MAX_BANK_INDEX_OUT_OF_RANGE_ERROR	2
#define chartup_I2CEEPROM_I2C_INDEX_OUT_OF_RANGE_ERROR		3
#define chartup_I2CEEPROM_ADRESS_OUT_OF_RANGE_ERROR			4

#define chartup_I2CEEPROM_ID (0x50 + 0)		/* I2C address for 24LC128-EEPROM */

#define chartup_I2CEEPROM_ABSOLUTE_MAX_I2C_INDEX			1
#define chartup_I2CEEPROM_ABSOLUTE_MAX_BANK_INDEX			7
#define chartup_I2CEEPROM_ABSOLUTE_MAX_ADDRESS_INDEX		65535

class chartup_I2CEEPROM{
public:
	chartup_I2CEEPROM(unsigned char interface, unsigned char max_bank_index);
	void write(unsigned char chip, unsigned int address, char data);
	void write(unsigned int address, char data);
	char read(unsigned char chip, unsigned int address);
	char read(unsigned int address);
	unsigned char set_bank(unsigned char chip);
	unsigned char get_last_bank();
	unsigned char get_last_error();
private:
	void _write0(unsigned char chip, unsigned int address, char data);
	char _read0(unsigned char chip, unsigned int address);
#if WIRE_INTERFACES_COUNT > 1
	void _write1(unsigned char chip, unsigned int address, char data);
	char _read1(unsigned char chip, unsigned int address);
#endif
	unsigned char _interface;
	unsigned char _max_bank_index;
	unsigned char _last_error_code;
	unsigned char _last_bank_id;
};

#endif

