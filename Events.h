/*
 * Events.h
 *
 *  Created on: Nov 4, 2018
 *      Author: Inaki
 */

#ifndef EVENTS_H_
#define EVENTS_H_

/* KSDK */
#include "fsl_common.h"
#include "EmbeddedTypes.h"
#include "fsl_os_abstraction.h"

/* Define the available Task's Events */
#define gMyEventTimer_c (1 << 0)
#define gEventButton1_c (1 << 1)
#define gEventButton2_c (1 << 2)
#define gEventButton3_c (1 << 3)
#define gEventButton4_c (1 << 4)


#endif /* EVENTS_H_ */
