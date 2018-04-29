/*
// File name:      "Lab1_C.h"
// Description:    Change toggle delay and color of LED using switches

// Written By:     Yogesh Mahajan (y.mahajan@gmail.com) [14D070022, IIT Bombay]

// Dependencies:   "basic_includes.h"
*/
////////////////////////////////////////////////////////////////////////////////
#ifndef _LAB2_H_
#define _LAB2_H_

# include "basic_include.h"
#include "Tiva_SwitchManager.h"

#ifdef DEFINE_VARIABLES
#define EXTERN                  
#define INITIALIZER(...)        = __VA_ARGS__
#else
#define EXTERN                  extern
#define INITIALIZER(...)       
#endif

#define USE_TIMER0
#define USE_TIMER1

// Timer value for 0.5s delay
EXTERN int32_t g_ui32ToggleConstant;
// Timer scale
EXTERN int32_t g_ui32ToggleScale INITIALIZER(2);
// Timer value
EXTERN int32_t g_ui32ToggleTime;
// LED color
EXTERN int32_t g_ui32LEDColor INITIALIZER(2);
// Debouncing Time 10 ms
EXTERN int32_t g_ui32DebounceTime;

// Zero => do nothing 
EXTERN int32_t g_ui32SystemStatus INITIALIZER(0);

extern void taskSetup();
extern void toggleLED();
extern void ticker();
extern void ToDo();
extern void changeLEDConfig();

#endif
