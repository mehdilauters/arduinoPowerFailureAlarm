/*
 * EmbeddedSettings.h
 *
 *  Created on: 26 sept. 2013
 *      Author: mehdi
 */

#ifndef EMBEDDEDSETTINGS_H_
#define EMBEDDEDSETTINGS_H_

#include "Settings.h"

class EmbeddedSettings: public Settings {
public:
	EmbeddedSettings();
	void countFailure(void);
	void save(/*bool _all = false*/);
	void load(void);
	void testRw(void);
	void print();
	virtual ~EmbeddedSettings();
};

#endif /* EMBEDDEDSETTINGS_H_ */
