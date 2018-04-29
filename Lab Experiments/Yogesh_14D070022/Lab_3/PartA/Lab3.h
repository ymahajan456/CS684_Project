/*
// File name:      "Lab1_C.h"
// Description:    Change toggle delay and color of LED using switches

// Written By:     Yogesh Mahajan (y.mahajan@gmail.com) [14D070022, IIT Bombay]

// Dependencies:   "basic_includes.h"
*/
////////////////////////////////////////////////////////////////////////////////
#ifndef _LAB3_A_H_
#define _LAB3_A_H_

# include "basic_include.h"
#include "Tiva_SwitchManager.h"
#include "Tiva_LED.h"

#ifdef DEFINE_VARIABLES
#define EXTERN                  
#define INITIALIZER(...)        = __VA_ARGS__
#else
#define EXTERN                  extern
#define INITIALIZER(...)       
#endif

#define USE_TIMER0
#define USE_TIMER1

#define AUTO 1
#define MANUAL 0

#define RED_TARGET 0
#define BLUE_TARGET 1
#define GREEN_TARGET 2

EXTERN int32_t g_ui32DebounceTime;
EXTERN int32_t g_ui32RPWM INITIALIZER(1);
EXTERN int32_t g_ui32GPWM INITIALIZER(1);
EXTERN int32_t g_ui32BPWM INITIALIZER(1);

EXTERN int32_t g_ui32Mode INITIALIZER(1);
EXTERN int32_t g_ui32ManualTarget INITIALIZER(1);

EXTERN int32_t g_ui32AutoSpeed INITIALIZER(50000);
EXTERN int32_t g_ui32ManualSpeed INITIALIZER(100);
EXTERN int32_t g_ui32SpeedCap;

// Zero => do nothing 
EXTERN uint32_t g_ui32SystemStatus INITIALIZER(0);
EXTERN uint32_t color[3];

extern void LEDCycle();
extern void modeDetect();
extern void switchAction();
extern void taskSetup();
extern void ticker();
extern void ToDo();


#endif
