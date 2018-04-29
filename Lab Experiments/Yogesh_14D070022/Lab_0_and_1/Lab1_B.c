/*
// File name:      "Lab1_B.c"
// Description:    Increment global variable every time switch is pressed using
                    interrupt

// Written By:     Yogesh Mahajan (y.mahajan@gmail.com) [14D070022, IIT Bombay]

// Dependencies:   "basic_includes.h"
                   "Lab1_B.h"

// Functions        taskSetup():    Configure SW2 interrupt
                    incCount:       Software de-bouncing and increment counter
*/
////////////////////////////////////////////////////////////////////////////////
#include "basic_include.h"
#include "Lab1_B.h"

void taskSetup()
{
    // set the pad configuration for SW2
    MAP_GPIOPadConfigSet(IOBASE, SW2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    // disable SW1 interrupt
    MAP_GPIOIntDisable(IOBASE, SW2);
    // Register interrupt function
    GPIOIntRegister(IOBASE, incCount);
    // Set interrups on both edges
    MAP_GPIOIntTypeSet(IOBASE, SW2, GPIO_FALLING_EDGE);
    // clear SW1 interrupt
    MAP_GPIOIntClear(IOBASE, SW2);
    // enable SW1 interrupt
    MAP_GPIOIntEnable(IOBASE, SW2);

    // Enable Master interrupt
    MAP_IntMasterEnable();
}

void incCount()
{
    // Software de-bouncing
    
    // 50 MHz clock => 10 ms = 500000 cycles
    SysCtlDelay(500000);

    // If pin is still pulled down then increment counter
    if(GPIOPinRead(IOBASE, SW2) ^ SW1)
        sw2Status++;

    // clear the interrupt
    GPIOIntClear(IOBASE, SW2);
}
