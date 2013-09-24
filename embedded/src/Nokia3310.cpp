/*
 * Nokia3310.cpp
 *
 *  Created on: 21 sept. 2013
 *      Author: mehdi
 */

#include "Nokia3310.h"

Nokia3310::Nokia3310() {
	// TODO Auto-generated constructor stub

}

void Nokia3310::sendSms(String _number, String _message)
{
	Serial.println("Sms " + _number + " => " + _message );
}

Nokia3310::~Nokia3310() {
	// TODO Auto-generated destructor stub
}

