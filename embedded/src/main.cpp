
#include "PowerFailureAlarm.h"
#include "Nokia3310.h"
#include "ResourcesManager.h"
#include "Contact.h"

ResourcesManager m_resourcesManager;
Nokia3310 m_nokia;

void setup()   {
  Serial.begin(115200);
//  delay(2000);
//  Contact v(1);
//  int i;
//  for(i=0;i<127;i++)
//  {
  //  Nokia3310::n61Handler(0,'U');
//  }
//  delay(2000);
  //Nokia3310 * nokia = new Nokia3310();
//  delay(2000);
//  Serial.print("===>");
//  Serial.println(millis());
  //nokia->smsSend("0687672875", "salut");

//  Contact::testRw();

//  m_resourcesManager.testRw();

  m_resourcesManager.load();
  pinMode(PIN_POWER_FAILURE_LED, OUTPUT);

}



void loop()
{
  while(1)
  {
	  m_resourcesManager.update();
	  if(! m_resourcesManager.isPowered()) // if not powered
	  {
//		  Serial.println("Not powered");
		  digitalWrite(PIN_POWER_FAILURE_LED, HIGH);


		  if(m_resourcesManager.powerFailure() && !m_resourcesManager.getRaised()) // if not powered and not raised
		  {
			  m_resourcesManager.raised();
			  m_resourcesManager.print();
			  // send message to each contact
			 for(int i=0; i<CONTACT_MAX_COUNT; i++)
			 {
				 Contact ** contact = m_resourcesManager.getContactList();
				 contact[i]->load();
				 m_nokia.sendSms(contact[i]->getNumber(), "Power alert" + contact[i]->getName());
				 contact[i]->unload();
			 }
		  }
	  }
	  else
	  {
//		  Serial.println("powered");
		  digitalWrite(PIN_POWER_FAILURE_LED, LOW);
	  }
	  delay(100);
  }
}
