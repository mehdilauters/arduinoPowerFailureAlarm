/*
 * ResourcesManager.h
 *
 *  Created on: 25 sept. 2013
 *      Author: mehdi
 */

#ifndef RESOURCESM_H_
#define RESOURCESM_H_

class SerialPortAccess;
class Settings;

class ResourcesManager {
private:
	SerialPortAccess *m_serialPort;
	Settings *m_settings;
public:
	ResourcesManager();
	Settings *getSettings();
	virtual ~ResourcesManager();
};

#endif /* RESOURCESM_H_ */
