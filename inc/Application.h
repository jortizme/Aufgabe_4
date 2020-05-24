/*
 * Application.h
 *
 *  Created on: 23.06.2016
 *      Author: Wuebbelmann
 */

/**
 *  \brief     Application.
 *  \details   starts all tasks.\n
 *
 *  \author    J. Wuebbelmann
 *  \version   1.0
 *  \date      23.06.2016
 *  \pre       First initialize the system, based on a CMSIS RTOS API compliant RTOS
 *  \copyright HS Osnabrueck
 */

/** @file */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <cmsis_os.h>

#include <Ringbuffer.h>

#include <ConsumerProducer.h>
#include <DebugTask.h>
#include <TextGenerator.h>
#include <Mutex.h>
#include <Semaphore.h>
#include <TermColor.h>

#define INT_EVENT (0x01 << 6)

class Application {
public:
	Application();
	~Application();
private:

	Erzeuger::Producer PRO;
	TaskManager MNG;
	TextGenerator GEN;
	Verbraucher::Consumer CON1;
	Verbraucher::Consumer CON2;
	Verbraucher::Consumer CON3;

};

#endif /* APPLICATION_H_ */
