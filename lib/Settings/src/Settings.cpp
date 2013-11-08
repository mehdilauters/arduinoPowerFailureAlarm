/*
 * EmbeddedSettings.cpp
 *
 *  Created on: 21 sept. 2013
 *      Author: mehdi
 */

#include "EmbeddedSettings.h"
#include "Contact.h"

Settings::Settings() {
	this->m_powerFailureDelay = 0;
	this->m_powerFailureCount = 0;
}


void Settings::setPowerFailureDelay(int _delay)
{
	this->m_powerFailureDelay = _delay;
}

void Settings::setPowerFailureCount(int _count)
{
	this->m_powerFailureCount = _count;
}


Contact **Settings::getContactList(void)
{
	return this->m_contactList;
}

int Settings::getPowerFailureDelay(void)
{
	return this->m_powerFailureDelay;
}

int Settings::getPowerFailureCount(void)
{
	return this->m_powerFailureCount;
}



Settings::~Settings() {

}

