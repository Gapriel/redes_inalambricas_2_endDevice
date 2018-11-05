/*
 * practica02.c
 *
 *  Created on: Oct 29, 2018
 *      Author: Inaki
 */


#include <Timer.h>
#include "PhyInterface.h"
#include "MacInterface.h"

/* Data request packet for sending UART input to the coordinator */
nwkToMcpsMessage_t *mpPacket;

instanceId_t   macInstance;

/* Information about the PAN we are part of */
panDescriptor_t mCoordInfo;

/* This is either the short address assigned by the PAN coordinator
   during association, or our own extended MAC address. */
uint8_t maMyAddress[8];

/* The devices address mode. If 2, then maMyAddress contains the short
   address assigned by the PAN coordinator. If 3, then maMyAddress is
   equal to the extended address. */
addrModeType_t mAddrMode;

/* The MSDU handle is a unique data packet identifier */
uint8_t mMsduHandle;

/* Number of pending data packets */
uint8_t mcPendingPackets;



extern osaEventId_t tasksEvents;

/* Global Variable to store our TimerID */
tmrTimerID_t myTimerID = gTmrInvalidTimerID_c;

/* Handler ID for task */
static osaTaskId_t gMyTaskHandler_ID;

/* OSA Task Definition*/
OSA_TASK_DEFINE(TimerTask, gMyTaskPriority_c, 1, gMyTaskStackSize_c, FALSE );

uint8_t timerCounter;

/* This is the function called by the Timer each time it expires */
static void myTaskTimerCallback(void *param)
{
	OSA_EventSet(tasksEvents, gMyEventTimer_c);
}


/* Function to init the task */
void TimerTask_Init(void)
{
	/* The instance of the MAC is passed at task creaton */
	gMyTaskHandler_ID = OSA_TaskCreate(OSA_TASK(TimerTask), NULL);
}

void StartTimer3s(void)
{
	TMR_StartIntervalTimer(myTimerID, /*myTimerID*/
				3000, /* Timer's Timeout */
				myTaskTimerCallback, /* pointer to myTaskTimerCallback function */
				NULL
				);
	OSA_EventSet(tasksEvents, gMyEventTimer_c);
}

/* Main custom task */
void TimerTask(osaTaskParam_t argument)
{
	osaEventFlags_t customEvent;
	myTimerID = TMR_AllocateTimer();
	timerCounter = 1;
	while(1)
	{
		OSA_EventWait(tasksEvents, gMyEventTimer_c, FALSE, osaWaitForever_c, &customEvent);
		if( !gUseRtos_c && !customEvent)
		{
			break;
		}

		if (timerCounter >= 5)
		{
			timerCounter = 1;
		}
		TMR_StopTimer(myTimerID);
		/* Turn leds OFF */
		LED_StopFlashingAllLeds();
		switch (timerCounter)
		{
		case LED1counter:
			Led_TurnOn(LED1);
			break;
		case LED2counter:
			Led_TurnOn(LED2);
			break;
		case LED3counter:
			Led_TurnOn(LED3);
			break;
		case LED4counter:
			Led_TurnOn(LED4);
			break;
		default:
			break;
		}
		/* Send message */
		if(mpPacket == NULL)
		{
			/* If the maximum number of pending data buffes is below maximum limit
			and we do not have a data buffer already then allocate one. */
			mpPacket = MSG_Alloc(sizeof(nwkToMcpsMessage_t) + gMaxPHYPacketSize_c);
		}

		if(mpPacket != NULL)
		{
			mpPacket->msgType = gMcpsDataReq_c;
			mpPacket->msgData.dataReq.pMsdu = (uint8_t*)(&mpPacket->msgData.dataReq.pMsdu) +
													  sizeof(mpPacket->msgData.dataReq.pMsdu);
//			mpPacket->msgData.dataReq.pMsdu[0] = "Counter:"+timerCounter;
			mpPacket->msgData.dataReq.pMsdu[0] = 'C';
			mpPacket->msgData.dataReq.pMsdu[1] = 'o';
			mpPacket->msgData.dataReq.pMsdu[2] = 'u';
			mpPacket->msgData.dataReq.pMsdu[3] = 'n';
			mpPacket->msgData.dataReq.pMsdu[4] = 't';
			mpPacket->msgData.dataReq.pMsdu[5] = 'e';
			mpPacket->msgData.dataReq.pMsdu[6] = 'r';
			mpPacket->msgData.dataReq.pMsdu[7] = ':';
			mpPacket->msgData.dataReq.pMsdu[8] = timerCounter + '0';
			mpPacket->msgData.dataReq.pMsdu[9] = '\0';
			/* Create the header using coordinator information gained during
			the scan procedure. Also use the short address we were assigned
			by the coordinator during association. */
			FLib_MemCpy(&mpPacket->msgData.dataReq.dstAddr, &mCoordInfo.coordAddress, 8);
			FLib_MemCpy(&mpPacket->msgData.dataReq.srcAddr, &maMyAddress, 8);
			FLib_MemCpy(&mpPacket->msgData.dataReq.dstPanId, &mCoordInfo.coordPanId, 2);
			FLib_MemCpy(&mpPacket->msgData.dataReq.srcPanId, &mCoordInfo.coordPanId, 2);
			mpPacket->msgData.dataReq.dstAddrMode = mCoordInfo.coordAddrMode;
			mpPacket->msgData.dataReq.srcAddrMode = mAddrMode;
			mpPacket->msgData.dataReq.msduLength = 10;
			/* Request MAC level acknowledgement of the data packet */
			mpPacket->msgData.dataReq.txOptions = gMacTxOptionsAck_c;
			/* Give the data packet a handle. The handle is
			returned in the MCPS-Data Confirm message. */
			mpPacket->msgData.dataReq.msduHandle = mMsduHandle++;
			/* Don't use security */
			mpPacket->msgData.dataReq.securityLevel = gMacSecurityNone_c;

			NWK_MCPS_SapHandler(mpPacket, macInstance);

			/* Prepare for another data buffer */
			mpPacket = NULL;
			mcPendingPackets++;
		}
		timerCounter++;
		TMR_StartIntervalTimer(myTimerID, /*myTimerID*/
						3000, /* Timer's Timeout */
						myTaskTimerCallback, /* pointer to myTaskTimerCallback function */
						NULL
						);
	}
}
