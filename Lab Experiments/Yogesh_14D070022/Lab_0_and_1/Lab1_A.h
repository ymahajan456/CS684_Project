/*
// File name:      "Lab1_A.h"
// Description:    Turn on the LED when switch is pressed and cycle through
                    LED colors

// Written By:     Yogesh Mahajan (y.mahajan@gmail.com) [14D070022, IIT Bombay]

// Dependencies:   "basic_includes.h"
*/
////////////////////////////////////////////////////////////////////////////////
#ifndef _LAB1_A_H_
#define _LAB1_A_H_

# include "basic_include.h"

#ifdef DEFINE_VARIABLES
#define EXTERN                  
#define INITIALIZER(...)        = __VA_ARGS__
#else
#define EXTERN                  extern
#define INITIALIZER(...)       
#endif

// global variable for LED color
EXTERN uint32_t g_ui32LEDcolor INITIALIZER(2);

extern void taskSetup();
extern void danceWithLEDs();

#endif
