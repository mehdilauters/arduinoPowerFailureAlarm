/*
 * ResourcesManager.cpp
 *
 *  Created on: 21 sept. 2013
 *      Author: mehdi
 */

#include "ResourcesManager.h"
#include "EmbeddedSettings.h"
#include "Contact.h"
#include "Nokia3310.h"
#include "Led.h"
#include "Logger.h"

#define MODULE_ID 10
#define SIGNAL_ID_VOLTAGE 0


ResourcesManager::ResourcesManager() {

  this->m_logIo = &Serial;
  #ifdef board_micro
    ((Serial_*)this->m_logIo)->begin(FBUS_BAUDRATE);
    this->m_fbusIo = &Serial1;
    ((HardwareSerial*)this->m_fbusIo)->begin(FBUS_BAUDRATE);
  #else
    #ifdef LOGGER_DEBUG
      ((HardwareSerial*)this->m_logIo)->begin(FBUS_BAUDRATE);
      this->m_fbusIo = this->m_logIo;
    #endif
  #endif

  this->m_statusLed = new Led(PIN_LED, false);


  this->m_logger = new Logger(this->m_logIo, LOG_LEVEL_VERBOSE);

  while(! this->m_logger->ready() )
  {
    this->getStatusLed()->on();
    delay(500);
    this->getStatusLed()->off();
    delay(100);
  }
  INFO("Starting logging");

  pinMode(PIN_MAIN_POWER, INPUT);
  pinMode(PIN_POWER_LEVEL, INPUT);


  this->m_nokia = new Nokia3310(this);

  this->m_lastPowerTime = 0;
  this->m_raised = false;
  this->m_settings = new EmbeddedSettings(this);
  this->getSettings()->load();
}

Stream * ResourcesManager::getLogIo()
{
  return this->m_logIo;
}

Nokia3310 * ResourcesManager::getNokia()
{
  return this->m_nokia;
}

Led * ResourcesManager::getStatusLed()
{
  return this->m_statusLed;
}

float ResourcesManager::getPowerVoltage()
{
  int  powerLevel = analogRead(PIN_POWER_LEVEL);
  float volts = (powerLevel / POWER_LEVEL_FACTOR) * REFERENCE_VOLT *2 *2 ;
//  this->getLogger()->debug("power level "+ String(powerLevel));
  //this->getLogger()->log(volts , SIGNAL_ID_VOLTAGE, LOG_LEVEL_DEBUG);

  return volts;
}

void ResourcesManager::print()
{
  INFO("ResourcesManager::print");
  float powerLevel = this->getPowerVoltage();
  INFO("power level "+ String((int)powerLevel));
  INFO("isPowered "+ String(this->isPowered()));
  INFO("isRaised "+ String(this->m_raised));
  INFO("Nokia Status "+ String(this->getNokia()->ready()));
  INFO("Led Status "+ String(this->getStatusLed()->getState()));
  this->m_settings->print();
}

EmbeddedSettings * ResourcesManager::getSettings()
{
  return this->m_settings;
}

Logger *ResourcesManager::getLogger()
{
  return this->m_logger;
}


bool ResourcesManager::getRaised()
{
  return this->m_raised;
}

void ResourcesManager::raised()
{
  INFO("Raising power failure");
  this->m_raised = true;
  this->m_settings->countFailure();
}



bool ResourcesManager::isPowered(void)
{
  return digitalRead(PIN_MAIN_POWER) == 1;
}

void ResourcesManager::update()
{
  if(this->isPowered())
  {
    this->m_lastPowerTime = millis();
    this->m_raised = false;
  }
}

bool ResourcesManager::powerFailure(void)
{
  // is now powered
  if(!this->isPowered())
  {
    // check delay
    if((long)(millis() - (this->m_lastPowerTime + this->m_settings->getPowerFailureDelay())) >= 0 )
    {
      return true;
    }
  }

  return false;
}

void ResourcesManager::sendSms(String _message, bool _start)
{
  INFO("Send to all");
  if(_start)
  {
    this->getNokia()->on();
    this->getNokia()->ensureReady();
  }
  // send message to each contact
  Contact ** contactList = this->getSettings()->getContactList();
  for(int i=0; i<CONTACT_MAX_COUNT; i++)
  {
   if(contactList[i] != NULL)
   {
     this->getNokia()->sendSms(contactList[i]->getNumber(), _message);
   }
  }
  if(_start)
  {
    this->getNokia()->off();
  }
}

void ResourcesManager::sendAlert()
{
  INFO("Sending alert");

  int powerFailureCount = this->getSettings()->getPowerFailureCount();
  bool isPowered = this->isPowered();
  float voltage = this->getPowerVoltage();

  int battery;

  if(voltage > 8.5)
  {
    battery = 3;
  }
  else
    if(voltage > 8)
    {
      battery = 2;
    }
    else
      if(voltage > 7)
      {
        battery = 1;
      }
      else
      {
        battery = 0;
      }

  String batString = "";
  String powered = "No";
  if(isPowered)
  {
    powered = "Yes";
  }
  else
  {
    batString = String("\nBat ")+battery+"/3";
  }
  String message = String("Alert(count ")+ powerFailureCount +")\nPowered "+ powered +batString;
  this->sendSms(message, true);

}

Stream * ResourcesManager::getFbusIo()
{
  return this->m_fbusIo;
}


ResourcesManager::~ResourcesManager() {
  // TODO Auto-generated destructor stub
}

