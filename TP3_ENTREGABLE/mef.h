#ifndef MEF_H_
#define MEF_H_

#include "main.h"


enum stateFlag{
	KEEP, NEXT, NEXT_ON, NEXT_OFF, NEXT_ST, NEXT_SA
} sflg;

enum timerInterruptFlag{
	ON, OFF
} tiflg;

enum stateMachineStates{
	START, INIT, WAIT, ON, OFF, SET_TIME, SET_ALARM
} sms;

extern volatile tiflg FLAG_INT = OFF;
extern volatile sflg flag = KEEP;

void updateMef();
void state_START();
void state_INIT();
void state_WAIT();
void state_ON();
void state_SET_TIME();
void state_SET_ALARM();
sms getState();

#endif