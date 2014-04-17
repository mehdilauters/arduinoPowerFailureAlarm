arduinoPowerFailureAlarm
========================

![alarm picture](http://lauters.fr/blog/wp-content/uploads/2013/10/box_opened.jpg "Power Failure Alarm")

This is the source code for an arduino and nokia 3310 based power failure alarm.
Designed to be reliable chip and modulable.

The embedded program is working and tested on arduino Uno and Micro
The configuration tools to be run on the computer is not finished. It is based on Qt


Features of the alarm:
- contact list stored in eeprom
- count of all power failures stored in eeprom
- delay between power failure and sms alert stored in eeprom
- eeprom reset if data not valid
- sms indicating the failure, and the battery status
- sms indicating that  the power is back and the time of failure
- debug serial commands to switch on/off the phone, reset settings, simulate alert...
- debug messages can be enabled at compilation time


Hardware schematic used is
![hardware layout](http://lauters.fr/blog/wp-content/uploads/2013/10/layout.png "Hardware Layout")

More information available at http://lauters.fr/blog/arduino-power-failure-alarm/

All project uses CMake.

To build and upload the software:
- update the source to set up your contact list 
embedded/src/EmbeddedSettings.cpp at void EmbeddedSettings::defaultValues(void)
	this->m_contactList[i] = new Contact(i, "contact1", "3366XXXX");
	i++;
	this->m_contactList[i] = new Contact(i, "contact2", "3367XXXX");
	i++;
- create a build directory and go into it
	mkdir /tmp/build
	cd /tmp/build
- build the embedded project
	cmake /path/to/the/sources
	make PowerFailureAlarm
- upload the binary to the arduino
	make PowerFailureAlarm-upload

Your alarm is ready to use

Attention : because the sms library is not well debugged and arduino as a very small amount of RAM, sms with more than several words may leads to a reboot.