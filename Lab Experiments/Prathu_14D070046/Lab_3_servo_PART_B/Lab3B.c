#include "Lab3B.h"


uint32_t ui32ServoValue;

int32_t tatti = 0;

void taskSetup()
{
    g_ui32DebounceTime = MAP_SysCtlClockGet()/100;


    initServo();
    initSwitches();

    ui32ServoValue = g_ui32ServoRest;

    #ifdef USE_TIMER0
    // Clock source s ssytem timer only -> 50MHz
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    #endif

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
    tatti ++;
    g_ui32ReadValue = GPIOPinRead(IOBASE, SW1|SW2);
    switchManager(&g_ui32ReadValue, SW1, &g_ui32SW1FSMState, &g_ui32SW1PressTime, &g_ui32SW1PressCount, &g_ui32SW1LongPress);
    switchManager(&g_ui32ReadValue, SW2, &g_ui32SW2FSMState, &g_ui32SW2PressTime, &g_ui32SW2PressCount, &g_ui32SW2LongPress);
    switchAction();
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
}

void ToDO()
{

}

void switchAction()
{
    if(g_ui32SW1PressTime == 1)
    {
        ui32ServoValue -= g_ui32ServoStep;
        ui32ServoValue = (ui32ServoValue < g_ui32ServoMin) ? g_ui32ServoMin : ui32ServoValue;
        PWMPulseWidthSet(SERVO_PWM_BASE, SERVO_PIN, ui32ServoValue);
    }
    if(g_ui32SW2PressTime == 1)
    {
        ui32ServoValue += g_ui32ServoStep;
        ui32ServoValue = (ui32ServoValue > g_ui32ServoMax) ? g_ui32ServoMax : ui32ServoValue;
        PWMPulseWidthSet(SERVO_PWM_BASE, SERVO_PIN, ui32ServoValue);
    }
}




