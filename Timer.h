/*
 * practica02.h
 *
 *  Created on: Oct 29, 2018
 *      Author: Inaki
 */

#ifndef TIMER_H_
#define TIMER_H_


/* Fwk */
#include "TimersManager.h"
#include "FunctionLib.h"
#include "LED.h"
#include "Events.h"
/* KSDK */
#include "fsl_common.h"
#include "EmbeddedTypes.h"
#include "fsl_os_abstraction.h"


#define gMyTaskPriority_c 3
#define gMyTaskStackSize_c 400
#define gMyTimerTime	   3000

typedef enum {
	LED1counter = 1,
	LED2counter,
	LED3counter,
	LED4counter
} LEDstates_t;

void TimerTask(osaTaskParam_t argument);
void TimerTask_Init(void);

void StartTimer3s(void);

#endif /* TIMER_H_ */
