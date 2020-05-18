/*
 * ConsumerProducer.cpp
 *
 *  Created on: 14.09.2015
 *      Author: Wuebbelmann
 */


#include <ConsumerProducer.h>

#include <TermColor.h>
#include "Application.h"

/* TODO: implement consumer and producer task.
 * Implementation can be done in C style or by using an OOP approach.
 */


namespace Erzeuger{


	Producer::Producer(const char * const name,
						uint16_t    stackDepth,
						osPriority  priority,
						RTOS::Semaphore*  used_sem,
						RTOS::Semaphore* unused_sem,
						Ringbuffer<char>* buff,
						RTOS::Mutex* mutex_buff,
						RTOS::Mutex* mutex_printf,
						const Platform::BSP::TermColor::Color& color,
						bool        immidiateStart):

						RTOS::Task(name, stackDepth, priority,immidiateStart),
						m_used_sem(used_sem), m_unused_sem(unused_sem),
						m_buff(buff), m_mutex_buff(mutex_buff),m_mutex_printf(mutex_printf),m_color(color)
{
	//2
	osMessageQDef_t message = {.queue_sz=3, .item_sz= sizeof(char*), .pool = pool};

	m_Message_ID = this->createMessageBox(&message);

	this->finalizeInit();
}

Producer::~Producer(){}

void Producer::m_task()
{

	while(true)
	{
		char* letter = nullptr;

		osEvent result  = osSignalWait(RTOS::MESSAGEBOXEVENT | INT_EVENT, osWaitForever);

		if(result.value.signals & RTOS::MESSAGEBOXEVENT || result.value.signals & INT_EVENT )
		{
			result = osMessageGet(m_Message_ID,osWaitForever); //ggf. Zeit anpassen

			letter = (char*)result.value.p;

			while(*letter != '\0')
			{
				m_unused_sem->down();

				m_mutex_buff->lock();

				*m_buff << *letter;

				m_mutex_buff->unlock();

				m_used_sem->up();

				m_mutex_printf->lock();

				Platform::BSP::cout << m_color;

				printf("%c", *letter);

				m_mutex_printf->unlock();

				letter++;

			}
		}
	}

}

}


namespace Verbraucher
{

		Consumer::Consumer(const char * const name,
						uint16_t    stackDepth,
						osPriority  priority,
						RTOS::Semaphore* used_sem,
						RTOS::Semaphore* unused_sem,
						Ringbuffer<char>* buff,
						RTOS::Mutex* mutex_buff,
						RTOS::Mutex* mutex_printf,
						const Platform::BSP::TermColor::Color& color,
						bool        immidiateStart):

						RTOS::Task(name, stackDepth, priority,immidiateStart),
						m_used_sem(used_sem), m_unused_sem(unused_sem),
						m_buff(buff),m_mutex_buff(mutex_buff),m_mutex_printf(mutex_printf),m_color(color)
{
	this->finalizeInit();
}


Consumer::~Consumer(){}

void Consumer::m_task()
{
	while(true)
	{
		char letter = '?';

		m_used_sem->down();
		m_mutex_buff->lock();

		*m_buff >> letter;

		m_mutex_printf->lock();

		Platform::BSP::cout << m_color;

		printf("%c",letter);

		m_mutex_printf->unlock();

		//printf("Consumer %s read from FIFO: %c\n", m_Name,letter);

		m_mutex_buff->unlock();

		m_unused_sem->up();

	}

}



}



