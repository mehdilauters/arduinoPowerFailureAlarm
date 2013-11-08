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

#include "PowerFailureAlarmComon.h"

#define LOG_LEVEL LOG_LEVEL_WARNING




//#define DEBUG_WAIT_FOR_LOGGER

#define CONTACT_OFFSET (sizeof(int)*2)
#define RESOURCES_OFFSET 0

#define PIN_NOKIA_CONTROLE 7
#define PIN_MAIN_POWER 11
#define PIN_POWER_LEVEL A11
#define PIN_LED 5

// resistor bridge to read power voltage
#define POWER_LEVEL_R1 320
#define POWER_LEVEL_R2 320
#define POWER_LEVEL_FACTOR ((float)1024 / ((float)POWER_LEVEL_R2/((float)POWER_LEVEL_R1 + (float)POWER_LEVEL_R2)))
#define REFERENCE_VOLT 5

#define SMS_SEND_NB_TRIES 3
#define GETSMSC_NB_TRIES 3
#define FBUS_NB_TRIES 3

#define LED_FAILURE_BLINK_TIME 250


#define FBUS_BAUDRATE 115200



#endif /* POWERFAILUREALARM_H_ */
