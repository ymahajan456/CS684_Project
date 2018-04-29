/*
// File name:      "Lab1_C.h"
// Description:    Change toggle delay and color of LED using switches

// Written By:     Yogesh Mahajan (y.mahajan@gmail.com) [14D070022, IIT Bombay]

// Dependencies:   "basic_includes.h"
*/
////////////////////////////////////////////////////////////////////////////////
#ifndef _LAB3_B_H_
#define _LAB3_B_H_

# include "basic_include.h"
#include "Tiva_SwitchManager.h"
#include "Tiva_Servo.h"

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


// Zero => do nothing 
EXTERN uint32_t g_ui32SystemStatus INITIALIZER(0);

extern void switchAction();
extern void taskSetup();
extern void ticker();
extern void ToDo();


#endif
