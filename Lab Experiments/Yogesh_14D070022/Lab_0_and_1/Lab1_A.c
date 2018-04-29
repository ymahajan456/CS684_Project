/*
// File name:      "Lab1_A.c"
// Description:    Turn on the LED when switch is pressed and cycle through
                    LED colors

// Written By:     Yogesh Mahajan (y.mahajan@gmail.com) [14D070022, IIT Bombay]

// Dependencies:   "basic_includes.h"
                   "Lab1_A.h"

// Functions        taskSetup():    Configure SW1 interrupt
                    danceWithLEDs() Change LED color on every switch press and 
                                    keeps LED on while switch is pressed
*/
////////////////////////////////////////////////////////////////////////////////
#include "basic_include.h"
#include "Lab1_A.h"

int32_t ui32pinStatus;

void taskSetup()
{
    // set the pad configuration for SW1
    MAP_GPIOPadConfigSet(IOBASE, SW1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    // disable SW1 interrupt
    MAP_GPIOIntDisable(IOBASE, SW1);
    // Register interrupt function
    GPIOIntRegister(IOBASE, danceWithLEDs);
    // Set interrups on both edges
    MAP_GPIOIntTypeSet(IOBASE, SW1, GPIO_BOTH_EDGES);
    // clear SW1 interrupt
    MAP_GPIOIntClear(IOBASE, SW1);
    // enable SW1 interrupt
    MAP_GPIOIntEnable(IOBASE, SW1);

    // Enable Master interrupt
    MAP_IntMasterEnable();
}

void danceWithLEDs()
{
    // de-bouncing
    ui32pinStatus = GPIOPinRead(IOBASE, SW1);
    // 50 MHz clock => 10 ms = 500000 cycles
    SysCtlDelay(500000);
    if(ui32pinStatus ^  GPIOPinRead(IOBASE, SW1))
    {
        // clear the interrupt
        GPIOIntClear(IOBASE, SW1);
        return;
    }
    
    // Pull down switch => unpressed when '1'
    if(ui32pinStatus)
    {
        // Turn off all LEDs
        GPIOPinWrite(IOBASE, RED|BLUE|GREEN, 0x00);
    }
    else
    {
        // Turn on LED
        GPIOPinWrite(IOBASE, RED|BLUE|GREEN, g_ui32LEDcolor);
        // Update LED color
        if(g_ui32LEDcolor == 8)
            g_ui32LEDcolor = 2;
        else
            g_ui32LEDcolor = g_ui32LEDcolor << 1;
    }
    // clear the interrupt
    GPIOIntClear(IOBASE, SW1);
}
