/*
 * Contact.cpp
 *
 *  Created on: 17 sept. 2013
 *      Author: mehdi
 */

#include "Contact.h"



Contact::Contact(int _id, string _name, string _number) {
	if(_id > CONTACT_MAX_COUNT -1)
	{
		_id = 0;
	}
	this->m_id = _id;
	this->m_name = _name;
	this->m_number = _number;

}


string Contact::getName()
{
	return this->m_name;
}

string Contact::getNumber()
{
	return this->m_number;
}

void Contact::print()
{

	cout << ">>>>>" << endl;
	cout << "Contact n " <<	this->m_id  << endl;
	cout << this->m_name << endl;
	cout << this->m_number << endl;
	cout << "<<<<<" << endl;
}


Contact::~Contact() {
	// TODO Auto-generated destructor stub
}

