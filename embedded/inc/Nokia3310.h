/*
 * Nokia3310.h
 *
 *  Created on: 21 sept. 2013
 *      Author: mehdi
 */

#ifndef NOKIA3310_H_
#define NOKIA3310_H_

#include "PowerFailureAlarm.h"

class Nokia3310 {
public:
	Nokia3310();
	void sendSms(String _number, String _message);
	virtual ~Nokia3310();
};

#endif /* NOKIA3310_H_ */
