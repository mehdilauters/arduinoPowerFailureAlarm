/*
 * ResourcesManager.cpp
 *
 *  Created on: 21 sept. 2013
 *      Author: mehdi
 */

#include "ResourcesManager.h"
#include "Tools.h"
#include "Contact.h"

ResourcesManager::ResourcesManager() {
	pinMode(PIN_POWER, INPUT);
	this->m_powerFailureDelay = 0;
	this->m_powerFailureCount = 0;
	this->m_lastPowerTime = 0;
	this->m_raised = false;
}

void ResourcesManager::save(bool _all)
{
	Tools::EEPROMwriteInt(RESOURCES_OFFSET, this->m_powerFailureDelay);
	Tools::EEPROMwriteInt(RESOURCES_OFFSET + sizeof(int), this->m_powerFailureCount);
	if(_all)
	{
		for(int i=0; i < CONTACT_MAX_COUNT; i++)
			{
				this->m_contactList[i]->save();
			}
	}
}


void ResourcesManager::load(void)
{
	this->m_powerFailureDelay = Tools::EEPROMreadInt(RESOURCES_OFFSET);
	this->m_powerFailureCount = Tools::EEPROMreadInt(RESOURCES_OFFSET + sizeof(int));

	for(int i=0; i < CONTACT_MAX_COUNT; i++)
	{
		this->m_contactList[i] = new Contact(i);
	}
}

void ResourcesManager::testRw(void)
{
	this->m_powerFailureCount = 10;
	this->m_powerFailureDelay = 200;

	this->save();

	Contact::testRw();

	this->m_powerFailureCount = 0;
	this->m_powerFailureDelay = 0;


	this->load();

	this->print();

}

void ResourcesManager::print()
{
	Serial.println("*******ResourcesManager****");
	Serial.println("Count => "+ String(this->m_powerFailureCount));
	Serial.println("Delay => "+ String(this->m_powerFailureDelay));
}

bool ResourcesManager::getRaised()
{
	return this->m_raised;
}

void ResourcesManager::raised()
{
	this->m_raised = true;
	this->m_powerFailureCount++;
}

void ResourcesManager::setPowerFailureDelay(int _delay)
{
	this->m_powerFailureDelay = _delay;
}

void ResourcesManager::setPowerFailureCount(int _count)
{
	this->m_powerFailureCount = _count;
}

bool ResourcesManager::isPowered(void)
{
	return digitalRead(PIN_POWER) == 1;
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
//		Serial.println(String(millis())+" - "+String(this->m_lastPowerTime) +" ("+String(millis() - this->m_lastPowerTime)+") > "+ String(this->m_powerFailureDelay));
		// check delay
		if(millis() - this->m_lastPowerTime > this->m_powerFailureDelay)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}

Contact **ResourcesManager::getContactList(void)
{
	return this->m_contactList;
}


ResourcesManager::~ResourcesManager() {
	// TODO Auto-generated destructor stub
}

