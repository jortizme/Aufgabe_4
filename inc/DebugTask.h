/*
 * DebugTask.h
 *
 *  Created on: 04.08.2015
 *      Author: Wuebbelmann
 */

#ifndef DEBUGTASK_H_
#define DEBUGTASK_H_

#include "PeriodicTaskBase.h"

class TaskManager : public RTOS::PeriodicTaskBase{

public:

	TaskManager(const char * const name,
    			uint16_t    stackDepth,
				osPriority  priority,
				uint32_t    period,
				bool        immidiateStart = true);

	~TaskManager();

/*Task Funktionalität*/
/*Inhrited from RTOS::Task*/
protected:

	void m_task();

};

#endif /* DEBUGTASK_H_ */
