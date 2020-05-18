/*
 * DebugTask.cpp
 *
 *  Created on: 04.08.2015
 *      Author: Wuebbelmann
 */


#include <cstdlib>
#include "FreeRTOS.h"
#include "task.h"
#include "chip.h"

#include <TermColor.h>

#include "DebugTask.h"

using Color = Platform::BSP::TermColor;

TaskManager::TaskManager(const char * const name,
						uint16_t    stackDepth,
						osPriority  priority,
						uint32_t    period,
						bool        immidiateStart): RTOS::PeriodicTaskBase(name, stackDepth, priority,period,immidiateStart)
{
	this->finalizeInit();

}

TaskManager::~TaskManager(){}

void TaskManager::m_task()

{
	//char *pcWriteBuffer;
	TaskStatus_t *pxTaskStatusArray;
	volatile UBaseType_t uxArraySize, x;
	uint32_t ulTotalRunTime, ulStatsAsPercentage;
	char State[10];

	while(true)
	{
		osEvent result = osSignalWait(RTOS::TIMEREVENT, osWaitForever);



			if(result.value.signals & RTOS::TIMEREVENT){

				Platform::BSP::cout << Color::stdcol;
				printf("TASK MANAGER UPDATED EVERY 2 SECONDS\r\n");

			// Take a snapshot of the number of tasks in case it changes while this
			// function is executing.
			uxArraySize = uxTaskGetNumberOfTasks();

			// Allocate a TaskStatus_t structure for each task.  An array could be
			// allocated statically at compile time.
			pxTaskStatusArray = (TaskStatus_t *)pvPortMalloc( uxArraySize * sizeof( TaskStatus_t ) );

			if( pxTaskStatusArray != NULL )
			{
				// Generate raw status information about each task.
				uxArraySize = uxTaskGetSystemState( pxTaskStatusArray, uxArraySize, &ulTotalRunTime );

				// For percentage calculations.
				ulTotalRunTime /= 100UL;

				// Avoid divide by zero errors.
				if( ulTotalRunTime > 0 )
				{
					Platform::BSP::cout<<Color::red;
					printf ("\n\n%20s%10s%15s%10s%10s%15s%10s%10s\r\n", "NAME", "ID", "STATE", "PRIO", "BASE", "TIME", "%%CPU", "STACK");
					// For each populated position in the pxTaskStatusArray array,
					// format the raw data as human readable ASCII data

					Platform::BSP::cout<<Color::green;

					for( x = 0; x < uxArraySize; x++ )
					{
						// What percentage of the total run time has the task used?
						// This will always be rounded down to the nearest integer.
						// ulTotalRunTimeDiv100 has already been divided by 100.
						ulStatsAsPercentage = pxTaskStatusArray[ x ].ulRunTimeCounter / ulTotalRunTime;

						if(pxTaskStatusArray[x].eCurrentState==0) strcpy(State ,"Running");
						else if(pxTaskStatusArray[x].eCurrentState==1)  strcpy(State ,"Ready");
						else if(pxTaskStatusArray[x].eCurrentState==2)  strcpy(State ,"Blocked");
						else if(pxTaskStatusArray[x].eCurrentState==3)  strcpy(State ,"Suspended");
						else if(pxTaskStatusArray[x].eCurrentState==4)  strcpy(State ,"Deleted");
						else  strcpy(State ,"Invalid");

						if( ulStatsAsPercentage > 0UL )
						{
							printf(  "%20s%10u%15s%10u%10u%15u%10u%10u\r\n",
									pxTaskStatusArray[ x ].pcTaskName,
									(unsigned int)pxTaskStatusArray[ x ].xTaskNumber,
									State,
									(unsigned int) pxTaskStatusArray[ x ].uxCurrentPriority,
									(unsigned int) pxTaskStatusArray[ x ].uxBasePriority,
									(unsigned int)pxTaskStatusArray[ x ].ulRunTimeCounter,
									(unsigned int) ulStatsAsPercentage,
									(unsigned int)pxTaskStatusArray[ x ].usStackHighWaterMark);
						}
						else
						{
							// If the percentage is zero here then the task has
							// consumed less than 1% of the total run time.
							printf( "%20s%10u%15s%10u%10u%15u%10u%10u\r\n",
									pxTaskStatusArray[ x ].pcTaskName,
									(unsigned int)pxTaskStatusArray[ x ].xTaskNumber,
									State,
									(unsigned int)pxTaskStatusArray[ x ].uxCurrentPriority,
									(unsigned int)pxTaskStatusArray[ x ].uxBasePriority,
									(unsigned int)pxTaskStatusArray[ x ].ulRunTimeCounter,
									0,
									(unsigned int)pxTaskStatusArray[ x ].usStackHighWaterMark);
						}

						 //puts(pcWriteBuffer);

						//pcWriteBuffer += strlen( ( char * ) pcWriteBuffer );
					}
				}

				// The array is no longer needed, free the memory it consumes.
				vPortFree( pxTaskStatusArray );
			}
		}
	}


}




