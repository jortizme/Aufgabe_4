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
				RTOS::Mutex* mutex_printf,
				bool        immidiateStart = true);

	~TaskManager();

/*Task Funktionalität*/
/*Inhrited from RTOS::Task*/
protected:

	void m_task();

private:
	RTOS::Mutex* m_mutex_printf;

};

#endif /* DEBUGTASK_H_ */
