/*
 * SerialPortAccess.cpp
 *
 *  Created on: 26 sept. 2013
 *      Author: mehdi
 */

#include "SerialPortAccess.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QtCore/QDebug>

QT_USE_NAMESPACE

SerialPortAccess::SerialPortAccess() {
	  // Example use QSerialPortInfo
	    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
	        qDebug() << "Name        : " << info.portName();
	        qDebug() << "Description : " << info.description();
	        qDebug() << "Manufacturer: " << info.manufacturer();

	        // Example use QSerialPort
	        QSerialPort serial;
	        serial.setPort(info);
	        if (serial.open(QIODevice::ReadWrite))
	            serial.close();
	    }

}

SerialPortAccess::~SerialPortAccess() {
	// TODO Auto-generated destructor stub
}

