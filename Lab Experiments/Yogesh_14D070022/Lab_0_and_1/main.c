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
#include "Lab1_A.h"

void sysSetup()
{
    // using ROM function to save flash memory

    // set clock at 50MHz using external oscillator [16 MHz]
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    // Enable Port F
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    #ifdef USE_TIMER1
    // Clock source s ssytem timer only -> 50MHz
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    #endif

    // unlock NMI on PF0 i.e. SW2
    HWREG(IOBASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(IOBASE + GPIO_O_CR) |= 0x01;
    HWREG(IOBASE + GPIO_O_LOCK) = 0;


    // set pins 1, 2 and 3 at PORT F as output
    MAP_GPIOPinTypeGPIOOutput(IOBASE, RED|BLUE|GREEN);
    // set pins 0 and 4 as input
    MAP_GPIOPinTypeGPIOInput(IOBASE, SW1|SW2); 

    // write 0 on output pins
    GPIOPinWrite(IOBASE, RED|BLUE|GREEN, 0x00);
}


int main(void)

{
    sysSetup();
    taskSetup();

    while(1);
}
