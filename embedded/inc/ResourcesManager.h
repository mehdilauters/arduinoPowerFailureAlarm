/*
 * ResourcesManager.h
 *
 *  Created on: 21 sept. 2013
 *      Author: mehdi
 */

#ifndef RESOURCESMANAGER_H_
#define RESOURCESMANAGER_H_


#include "PowerFailureAlarm.h"
class EmbeddedSettings;
class Nokia3310;
class Led;
class Logger;

class ResourcesManager {
private:
	Stream *m_fbusIo;
	Stream *m_logIo;
	Logger *m_logger;
	EmbeddedSettings * m_settings;
	Nokia3310 * m_nokia;
	Led * m_statusLed;
	int m_lastPowerTime;
	bool m_raised;
public:
	ResourcesManager();

	bool isPowered(void);

	bool powerFailure(void);

	bool getRaised();
	void raised();

	Nokia3310 * getNokia();
	Led * getStatusLed();

	float getPowerVoltage();

	void testRw(void);
	void print();
	void update();
	EmbeddedSettings * getSettings();
	Logger *getLogger();

	Stream * getLogIo();
	void sendAlert();
	void sendSms(String _message, bool _start = false);
	Stream * getFbusIo();

	virtual ~ResourcesManager();
};


#endif /* RESOURCESMANAGER_H_ */
