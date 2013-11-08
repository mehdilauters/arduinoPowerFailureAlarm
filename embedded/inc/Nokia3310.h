/*
 * Nokia3310.h
 *
 *  Created on: 21 sept. 2013
 *      Author: mehdi
 */

#ifndef NOKIA3310_H_
#define NOKIA3310_H_

#include "PowerFailureAlarm.h"

// seconds
#define NOKIA_STARTING_TIME 10


class ResourcesManager;

class Nokia3310 {
private:
  static ResourcesManager *m_resourcesManager;
  int m_pinEnable;
  unsigned long m_startedTime;
  bool m_status;
  bool m_initialized;
public:
  Nokia3310(ResourcesManager *_resourcesManager, int _pinEnable = PIN_NOKIA_CONTROLE);
  void on(bool _force = false);
  void off(bool _force = false);
  bool ready();
  bool started();
  void ensureReady();
  void sendSms(String _number, String _message, bool _start = false);
  static int handler(int cmd, unsigned char c);
  virtual ~Nokia3310();
};

#endif /* NOKIA3310_H_ */
