/*
 * PowerAlarmDialog.h
 *
 *  Created on: 22 sept. 2013
 *      Author: mehdi
 */

#ifndef POWERALARMDIALOG_H_
#define POWERALARMDIALOG_H_

#include <QtGui/QDialog>

namespace Ui {
    class PowerFailureAlarm;
}


class PowerAlarmDialog : public QDialog
{
	//Q_OBJECT
private:
    Ui::PowerFailureAlarm *ui;
public:
	PowerAlarmDialog(QWidget *parent = 0);
	virtual ~PowerAlarmDialog();
};

#endif /* POWERALARMDIALOG_H_ */
