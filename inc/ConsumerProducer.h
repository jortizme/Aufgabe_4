/*
 * ConsumerProducer.h
 *
 *  Created on: 14.09.2015
 *      Author: Wuebbelmann
 */

#ifndef CONSUMERPRODUCER_H_
#define CONSUMERPRODUCER_H_

#include "TaskBase.h"
#include "Semaphore.h"
#include "Ringbuffer.h"
#include "Mutex.h"
#include "TermColor.h"


namespace Erzeuger{


class Producer: public RTOS::Task{

public:

		Producer(const char * const name,
				uint16_t    stackDepth,
				osPriority  priority,
				RTOS::Semaphore* used_sem,
				RTOS::Semaphore* unused_sem,
				Ringbuffer<char>* buff,
				RTOS::Mutex* mutex_buff,
				RTOS::Mutex* mutex_printf,
				const Platform::BSP::TermColor::Color& color,
				bool        immidiateStart = true);

		~Producer();

protected:
	void m_task();

private:

	RTOS::Semaphore* m_used_sem;
	RTOS::Semaphore* m_unused_sem;
	Ringbuffer<char>* m_buff;
	osMessageQId m_Message_ID;
	RTOS::Mutex* m_mutex_buff;
	RTOS::Mutex* m_mutex_printf;
	Platform::BSP::TermColor::Color m_color;

	char pool[3];



	};
}

namespace 	Verbraucher{

class Consumer: public RTOS::Task{

public:

		Consumer(const char * const name,
				uint16_t    stackDepth,
				osPriority  priority,
				RTOS::Semaphore* used_sem,
				RTOS::Semaphore* unused_sem,
				Ringbuffer<char>* m_buf,
				RTOS::Mutex* mutex_buff,
				RTOS::Mutex* mutex_printf,
				const Platform::BSP::TermColor::Color& color,
				bool        immidiateStart = true);

		~Consumer();



protected:

	void m_task();

private:

	RTOS::Semaphore* m_used_sem;
	RTOS::Semaphore* m_unused_sem;
	Ringbuffer<char>* m_buff;
	osMessageQId m_Message_ID;
	RTOS::Mutex* m_mutex_buff;
	RTOS::Mutex* m_mutex_printf;
	Platform::BSP::TermColor::Color m_color;

	};

}
#endif /* CONSUMERPRODUCER_H_ */
