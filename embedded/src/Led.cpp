/*
 * Led.cpp
 *
 *  Created on: 8 févr. 2013
 *      Author: mehdi
 */

#include "Led.h"
#include "Logger.h"
#define MODULE_ID 40

Led::Led(int _pin, bool state)
 {
 
 this->m_pinLed = _pin;
 this->m_lastBlink = 0;
 
  this->m_state = state;
  pinMode(this->m_pinLed, OUTPUT);
  if(this->m_state)
  {
    this->on();
  }
  else
  {
    this->off();
  }

}

void Led::blink(int _time)
{
  if((long)(millis() - (this->m_lastBlink + _time)) >= 0)
  {
    VERBOSE("Blink")
    this->toggle();
    this->m_lastBlink = millis();
  }
}

bool Led::getState()
{
  return this->m_state;
}

void Led::setState(bool _state)
{
  if(_state)
  {
    this->on();
  }
  else
  {
    this->off();
  }
}

void Led::on()
{
  if(!this->m_state)
  {
    digitalWrite(this->m_pinLed, HIGH);
    this->m_state = true;
  }
}

void Led::off()
{
  if(this->m_state)
  {
    this->m_state = false;
    digitalWrite(this->m_pinLed, LOW);
  }
}

void Led::toggle()
{
  if(this->m_state)
  {
    this->off();
  }
  else
  {
    this->on();
  }
}

//void Led::blink(int time)
//{
//
//}


Led::~Led() {

}

