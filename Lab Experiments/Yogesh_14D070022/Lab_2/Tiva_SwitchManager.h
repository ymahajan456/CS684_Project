/*
// File name:      "Lab1_C.h"
// Description:    Change toggle delay and color of LED using switches

// Written By:     Yogesh Mahajan (y.mahajan@gmail.com) [14D070022, IIT Bombay]

// Dependencies:   "basic_includes.h"
*/
////////////////////////////////////////////////////////////////////////////////
#ifndef _TIVA_SWITCH_MANAGER_H_
#define _TIVA_SWITCH_MANAGER_H_

# include "basic_include.h"

#ifdef DEFINE_VARIABLES
#define EXTERN                  
#define INITIALIZER(...)        = __VA_ARGS__
#else
#define EXTERN                  extern
#define INITIALIZER(...)       
#endif

#define SW_UNPRESSED   1   // Unpressed State 
#define SW_PRESSED     4   // Pressed State
#define SW_PRESSWAIT   2   // press wait
#define SW_RELESEWAIT  8   // release wait

EXTERN int32_t g_ui32SW1PressTime INITIALIZER(0);
EXTERN int32_t g_ui32SW2PressTime INITIALIZER(0);

EXTERN int32_t g_ui32SW1PressCount INITIALIZER(0);
EXTERN int32_t g_ui32SW2PressCount INITIALIZER(0);

EXTERN int32_t g_ui32SW1LongPress INITIALIZER(0);
EXTERN int32_t g_ui32SW2LongPress INITIALIZER(0);

EXTERN int32_t g_ui32SW1FSMState INITIALIZER(1);
EXTERN int32_t g_ui32SW2FSMState INITIALIZER(1);

EXTERN int32_t g_ui32ReadValue;



extern void switchManager(int32_t *ui32PortStatus, int32_t ui32SWMask, int32_t *ui32SWFSMState, int32_t *ui32SWCount);

#endif
