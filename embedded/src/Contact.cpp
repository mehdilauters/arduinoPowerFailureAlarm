/*
 * Contact.cpp
 *
 *  Created on: 17 sept. 2013
 *      Author: mehdi
 */

#include "Contact.h"
#include "Tools.h"


Contact::Contact(int _id, String _name, String _number) {
	if(_id > CONTACT_MAX_COUNT -1)
	{
		_id = 0;
	}
	this->m_id = _id;
	this->m_name = _name;
	this->m_number = _number;

}

void Contact::save(void)
{
	Tools::EEPROMsaveString(this->m_id * CONTACT_MAX_SIZE + CONTACT_OFFSET, this->m_name, CONTACT_MAX_NAME_SIZE );
	Tools::EEPROMsaveString(this->m_id * CONTACT_MAX_SIZE + CONTACT_MAX_NAME_SIZE + CONTACT_OFFSET, this->m_number, CONTACT_MAX_NUMBER_SIZE );
}




bool Contact::load(void)
{
	this->m_name = Tools::EEPROMreadString(		this->m_id * CONTACT_MAX_SIZE + CONTACT_OFFSET, CONTACT_MAX_NAME_SIZE );
	this->m_number = Tools::EEPROMreadString(	this->m_id * CONTACT_MAX_SIZE + CONTACT_OFFSET+ CONTACT_MAX_NAME_SIZE, CONTACT_MAX_NUMBER_SIZE );
	return this->m_name != "" && this->m_number != "";
}


String Contact::getName()
{
	return this->m_name;
}

String Contact::getNumber()
{
	return this->m_number;
}

void Contact::print()
{
//	for(int i=0; i < CONTACT_MAX_SIZE; i++)
//	{
//		Serial.write(EEPROM.read(this->m_id * CONTACT_MAX_SIZE + i) );
//	}
	Serial.println(">>>>>");
	Serial.println("Contact n " + String(this->m_id));
	Serial.println(this->m_name);
	Serial.println(this->m_number);
	Serial.println("<<<<<");
}

void Contact::testRw()
{
	for(int i = 0; i < CONTACT_MAX_COUNT; i++)
	{
		Contact c(i, String(i) + "_TestC" , String(i) + "+33_");
		c.print();
		c.save();
		Contact c1(i);
		c1.load();
		c1.print();
	}

	/*Contact c(0, "TestCmaaaaaaaaaaaaax_" + 0, "+33_" + 0);
	c.save();
	Contact c1(0);
	c1.load();
	c1.print();*/

}

Contact::~Contact() {
	// TODO Auto-generated destructor stub
}

