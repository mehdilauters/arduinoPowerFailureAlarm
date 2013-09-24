/*
 * Tools.cpp
 *
 *  Created on: 21 sept. 2013
 *      Author: mehdi
 */

#include "Tools.h"

#include <EEPROM/EEPROM.h>

Tools::Tools() {
	// TODO Auto-generated constructor stub

}


void Tools::EEPROMsaveString(int _addr, String _string, int _maxSize)
{
	int strSize = _string.length();
	int i;
	for(i=0; i < strSize; i++) // without '\0'
	{
		EEPROM.write(_addr + i, _string.charAt( i ));
//		Serial.println("w>>"+String(_addr + i)+"===>"+_string.charAt( i )+"<<");
		if( i == _maxSize -1 )
		{
			break;
		}
	}
	EEPROM.write(_addr + i, '\0');
}


String Tools::EEPROMreadString(int _addr, int _maxSize)
{
	int i;
	String res = "";
	for(i=0; i < _maxSize -1; i++) // without '\0'
	{
		char c;
		c = EEPROM.read(_addr + i);
//		Serial.println("r>>"+String(_addr+i)+"=>"+c+"<<");
		if( c == '\0' )
		{
			return res;
		}
		res = res + c;

	}
	return "";
}

void Tools::EEPROMwriteInt(int p_address, int p_value)
      {
      byte lowByte = ((p_value >> 0) & 0xFF);
      byte highByte = ((p_value >> 8) & 0xFF);

      EEPROM.write(p_address, lowByte);
      EEPROM.write(p_address + 1, highByte);
      }

//This function will read a 2 byte integer from the eeprom at the specified address and address + 1
unsigned int Tools::EEPROMreadInt(int p_address)
      {
      byte lowByte = EEPROM.read(p_address);
      byte highByte = EEPROM.read(p_address + 1);

      return ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);
      }


Tools::~Tools() {
	// TODO Auto-generated destructor stub
}

