/*
 * EmbeddedSettings.h
 *
 *  Created on: 21 sept. 2013
 *      Author: mehdi
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_


#include "PowerFailureAlarmComon.h"
class Contact;

class Settings {
protected:
	int m_powerFailureDelay;
	int m_powerFailureCount;
	Contact *m_contactList[CONTACT_MAX_COUNT];
public:
	Settings();
	void setPowerFailureDelay(int _delay);
	void setPowerFailureCount(int _count);

	int getPowerFailureDelay(void);
	int getPowerFailureCount(void);

	virtual void save(bool _all) = 0;
	virtual void load(void) = 0;
	Contact **getContactList(void);

	virtual void print() = 0;

	virtual ~Settings();
};

#endif /* SETTINGS_H_ */
