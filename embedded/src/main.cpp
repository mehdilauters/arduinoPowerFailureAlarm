
#include "PowerFailureAlarm.h"
#include "Nokia3310.h"
#include "Led.h"
#include "ResourcesManager.h"
#include "EmbeddedSettings.h"
#include "Logger.h"
#include "Tools.h"
#define MODULE_ID 0



ResourcesManager *m_resourcesManager;

void setup()   {
 m_resourcesManager = new ResourcesManager();
// m_resourcesManager->getSettings()->print();
// m_resourcesManager->getSettings()->defaultValues();
}




void loop()
{
  bool failure = false;
  unsigned long failureTime = 0;
  while(1)
  {
    Stream *inputStream = NULL;
  inputStream =m_resourcesManager->getLogIo();
    if(inputStream->available())
        {
          char c = inputStream->read();
          switch (c) {
          case '0':
            m_resourcesManager->getNokia()->on();
            m_resourcesManager->getNokia()->ensureReady();
            break;
          case '1':
          {
            m_resourcesManager->sendSms("Test at "+ String(millis()) +"ms", true );
            break;
          }
          case '2':
            m_resourcesManager->getNokia()->off();
            break;
          case '3':
            m_resourcesManager->getSettings()->defaultValues();
              break;
          case '4':
            m_resourcesManager->print();
            break;
          case '5':
            m_resourcesManager->sendAlert();
            break;
          case '6':
            m_resourcesManager->getNokia()->sendSms("33687672875","nazdar " + String(millis()), true);
            break;
          case 'h':
            ERROR("0 NokiaOn\n1 smsTest\n2 nokiaOff\n3 defaultValues\n4 status\n5 alert\n? help");
          break;
          default:
            WARNING("unknown "+String(c));
            break;
        }

        }
    m_resourcesManager->getStatusLed()->blink(1000);
    m_resourcesManager->update();
    if(! m_resourcesManager->isPowered()) // if not powered
    {
      m_resourcesManager->getStatusLed()->blink(LED_FAILURE_BLINK_TIME);


      if(m_resourcesManager->powerFailure() && !m_resourcesManager->getRaised()) // if not powered and not raised
      {
        m_resourcesManager->getStatusLed()->on();
        failureTime = millis();
        failure = true;
        m_resourcesManager->raised();
        m_resourcesManager->sendAlert();

      }
    }
    else
    {
      if( failure )
      {
          failure = false;
          String time = Tools::print_time(millis() - failureTime);

          String message = "Power back "+String(time);
          m_resourcesManager->sendSms(message, true);
          m_resourcesManager->getStatusLed()->off();
      }
    }
    delay(100);
  }
}
