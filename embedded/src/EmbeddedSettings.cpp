/*
 * EmbeddedSettings.cpp
 *
 *  Created on: 26 sept. 2013
 *      Author: mehdi
 */

#include "EmbeddedSettings.h"
#include "Tools.h"
#include "Contact.h"
#include "ResourcesManager.h"
#include "Logger.h"

#define MODULE_ID 10

EmbeddedSettings::EmbeddedSettings(ResourcesManager *_resourcesManager)
{
	this->m_powerFailureCount = 0;
	this->m_powerFailureDelay = SETTINGS_DEFAULT_DELAY;
	for(int i=0; i < CONTACT_MAX_COUNT; i++)
	{
		this->m_contactList[i] = NULL;
	}

	this->m_resourcesManager = _resourcesManager;
}

void EmbeddedSettings::defaultValues(void)
{
	INFO("Reseting to default values");
	this->m_powerFailureCount = 0;
	this->m_powerFailureDelay = SETTINGS_DEFAULT_DELAY;

	for(int i=0; i < CONTACT_MAX_COUNT; i++)
	{
		if( this->m_contactList[i] != NULL )
		{
			free(this->m_contactList[i]);
		}
	}

	int i=0;
	this->m_contactList[0] = new Contact(i, "Mehdi", "33687672875");
	i++;
	this->m_contactList[1] = new Contact(i, "Portable", "33666087175");
	i++;

	for(i; i < CONTACT_MAX_COUNT; i++)
	{
		this->m_contactList[i] = new Contact(i, "", "");
	}

	this->save(true);

	for(int i=1; i < CONTACT_MAX_COUNT; i++)
	{
		free(this->m_contactList[i]);
		this->m_contactList[i] = NULL;
	}
}

void EmbeddedSettings::countFailure(void)
{
	this->m_powerFailureCount++;
	this->save();
}

void EmbeddedSettings::save(bool _all)
{
	DEBUG("Saving data to eeprom");
	Tools::EEPROMwriteInt(RESOURCES_OFFSET + sizeof(int), this->m_powerFailureCount);
	if(_all)
	{
		Tools::EEPROMwriteInt(RESOURCES_OFFSET, this->m_powerFailureDelay);
		for(int i=0; i < CONTACT_MAX_COUNT; i++)
		{
			if( this->m_contactList[i] != NULL )
			{
				DEBUG("Contact "+String(i) + " name " + this->m_contactList[i]->getName() + " number " + this->m_contactList[i]->getNumber() );
				this->m_contactList[i]->save();
			}
		}
	}
}


void EmbeddedSettings::load(void)
{
	INFO("Loading data" );
	this->m_powerFailureDelay = Tools::EEPROMreadInt(RESOURCES_OFFSET);
	this->m_powerFailureCount = Tools::EEPROMreadInt(RESOURCES_OFFSET + sizeof(int));
	DEBUG("failure delay "+String(this->m_powerFailureDelay)+" failure count "+ String(this->m_powerFailureCount) );

	bool contactFound = false;

	for(int i=0; i < CONTACT_MAX_COUNT; i++)
	{
		this->m_contactList[i] = new Contact(i);
		if( ! this->m_contactList[i]->load() )
		{
			free(this->m_contactList[i]);
			this->m_contactList[i] = NULL;
		}
		else
		{
			DEBUG("Contact "+String(i) + " name " + this->m_contactList[i]->getName() + " number " + this->m_contactList[i]->getNumber() );
			contactFound = true;
		}
	}

	if(! contactFound )
	{
		WARNING("no contact found reseting values");
		this->defaultValues();
	}

}

void EmbeddedSettings::print()
{
	INFO("failureCount "+ String(this->m_powerFailureCount));
	INFO("failureDelay "+ String(this->m_powerFailureDelay));
	for(int i=0; i < CONTACT_MAX_COUNT; i++)
	{
		if( this->m_contactList[i] != NULL )
		{
			INFO("<<< Contact "+ String(i));
			INFO("name "+ this->m_contactList[i]->getName());
			INFO("number "+ this->m_contactList[i]->getNumber());
			INFO(">>>> Contact "+ String(i));
		}
		else
		{
			INFO(">>>> Contact "+ String(i) + " empty");
		}
	}

	Tools::EEPROMDump(this->m_resourcesManager->getLogger()->getStream(), (int)RESOURCES_OFFSET, RESOURCES_OFFSET+sizeof(int)*2+CONTACT_MAX_SIZE*CONTACT_MAX_COUNT);
}



EmbeddedSettings::~EmbeddedSettings() {
	// TODO Auto-generated destructor stub
}

