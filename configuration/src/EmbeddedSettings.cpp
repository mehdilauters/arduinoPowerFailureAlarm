/*
 * EmbeddedSettings.cpp
 *
 *  Created on: 26 sept. 2013
 *      Author: mehdi
 */

#include "EmbeddedSettings.h"
#include "Contact.h"

EmbeddedSettings::EmbeddedSettings()
{


}



void EmbeddedSettings::save(/*bool _all*/)
{
	// TODO impl
	cerr << "EmbeddedSettings::save TODO"<<endl;
}


void EmbeddedSettings::load(void)
{
	// TODO impl
		cerr << "EmbeddedSettings::LOAD TODO"<<endl;
}

void EmbeddedSettings::print()
{
	// TODO impl
		cerr << "EmbeddedSettings::PRINT TODO"<<endl;
}

void EmbeddedSettings::testRw(void)
{
	this->m_powerFailureCount = 10;
	this->m_powerFailureDelay = 200;

	this->save();

//	Contact::testRw();

	this->m_powerFailureCount = 0;
	this->m_powerFailureDelay = 0;


	this->load();

	this->print();

}


EmbeddedSettings::~EmbeddedSettings() {
	// TODO Auto-generated destructor stub
}

