/*
 * PowerAlarmDialog.cpp
 *
 *  Created on: 22 sept. 2013
 *      Author: mehdi
 */

#include "PowerAlarmDialog.h"
#include <QtGui>
#include <QtCore>

#include "ui_PowerFailureAlarmDialog.h"

PowerAlarmDialog::PowerAlarmDialog(QWidget *parent)
:  QDialog(parent),
   ui(new Ui::PowerFailureAlarm)
{
	ui->setupUi(this);

}

PowerAlarmDialog::~PowerAlarmDialog() {
	// TODO Auto-generated destructor stub
}

