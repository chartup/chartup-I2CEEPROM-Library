/****************************************************************************

I2CEEPROM_Example_Wire1.ino - This is a DEMO example showing the usage of the
library for I2C EEPROM (alternate Arduino DUE I2C interface SDA1/SCL1
-> https://www.arduino.cc/en/Reference/Wire).

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
# URL       : https://github.com/chartup/chartup-I2CEEPROM-Library/blob/master/examples/I2CEEPROM_Example_Wire1/I2CEEPROM_Example_Wire1.ino
# AUTHOR    : Anton G. Mueckl (amueckl@chartup.de)
#
# DATE      : 2015-08-24
# CHANGES   : None.
# PURPOSE   :
##
#############################################################################

****************************************************************************/

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Wire.h>

#include <chartup_I2CEEPROM.h>

const unsigned int bytes_to_write = 1000;

int thisByte = 33; // code of first visible ASCII character ('!')

void setup() {
	Serial.begin(19200);
	chartup_I2CEEPROM i2ceeprom(1, 7);
	delay(3000);			// wait for the interface to get ready
	Serial.print("Write ");
	Serial.print(bytes_to_write);
	Serial.println(" Byte to EEPROM ICs.");
	{
		thisByte = 33;
		for (unsigned long i=0; i < bytes_to_write; i++)
		{
			for (int chip=0; chip < 8; chip++)
			{
				i2ceeprom.write(chip, i, thisByte);
			}
			thisByte++;
			if (thisByte == '~')
				thisByte = 33;
		}
	}

	
	for (int chip=0; chip < 8; chip++)
	{
		Serial.print("Read ");
		Serial.print(bytes_to_write);
		Serial.print(" Byte from EEPROM");
		Serial.println(chip);

		thisByte = 33;
		for (unsigned long i=0; i < bytes_to_write; i++)
		{
			char c = i2ceeprom.read(chip, i);
			if (c!=thisByte)
			{
				Serial.println("Read error!");
				Serial.print(c, DEC);
				Serial.println();
				break;
			}
			thisByte++;
			if (thisByte == '~')
			{
				thisByte = 33;
			}
		}
	}
	Serial.println("DONE.");
}

void loop() {
}
