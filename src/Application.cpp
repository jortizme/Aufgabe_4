/*
 * Application.cpp
 *
 *  Created on: 23.06.2016
 *      Author: Wuebbelmann
 */

#include <Application.h>

using Platform::BSP::cout;

#define JOYCPORT 0
#define JOYCPIN  17

const uint32_t bufferSize = (1 << 4) ;   // we want to have 16 elements in buffer.
char mem[bufferSize];

char ConsumerMem[sizeof(Verbraucher::Consumer)];

/* ringbuffer stores data, producer and consumer need reference or pointer to ringbuffer. */
Ringbuffer<char> ringbuffer(bufferSize, '?', mem);

static osSemaphoreDef_t unused_sem = {.name = "unused"};
static osSemaphoreDef_t used_sem = { .name = "used"};
static RTOS::Semaphore sem1 = RTOS::Semaphore(&unused_sem,bufferSize);
static RTOS::Semaphore sem2 = RTOS::Semaphore(&used_sem,0);

static osMutexDef_t mutex1 = {.name = "buff_mutex"};
static RTOS::Mutex buff_mutex = RTOS::Mutex(&mutex1);

static osMutexDef_t mutex2 = {.name = "printf_mutex"};
static RTOS::Mutex printf_mutex = RTOS::Mutex(&mutex2);



Application::Application():
		PRO("Producer",1024,osPriorityLow,&sem2,&sem1, &ringbuffer,&buff_mutex,&printf_mutex,Platform::BSP::TermColor::black,true),
		MNG("Manager",1024,osPriorityNormal,2000,&printf_mutex,true),
		GEN("Generator",1024,osPriorityNormal,5000,PRO.getTaskHandle(),JOYCPORT,JOYCPIN,Platform::BSP::DigitalInOut::Direction::INPUT,true),
		CON1("Consumer1",1024,osPriorityLow,&sem2,&sem1, &ringbuffer,&buff_mutex,&printf_mutex,Platform::BSP::TermColor::blue, true),
		CON2("Consumer2",1024,osPriorityLow,&sem2,&sem1, &ringbuffer,&buff_mutex,&printf_mutex,Platform::BSP::TermColor::red,true),
		CON3("Consumer3",1024,osPriorityLow,&sem2,&sem1, &ringbuffer,&buff_mutex,&printf_mutex,Platform::BSP::TermColor::green,true)

{
    cout << Platform::BSP::TermColor::cls
         << Platform::BSP::TermColor::blue
         << "Starting Application"
         << Platform::BSP::TermColor::stdcol
         << Platform::BSP::TermColor::endl;
}

Application::~Application()
{
	delete GEN.JW2;
}

