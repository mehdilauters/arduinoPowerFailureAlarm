/*
 * ResourcesManager.h
 *
 *  Created on: 21 sept. 2013
 *      Author: mehdi
 */

#ifndef RESOURCESMANAGER_H_
#define RESOURCESMANAGER_H_


#include "PowerFailureAlarm.h"
class Contact;

class ResourcesManager {
private:
	int m_powerFailureDelay;
	int m_powerFailureCount;
	int m_lastPowerTime;
	bool m_raised;
	Contact *m_contactList[CONTACT_MAX_COUNT];
public:
	ResourcesManager();
	void setPowerFailureDelay(int _delay);
	void setPowerFailureCount(int _count);
	void save(bool _all = false);
	void load(void);

	Contact **getContactList(void);
	bool isPowered(void);

	bool powerFailure(void);

	bool getRaised();
	void raised();

	void testRw(void);
	void print();
	void update();

	virtual ~ResourcesManager();
};

#endif /* RESOURCESMANAGER_H_ */
