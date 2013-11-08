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


// argument is time in milliseconds
String Tools::print_time(unsigned long t_milli)
{
    int days, hours, mins, secs;
    int fractime;
    unsigned long inttime;

    inttime  = t_milli / 1000;
    fractime = t_milli % 1000;
    // inttime is the total number of number of seconds
    // fractimeis the number of thousandths of a second

    // number of days is total number of seconds divided by 24 divided by 3600
    days     = inttime / (24*3600);
    inttime  = inttime % (24*3600);

    // Now, inttime is the remainder after subtracting the number of seconds
    // in the number of days
    hours    = inttime / 3600;
    inttime  = inttime % 3600;

    // Now, inttime is the remainder after subtracting the number of seconds
    // in the number of days and hours
    mins     = inttime / 60;
    inttime  = inttime % 60;

    // Now inttime is the number of seconds left after subtracting the number
    // in the number of days, hours and minutes. In other words, it is the
    // number of seconds.
    secs = inttime;

    // Don't bother to print days
    return String(days)+"d"+String(hours) + "h"+String(mins)+"m" +String(secs)+"s";
}


void Tools::EEPROMsaveString(int _addr, String _string, int _maxSize)
{
	int strSize = _string.length();
	if(strSize+1 >= _maxSize)
	{
		strSize = _maxSize -1;
	}
	int i;
	for(i=0; i < _maxSize; i++)
	{
		if(i < strSize)
		{
			EEPROM.write(_addr + i, _string.charAt( i ));
		}
		else
			if(i == strSize)
			{
				EEPROM.write(_addr + i, '\0');
			}
			else
			{
				EEPROM.write(_addr + i, 0xFF);
			}
	}
}


String Tools::EEPROMreadString(int _addr, int _maxSize)
{
	int i;
	String res = "";
	for(i=0; i < _maxSize; i++) // without '\0'
	{
		char c;
		c = EEPROM.read(_addr + i);
//		Serial.print(c);
//		Serial.println("r>>"+String(_addr+i)+"=>"+c+"<<");
		if( c == '\0' )
		{
			return res;
		}
		res = res + c;

	}
	return "";
}

void Tools::EEPROMDump(Stream *_stream, int p_start, int p_end)
{
//	for(int i=p_start;i<p_end;i++)
//	{
//		_stream->print(i);
//	}
//	_stream->print("\n");
	for(int i=p_start;i<p_end;i++)
	{
		_stream->write(EEPROM.read(i));
	}
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


void Tools::printDouble(Stream *_stream, double val, unsigned int precision){
// prints val with number of decimal places determine by precision
// NOTE: precision is 1 followed by the number of zeros for the desired number of decimial places
// example: printDouble( 3.1415, 100); // prints 3.14 (two decimal places)

    _stream->print (int(val));  //prints the int part
    _stream->print("."); // print the decimal point
    unsigned int frac;
    if(val >= 0)
        frac = (val - int(val)) * precision;
    else
        frac = (int(val)- val ) * precision;
    _stream->print(frac,DEC) ;
}

Tools::~Tools() {
	// TODO Auto-generated destructor stub
}

