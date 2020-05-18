/*!
===============================================================================
 \name        : main.c
 \author      : $(author)
 \version     :
 \copyright   : $(copyright)
 \description : main definition
===============================================================================*/


/*!
 * \mainpage Assignment Goals
 * \section intro_sec Introduction
 *  In this assignment you should synchronize the communication between tasks.\n
 *  Please use the mechanisms provided by the \ref RTOS C++ encapsulation of RTOS mechanisms.
 *  \section task_sec Tasks
 *  \subsection TextGenerator
 *  Create a TextGeneratorTask\n
 *  The TextGenerator sends periodically a message to a Producer Task.
 *  \subsection Consumer
 *  Create a Consumer Task, there should be multiple instances of the Consumer Task.\n
 *  The Consumer Tasks reads items of type char out of a \ref Ringbuffer and prints them with printf on screen.\n
 *  The Consumers have to take care that no empty slots are read.
 *  \subsection Producer
 *  Create a Producer Task.\n
 *  The Producer received a message and writes the letters  in the \ref Ringbuffer. The produced items are printed.\n
 *  No unread slots are allowed to be overwritten.
 *  \subsection DebugTask
 *  A DebugTask shall write information about all tasks on the screen.
 *
 *
 *
 *
 */


#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

/* --System includes:   */
#include <memory>    // for placement new

/* --Platform includes. */
#include "chip.h"
#include <board_api.h>
#include <cmsis_os.h>

// appliaction includes
#include <Application.h>



static char appMem[sizeof(Application)];
static Application* app = nullptr;

int main(void) {

    osStatus xcode = osOK;
    /* --Initialize the underlying hardware. */
    SystemCoreClockUpdate();
    Board_Init();

    app = new(appMem) Application;  // RTOS reuses main stack, therefore  placement new!

    // initialize the Kernel
    xcode = osKernelInitialize();

    // Start the RTOS Kernel.
    // The scheduler will be started, never return back
    if (osOK == xcode)
    {
        osKernelStart();   // There should never be a return!
    }
    app->~Application();   // destroy application, should never happen.

    // Force the counter to be placed into memory
    volatile static int i = 0 ;
    // Enter an infinite loop, just incrementing a counter
    while(1) {
        i++ ;
    }
    return 0 ;
}
