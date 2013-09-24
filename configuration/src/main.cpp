/*
 * main.cpp
 *
 *  Created on: 31 janv. 2012
 *      Author: rox
 */


#include <QtCore>
#include <QApplication>
#include "PowerAlarmDialog.h"
#include <iostream>

//#include <signal.h>

//using namespace Noyau_Noyau;
using namespace std;

static PowerAlarmDialog* gui = NULL;

//void exitAnyway( int s){
//	std::cout <<"Noyau SIGNAL : CTRL+C detected" << endl;
//	exit(0);
//}

int main(int argc,char**argv) {

//	signal (SIGINT,exitAnyway);
//	signal (SIGTERM,exitAnyway);


	QApplication a(argc,argv);
	gui = new PowerAlarmDialog(NULL);
	gui->show();


	return a.exec();
}
