/*
// File name:      "Lab1_C.c"
// Description:    Change toggle delay and color of LED using switches

// Written By:     Yogesh Mahajan (y.mahajan@gmail.com) [14D070022, IIT Bombay]

// Dependencies:   "basic_includes.h"
                   "Lab1_C.h"

// Functions        taskSetup():        Configure interrupts and timer
                    toggleLED():        Timer ISR. Toggle LED on overflow
                    changeLEDConfig():  Change LED toggle period and color
*/
////////////////////////////////////////////////////////////////////////////////
#include "basic_include.h"
#include "Lab1_C.h"

int32_t ui32LEDStatus = 0;

void taskSetup()
{
    // set the pad configuration for SW2
    MAP_GPIOPadConfigSet(IOBASE, SW1|SW2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    // disable SW1 interrupt
    MAP_GPIOIntDisable(IOBASE, SW1|SW2);
    // Register interrupt function
    GPIOIntRegister(IOBASE, changeLEDConfig);
    // Set interrups on falling edges
    MAP_GPIOIntTypeSet(IOBASE, SW1|SW2, GPIO_FALLING_EDGE);
    // clear SW1 interrupt
    MAP_GPIOIntClear(IOBASE, SW1|SW2);
    // enable SW1 interrupt
    MAP_GPIOIntEnable(IOBASE, SW1|SW2);

    // Periodic mode for Timer 1A
    MAP_TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
    // Set load Value
    g_ui32ToggleTime = g_ui32ToggleConstant * g_ui32ToggleScale;
    MAP_TimerLoadSet(TIMER1_BASE, TIMER_A, g_ui32ToggleConstant);
    // register timer interrupt finction
    TimerIntRegister(TIMER1_BASE, TIMER_A, toggleLED);
    // Enable timer interrupt
    MAP_IntEnable(INT_TIMER1A);
    // Set interrupt condition
    MAP_TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

    // Enable Master interrupt
    MAP_IntMasterEnable();

    // Start Timer1A
    MAP_TimerEnable(TIMER1_BASE, TIMER_A);
}

extern void toggleLED()
{
    // Clear Timer A Timeout interrupt
    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    // change LED state
    ui32LEDStatus = ui32LEDStatus ^ 0xFFFF;
    GPIOPinWrite(IOBASE, RED|BLUE|GREEN, g_ui32LEDColor & ui32LEDStatus);
}

extern void changeLEDConfig()
{
    // Software de-bouncing
    
    // 50 MHz clock => 10 ms = 500000 cycles
    SysCtlDelay(500000);

    // Change Timer value
    if(GPIOPinRead(IOBASE, SW1) ^ SW1)
    {
        // Disable master interrupt as we have to change Timer's period
        IntMasterDisable();
        // Disable timer
        TimerDisable(TIMER1_BASE, TIMER_A);
        // Update timer register
        TimerLoadSet(TIMER1_BASE, TIMER_A, g_ui32ToggleTime);
        // Enable timer
        TimerEnable(TIMER1_BASE, TIMER_A);
        // Enable master interrupts
        IntMasterEnable();

        // update toggle time
        if(g_ui32ToggleScale == 3)
            g_ui32ToggleScale = 1;
        else
            g_ui32ToggleScale++;

        g_ui32ToggleTime = g_ui32ToggleScale * g_ui32ToggleConstant;
    }
    // clear the interrupt
    GPIOIntClear(IOBASE, SW1);

    // Toggle LED
    if(GPIOPinRead(IOBASE, SW2) ^ SW2)
    {
        if(g_ui32LEDColor == 8)
            g_ui32LEDColor = 2;
        else
            g_ui32LEDColor = g_ui32LEDColor << 1;
    }
    // clear the interrupt
    GPIOIntClear(IOBASE, SW2);
}
