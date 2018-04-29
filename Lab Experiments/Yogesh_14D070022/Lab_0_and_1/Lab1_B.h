/*
// File name:      "Lab1_B.h"
// Description:    Increment counter with wsitch

// Written By:     Yogesh Mahajan (y.mahajan@gmail.com) [14D070022, IIT Bombay]

// Dependencies:   "basic_includes.h"
*/
////////////////////////////////////////////////////////////////////////////////
#ifndef _LAB1_B_H_
#define _LAB1_B_H_

# include "basic_include.h"

#ifdef DEFINE_VARIABLES
#define EXTERN                  
#define INITIALIZER(...)        = __VA_ARGS__
#else
#define EXTERN                  extern
#define INITIALIZER(...)       
#endif

EXTERN int32_t sw2Status INITIALIZER(0);

extern void taskSetup();
extern void incCount();

#endif

