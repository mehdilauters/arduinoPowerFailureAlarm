/*
 * Contact.h
 *
 *  Created on: 17 sept. 2013
 *      Author: mehdi
 */

#ifndef CONTACT_H_
#define CONTACT_H_

#include "PowerFailureAlarmComon.h"

#include <iostream>
using namespace std;

class Contact {
private:
	int m_id;
	string m_name;
	string m_number;
public:
	Contact(int _id, string _name = "", string _number = "");

	string getName();
	string getNumber();
	void print();


	virtual ~Contact();
};

#endif /* CONTACT_H_ */
