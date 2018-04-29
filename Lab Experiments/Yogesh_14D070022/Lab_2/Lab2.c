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
#include "Lab2.h"

int32_t LEDcolor = 0;
int32_t ui32LEDStatus = 0;
int32_t tatti = 0;


void taskSetup()
{
    g_ui32DebounceTime = MAP_SysCtlClockGet()/100;
    g_ui32ToggleConstant = MAP_SysCtlClockGet()/2;
    
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    #ifdef USE_TIMER1
    // Clock source s ssytem timer only -> 50MHz
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    #endif

    #ifdef USE_TIMER0
    // Clock source s ssytem timer only -> 50MHz
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
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

    // set the pad configuration for SW2
    MAP_GPIOPadConfigSet(IOBASE, SW1|SW2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // Periodic mode for Timer 1A
    MAP_TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
    // Set load Value
    g_ui32ToggleTime = g_ui32ToggleConstant * g_ui32ToggleScale;
    MAP_TimerLoadSet(TIMER1_BASE, TIMER_A, g_ui32ToggleConstant);
    // register timer interrupt 
    TimerIntRegister(TIMER1_BASE, TIMER_A, toggleLED);

    MAP_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    // Set load Value
    MAP_TimerLoadSet(TIMER0_BASE, TIMER_A, g_ui32DebounceTime);
    // register timer interrupt 
    TimerIntRegister(TIMER0_BASE, TIMER_A, ticker);
    // Set interrupt condition
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    // Enable Master interrupt
    MAP_IntMasterEnable();

    // Start Timer1A
    TimerEnable(TIMER0_BASE, TIMER_A);
    TimerEnable(TIMER1_BASE, TIMER_A);
}

void toggleLED()
{
    // Clear Timer A Timeout interrupt
    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    // // change LED state
            // GPIOPinWrite(IOBASE, RED|BLUE|GREEN, LEDcolor++);
    ui32LEDStatus = ui32LEDStatus ^ 0b01110;
    GPIOPinWrite(IOBASE, RED|BLUE|GREEN, g_ui32LEDColor & ui32LEDStatus);
}


void ticker()
{
    g_ui32SystemStatus = 1;
    g_ui32ReadValue = GPIOPinRead(IOBASE, SW1|SW2);
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
}



void ToDO()
{
    if(g_ui32SystemStatus)
    {
        g_ui32SystemStatus = 0;
        switchManager(&g_ui32ReadValue, SW1, &g_ui32SW1FSMState, &g_ui32SW1PressTime);
        switchManager(&g_ui32ReadValue, SW2, &g_ui32SW2FSMState, &g_ui32SW2PressTime);
        changeLEDConfig();
    }
}

void changeLEDConfig()
{
    if(g_ui32SW1PressTime == 1)
    {
        TimerDisable(TIMER1_BASE, TIMER_A);
        // Update timer register
        TimerLoadSet(TIMER1_BASE, TIMER_A, g_ui32ToggleTime);
        // Enable timer
        tatti = TimerLoadGet(TIMER1_BASE, TIMER_A);
        TimerEnable(TIMER1_BASE, TIMER_A);
        
        // update toggle time
        if(g_ui32ToggleScale == 3)
            g_ui32ToggleScale = 1;
        else
            g_ui32ToggleScale++;

        g_ui32ToggleTime = g_ui32ToggleScale * g_ui32ToggleConstant;
    }

    // Toggle LED
    if(g_ui32SW2PressTime == 1)
    {
        if(g_ui32LEDColor == 8)
            g_ui32LEDColor = 2;
        else
            g_ui32LEDColor = g_ui32LEDColor << 1;
    }
}
