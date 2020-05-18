/*
 * TextGenerator.cpp
 *
 *  Created on: 29.06.2016
 *      Author: Wuebbelmann
 */


/* TODO: implement textgenenerator task.
 * Implementation can be done in C style or by using an OOP approach.
 */


#include <TextGenerator.h>
#include "Application.h"

using GPIO = Platform::BSP::DigitalInOut;

char JW2men[sizeof(GPIO)];

const char text[] =
    "Habe nun, ach! Philosophie,\r\n"
    "Juristerei und Medizin,\r\n"
    "Und leider auch Theologie\r\n"
    "Durchaus studiert, mit heissem Bemuehn.\r\n"
    "Da steh ich nun, ich armer Tor!\r\n"
    "Und bin so klug als wie zuvor;\r\n"
    "Heisse Magister, heisse Doktor gar\r\n"
    "Und ziehe schon an die zehen Jahr\r\n"
    "Herauf, herab und quer und krumm\r\n"
    "Meine Schueler an der Nase herum -\r\n"
    "Und sehe, dass wir nichts wissen koennen!\r\n";



TextGenerator::TextGenerator(const char * const name,
						uint16_t    stackDepth,
						osPriority  priority,
						uint32_t    period,
						osThreadId Producer_ID,
						uint8_t Port,
						uint32_t Pin,
						Platform::BSP::DigitalInOut::Direction Direction ,
						bool        immidiateStart):

						RTOS::PeriodicTaskBase(name, stackDepth, priority,period,immidiateStart),
						m_Producer_ID(Producer_ID)
{
	strcpy(m_text,text);

	GPIO::initialize();

	JW2 =  new(JW2men) GPIO(Port,Pin,Direction);

	GPIO::InterruptHandler Button_Interrupt = [this](uint32_t i)
			{

				osSignalSet(this->m_Producer_ID, INT_EVENT);
		/*if(false == this->sendMessage(this->m_Producer_ID, this->m_text))
						printf("Incorrect parameters TextGenerator\n");*/

			};

	if(true == JW2->onInterrupt(Button_Interrupt, GPIO::EdgeType::FALLING_EDGE))
		{
			printf("Interrupt initialized\n");
		}
	else{
		printf("Interrupt initialization failed.\n");
	}

	this->finalizeInit();

}

TextGenerator::~TextGenerator(){}


void TextGenerator::m_task()
{

	while(true)
	{

		osEvent result = osSignalWait(RTOS::TIMEREVENT, osWaitForever);

		if(result.value.signals & RTOS::TIMEREVENT)
		{
			if(false == this->sendMessage(this->m_Producer_ID, this->m_text))
				printf("Incorrect parameters TextGenerator\n");


		}
	}
}




