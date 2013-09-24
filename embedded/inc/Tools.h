/*
 * Tools.h
 *
 *  Created on: 21 sept. 2013
 *      Author: mehdi
 */

#ifndef TOOLS_H_
#define TOOLS_H_

#include "PowerFailureAlarm.h"

class Tools {
public:
	Tools();
	static void EEPROMsaveString(int _addr, String _string, int _maxSize);
	static String EEPROMreadString(int _addr, int _maxSize);
	static unsigned int EEPROMreadInt(int p_address);
	static void EEPROMwriteInt(int p_address, int p_value);
	virtual ~Tools();
};

#endif /* TOOLS_H_ */
