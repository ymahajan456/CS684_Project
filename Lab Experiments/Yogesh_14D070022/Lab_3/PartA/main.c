/*
// File name:      "main.c"
// Description:    Main file for LAB 1 [CS 684, IIT Bombay, 2018]

// Written By:     Yogesh Mahajan (y.mahajan@gmail.com) [14D070022, IIT Bombay]

// Dependencies:   "basic_includes.h"
                   "Lab1_A.h, Lab_B.h, Lab_C,h"

// Functions        sysSetup():     Setup system. Initialize peripherals
                    main():         main function
*/
////////////////////////////////////////////////////////////////////////////////
#define DEFINE_VARIABLES
#include "basic_include.h"
#include "Lab3.h"


void sysSetup()
{
    // using ROM function to save flash memory
    // set clock at 50MHz using external oscillator [16 MHz]
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
}


int main(void)

{
    sysSetup();
    taskSetup();

    while(1)
    {
        ToDO();
    }
}
