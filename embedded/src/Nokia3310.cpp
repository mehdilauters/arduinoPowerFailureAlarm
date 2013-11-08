/*
 * Nokia3310.cpp
 *
 *  Created on: 21 sept. 2013
 *      Author: mehdi
 */

#include "n61sms.h"
#include "Nokia3310.h"
#include "Logger.h"
#include "ResourcesManager.h"

#define MODULE_ID 20

ResourcesManager *Nokia3310::m_resourcesManager;

Nokia3310::Nokia3310(ResourcesManager *_resourcesManager,int _pinEnable) {
  Nokia3310::m_resourcesManager = _resourcesManager;
  this->m_pinEnable = _pinEnable;
  this->m_status = false;
  this->m_initialized = false;
  pinMode(this->m_pinEnable, OUTPUT);
  digitalWrite(this->m_pinEnable, LOW);
}

void Nokia3310::ensureReady()
{
  bool ready = false;
  while(!this->ready())
  {
    while(!this->started())
    {
      delay(100);
    }
    if(!this->ready())
    {
      ERROR("not ready");
      this->on(true);
    }
  }
}

bool Nokia3310::started()
{
  return this->m_status && ( (long)(millis() - (this->m_startedTime + NOKIA_STARTING_TIME*1000)) >= 0);
}

bool Nokia3310::ready()
{
  bool ready = this->started();
  if(ready && !this->m_initialized)
  {
    INFO("get smsc");
    this->m_initialized = n61_init(Nokia3310::handler);
    INFO("ok");
  }
  ready = ready && this->m_initialized;
  VERBOSE("ready:"+String(ready));
  return ready;
}


void Nokia3310::on(bool _force)
{
  INFO("Start");

  bool status = this->m_status;
  this->m_status = true;
  this->m_startedTime = millis();

  if(!(!status || _force))
  {
  WARNING("already on");
    return;
  }

  digitalWrite(this->m_pinEnable, HIGH);
  delay(3000);
  digitalWrite(this->m_pinEnable, LOW);

}

void Nokia3310::off(bool _force)
{
  INFO("Stopping Nokia");

  bool status = this->m_status;
  this->m_status = false;
  this->m_initialized = false;
  
  if(!(status || _force))
  {
    WARNING("already off");
    return;
  }
  digitalWrite(this->m_pinEnable, HIGH);
  delay(3000);
  digitalWrite(this->m_pinEnable, LOW);
}



void Nokia3310::sendSms(String _number, String _message, bool _start)
{
  if(_start)
  {
    this->on();
    for(int i=0;i<1000;i++)
    {
      delay(100);
      if(this->ready())
      {
        break;
      }
    }
  }

  if(!this->ready())
  {
    this->off(true);
    WARNING("not ready");
    return;
  }
      INFO("sms to " + _number);
      VERBOSE(_message);

  char * bufferNumber = (char*) malloc((_number.length()+1)*sizeof(char));
  _number.toCharArray(bufferNumber, _number.length()+1);

  char * bufferMsg = (char*) malloc((_message.length()+1)*sizeof(char));
    _message.toCharArray(bufferMsg, _message.length()+1);

  n61_smssend(bufferNumber , bufferMsg);

  free(bufferNumber);
  free(bufferMsg);
  INFO("sent");
  if(_start)
  {
    this->off();
  }
}

Nokia3310::~Nokia3310() {
  // TODO Auto-generated destructor stub
}

int Nokia3310::handler(int cmd, unsigned char c)
{
  if(cmd == 0) {
          Nokia3310::m_resourcesManager->getFbusIo()->write(c);          // send c out the port
                  return 0;
              }
              else if(cmd == 1)
              {
                unsigned char c = Nokia3310::m_resourcesManager->getFbusIo()->read();
          //Serial.write(c);
                  return c;    // return the next byte in queue
              }
              else if(cmd == 2)
                  return Nokia3310::m_resourcesManager->getFbusIo()->available();  // return zero if queue is empty
              else if(cmd == 3)
                  delay(1);               // delay for 1 millisecond
}
