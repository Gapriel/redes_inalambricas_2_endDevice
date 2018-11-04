/*
 * Button.h
 *
 *  Created on: Oct 31, 2018
 *      Author: Inaki
 */

#ifndef BUTTON_H_
#define BUTTON_H_

/* Fwk */
#include "pin_mux.h"
#include "GPIO_Adapter.h"
#include "FunctionLib.h"
#include "LED.h"
#include "Events.h"
/* KSDK */
#include "fsl_common.h"
#include "EmbeddedTypes.h"
#include "fsl_os_abstraction.h"

#define gButtonTaskPriority_c 3
#define gButtonTaskStackSize_c 400

extern void ButtonTask(osaTaskParam_t argument);
extern void ButtonsTask_Init(void);

#endif /* BUTTON_H_ */
