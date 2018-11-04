/*
 * Button.c
 *
 *  Created on: Oct 31, 2018
 *      Author: Inaki
 */

#include "Button.h"
#include "PhyInterface.h"
#include "MacInterface.h"

/* OSA Task Definition*/
OSA_TASK_DEFINE(ButtonTask, gButtonTaskPriority_c, 1, gButtonTaskStackSize_c, FALSE );

extern osaEventId_t tasksEvents;

/* Handler ID for task */
static osaTaskId_t gMyTaskHandler_ID;

const gpioInputPinConfig_t sw2_config = {
		gpioPort_C_c,
		4u,
		pinPull_Up_c,
		pinDma_FallingEdge_c
};
const gpioInputPinConfig_t sw3_config = {
		gpioPort_C_c,
		5u,
		pinPull_Up_c,
		pinDma_FallingEdge_c
};


extern uint8_t timerCounter;


/* This is the function called by the Timer each time it expires */
static void Button1_Callback(void *param)
{
	OSA_EventSet(tasksEvents, gEventButton1_c);
}

/* This is the function called by the Timer each time it expires */
static void Button2_Callback(void)
{
	OSA_EventSet(tasksEvents, gEventButton2_c);
	GpioClearPinIntFlag(&sw2_config);
}

/* This is the function called by the Timer each time it expires */
static void Button3_Callback(void)
{
	OSA_EventSet(tasksEvents, gEventButton3_c);
	GpioClearPinIntFlag(&sw3_config);
}

/* This is the function called by the Timer each time it expires */
static void Button4_Callback(void *param)
{
	OSA_EventSet(tasksEvents, gEventButton4_c);
}



/* Function to init the task */
void ButtonsTask_Init(void)
{
	GpioInstallIsr(Button2_Callback, gButtonTaskPriority_c, PORTB_PORTC_IRQn, &sw2_config);
	GpioInstallIsr(Button3_Callback, gButtonTaskPriority_c, PORTB_PORTC_IRQn, &sw3_config);

	/* The instance of the MAC is passed at task creaton */
	gMyTaskHandler_ID = OSA_TaskCreate(OSA_TASK(ButtonTask), NULL);
}

void ButtonTask(osaTaskParam_t argument)
{
	osaEventFlags_t customEvent;
	while(1)
	{
		OSA_EventWait(tasksEvents, gEventButton1_c | gEventButton2_c | gEventButton3_c | gEventButton4_c,
						FALSE, osaWaitForever_c, &customEvent);
		if( !gUseRtos_c && !customEvent)
		{
			break;
		}
		switch (customEvent)
		{
		case gEventButton1_c:
			timerCounter = 1;
			break;
		case gEventButton2_c:
			timerCounter = 2;
			break;
		case gEventButton3_c:
			timerCounter = 3;
			break;
		case gEventButton4_c:
			timerCounter = 4;
			break;
		default:
			break;
		}
		OSA_EventSet(tasksEvents, gMyEventTimer_c);

	}
}
