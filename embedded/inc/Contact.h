/*
 * Contact.h
 *
 *  Created on: 17 sept. 2013
 *      Author: mehdi
 */

#ifndef CONTACT_H_
#define CONTACT_H_
#include "PowerFailureAlarm.h"
#include "PowerFailureAlarmComon.h"

#define CONTACT_MAX_SIZE (CONTACT_MAX_NAME_SIZE + CONTACT_MAX_NUMBER_SIZE)

class Contact {
private:
	int m_id;
	String m_name;
	String m_number;
public:
	Contact(int _id, String _name = "", String _number = "");
	String getName();
	String getNumber();
	void save(void);
	bool load(void);
	void print();
	static void testRw();

	virtual ~Contact();
};

#endif /* CONTACT_H_ */
