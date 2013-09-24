/*
 * PowerFailureAlarm.h
 *
 *  Created on: 17 sept. 2013
 *      Author: mehdi
 */

#ifndef POWERFAILUREALARM_H_
#define POWERFAILUREALARM_H_

#if ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

#define CONTACT_MAX_COUNT 3
#define CONTACT_OFFSET (sizeof(int)*2)
#define RESOURCES_OFFSET 0

#define PIN_POWER 2
#define PIN_POWER_FAILURE_LED 13

#endif /* POWERFAILUREALARM_H_ */
