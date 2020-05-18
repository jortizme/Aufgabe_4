/*
 * TextGenerator.h
 *
 *  Created on: 29.06.2016
 *      Author: Wuebbelmann
 */

#ifndef INC_TEXTGENERATOR_H_
#define INC_TEXTGENERATOR_H_

#include "PeriodicTaskBase.h"
#include "FreeRTOS.h"
#include "digitalinout.h"


class TextGenerator : public RTOS::PeriodicTaskBase{

public:

	TextGenerator(const char * const name,
    			uint16_t    stackDepth,
				osPriority  priority,
				uint32_t    period,
				osThreadId Producer_ID,
				uint8_t Port,
				uint32_t Pin,
				Platform::BSP::DigitalInOut::Direction Direction = Platform::BSP::DigitalInOut::Direction::INPUT,
				bool        immidiateStart = true);

	~TextGenerator();

	Platform::BSP::DigitalInOut* JW2;


/*Inhrited from RTOS::Task*/
private:

	osThreadId m_Producer_ID;
	char m_text[361];


protected:

	void m_task();


};



#endif /* INC_TEXTGENERATOR_H_ */
