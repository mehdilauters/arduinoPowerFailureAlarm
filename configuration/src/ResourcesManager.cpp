/*
 * ResourcesM.cpp
 *
 *  Created on: 25 sept. 2013
 *      Author: mehdi
 */

#include "ResourcesManager.h"
#include "SerialPortAccess.h"
#include "EmbeddedSettings.h"

ResourcesManager::ResourcesManager() {
	this->m_serialPort = new SerialPortAccess();
	this->m_settings = new EmbeddedSettings();

}

Settings *ResourcesManager::getSettings()
{
	return this->m_settings;
}

ResourcesManager::~ResourcesManager() {
	// TODO Auto-generated destructor stub
}

