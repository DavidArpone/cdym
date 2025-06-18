#ifndef MEF_H_
#define MEF_H_

#include "main.h"

enum stateMachineStates{
	START, INIT, WAIT, ST_ON, ST_OFF, SET_TIME, SET_ALARM
};

enum stateFlag{
	KEEP, NEXT, NEXT_ON, NEXT_OFF, NEXT_ST, NEXT_SA
} flag;

enum timerInterruptFlag{
	ON, OFF
} FLAG_INT;

enum interruptTimerStateFlag{
	ENABLED, DISABLED
} FLAG_ON;



void updateMef();
void state_START();
void state_INIT();
void state_WAIT();
void state_ON();
void state_SET_TIME();
void state_SET_ALARM();
enum stateMachineStates getState();

#endif