/*
 * Led.h
 *
 *  Created on: 8 févr. 2013
 *      Author: mehdi
 */

#ifndef LED_H_
#define LED_H_

#if ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
 
 
class Led
 {
private:
  bool m_state;
  unsigned long m_lastBlink;
protected:
  int m_pinLed;
public:
  
  Led(int pin, bool state = false);
//  void blink(int time);
  void on();
  void off();
  void toggle();
  void setState(bool _state);
  void blink(int _time);
  bool getState();

  virtual ~Led();
};

#endif /* LED_H_ */
