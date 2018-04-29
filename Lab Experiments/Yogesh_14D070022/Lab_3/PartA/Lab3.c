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
#include "Lab3.h"



uint32_t decColour = 0, incColour = 0, i = 0, j=0, maxItr;

int32_t tatti = 0;

void taskSetup()
{
    g_ui32DebounceTime = MAP_SysCtlClockGet()/100;
    color[0] = 1000;
    color[1] = 5;
    color[2] = 5;
    g_ui32SpeedCap = g_ui32DebounceTime/2;

    // set PWM width to 1000
    PWMSetLeds(1000);

    #ifdef USE_TIMER0
    // Clock source s ssytem timer only -> 50MHz
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    #endif

    #ifdef USE_TIMER1
    // Clock source s ssytem timer only -> 50MHz
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    #endif

    // unlock NMI on PF0 i.e. SW2
    HWREG(IOBASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(IOBASE + GPIO_O_CR) |= 0x01;
    HWREG(IOBASE + GPIO_O_LOCK) = 0;

    // set pins 0 and 4 as input
    MAP_GPIOPinTypeGPIOInput(IOBASE, SW1|SW2); 
    
    // set the pad configuration for SW2
    MAP_GPIOPadConfigSet(IOBASE, SW1|SW2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    MAP_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    // Set load Value
    MAP_TimerLoadSet(TIMER0_BASE, TIMER_A, g_ui32DebounceTime);
    // register timer interrupt 
    TimerIntRegister(TIMER0_BASE, TIMER_A, ticker);
    // Set interrupt condition
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    // Enable Master interrupt
    MAP_IntMasterEnable();

    // Start Timer1A
    TimerEnable(TIMER0_BASE, TIMER_A);
}


void ticker()
{
    g_ui32SystemStatus = 1;
    tatti ++;
    g_ui32ReadValue = GPIOPinRead(IOBASE, SW1|SW2);
    switchManager(&g_ui32ReadValue, SW1, &g_ui32SW1FSMState, &g_ui32SW1PressTime, &g_ui32SW1PressCount, &g_ui32SW1LongPress);
    switchManager(&g_ui32ReadValue, SW2, &g_ui32SW2FSMState, &g_ui32SW2PressTime, &g_ui32SW2PressCount, &g_ui32SW2LongPress);
    modeDetect();
    switchAction();
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
}


void modeDetect()
{
    if(g_ui32SW2LongPress)
    {
        // nothing happened on SW1, no mode change, declare SW normal press

        // If sw1 is pressed once move to manual red mode
        if(g_ui32SW1PressCount == 1)
        {
            g_ui32Mode = MANUAL;
            g_ui32ManualTarget = RED_TARGET;
            g_ui32SW1PressCount = 0;
        }
        else if(g_ui32SW1PressCount == 2)
        {
            g_ui32Mode = MANUAL;
            g_ui32ManualTarget = BLUE_TARGET;
            g_ui32SW1PressCount = 0;
        }
        else if(g_ui32SW1LongPress == 1)
        {
            g_ui32Mode = MANUAL;
            g_ui32ManualTarget = GREEN_TARGET;
            g_ui32SW1LongPress = 0;
        }
        else
        {
            g_ui32SW2PressCount ++;
        }
        g_ui32SW2LongPress = 0;
    }
}

void switchAction()
{
    // if SW1 detects long and nothing on SW2, then declare SW1 normal press
    if(g_ui32SW1LongPress && !g_ui32SW2PressTime)
    {
        g_ui32SW1LongPress = 0;
        g_ui32SW1PressCount ++;
    }
    // SW1 action list
    if(g_ui32SW1PressCount && !g_ui32SW2PressTime)
    {
        // if auto mode
        if(g_ui32Mode)
        {
            g_ui32AutoSpeed += 5000;
            if(g_ui32AutoSpeed > g_ui32SpeedCap)
            {
                g_ui32AutoSpeed = g_ui32SpeedCap;
            }
        }
        else
        {
            color[g_ui32ManualTarget] += g_ui32ManualSpeed;
            if(color[g_ui32ManualTarget] > 1000)
            {
                color[g_ui32ManualTarget] = 1000;
            }
        }
        g_ui32SW1PressCount = 0;
        SetLedColor(color);
    }

    // SW2 action list
    if(g_ui32SW2PressCount)
    {
        // if auto mode
        if(g_ui32Mode)
        {
            g_ui32AutoSpeed -= 5000;
            g_ui32AutoSpeed = g_ui32AutoSpeed < 5000 ? 5000 : g_ui32AutoSpeed;

        }
        else
        {
            color[g_ui32ManualTarget] -= g_ui32ManualSpeed;
            if(color[g_ui32ManualTarget] < 1)
            {
                color[g_ui32ManualTarget] = 1;
            }
        }
        g_ui32SW2PressCount = 0;
        SetLedColor(color);
    }
}

void LEDCycle()
{
    for (decColour = 0; decColour < 3; decColour += 1)
    {
        incColour = decColour == 2 ? 0 : decColour + 1;

        // cross-fade the two colours.
        for(i = 5; i < 1000; i += 5)
        {
            color[decColour] -= 5;
            color[incColour] += 5;
            SetLedColor(color);
            SysCtlDelay(g_ui32AutoSpeed);
        }
    }

}

void ToDO()
{
    if(g_ui32Mode)
    {
        LEDCycle();
    }
}


