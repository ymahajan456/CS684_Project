/*
 * Author: Meet Udeshi
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/gpio.h"

#define RED GPIO_PIN_1
#define BLUE GPIO_PIN_2
#define GREEN GPIO_PIN_3

#define SW1 GPIO_PIN_4
#define SW2 GPIO_PIN_0

#define SW_STATE_IDLE 1
#define SW_STATE_PRESS 2
#define SW_STATE_RELEASE 3

#define SW_PRESSED 1
#define SW_RELEASED 2

uint8_t currLEDColor = RED;
uint8_t sw1State = SW_STATE_IDLE;
uint8_t sw2State = SW_STATE_IDLE;

uint32_t sw2Count = 0;

/*
 * Maps next LED color to current color
 *
 * currState: Current LED color
 */
uint8_t nextLEDState(uint8_t currState) {
    switch(currState) {
        case RED: return GREEN;
        case BLUE: return RED;
        case GREEN: return BLUE;
        default: return RED;
    }
}

/*
 * Detect key pressed or released using state machine
 *
 * currState: pointer to current state variable
 * swVal: Value of switch which was read
 *
 * Return: SW_PRESSED if switch pressed, SW_RELEASED if released else 0
 */
uint8_t detectKeyPress(uint8_t* currState, uint8_t swVal) {
    uint8_t nextState = SW_STATE_IDLE;
    uint8_t ret = 0;

    if(swVal) { // released
        nextState = SW_STATE_IDLE;
        if(*currState == SW_STATE_RELEASE) {
            ret = SW_RELEASED;
        }
    } else { // pressed
        switch(*currState) {
            case SW_STATE_IDLE:
                nextState = SW_STATE_PRESS;
                break;
            case SW_STATE_PRESS:
                nextState = SW_STATE_RELEASE;
                ret = SW_PRESSED;
                break;
            case SW_STATE_RELEASE:
                nextState = SW_STATE_RELEASE;
                break;
        }
    }
    *currState = nextState;
    return ret;
}

void Timer0IntHandler() {
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    uint8_t switchValue = GPIOPinRead(GPIO_PORTF_BASE, SW1 | SW2);

    uint8_t sw1Trans = detectKeyPress(&sw1State, switchValue & SW1);
    uint8_t sw2Trans = detectKeyPress(&sw2State, switchValue & SW2);

    if(sw1Trans == SW_PRESSED) {
        GPIOPinWrite(GPIO_PORTF_BASE, RED|BLUE|GREEN, currLEDColor);
        currLEDColor = nextLEDState(currLEDColor);
    } else if(sw1Trans == SW_RELEASED) {
        GPIOPinWrite(GPIO_PORTF_BASE, RED|BLUE|GREEN, 0);
    }

    if(sw2Trans == SW_PRESSED) {
        sw2Count ++;
    }
}

/*
 * Configure all required peripherals
 */
void configPeripherals() {
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

    // Enable PORTF clock
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    HWREG(GPIO_PORTF_BASE+GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE+GPIO_O_CR) |= SW2;

    // LED output, switches input
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, RED|BLUE|GREEN);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, SW1|SW2);
    GPIOPadConfigSet(GPIO_PORTF_BASE, SW1 | SW2,
                GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // Enable timer clock
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

    // Set-up timer interrupt
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    IntMasterEnable();

}

/*
 * Sets timer load to trigger interrupt at given frequency
 *
 * freq: Frequency in Hz
 */
void setTimerFreq(uint32_t freq) {
    uint32_t period = SysCtlClockGet() / freq;
    TimerLoadSet(TIMER0_BASE, TIMER_A, period - 1);
}

int main(void) {

    configPeripherals();
    setTimerFreq(100); // Hz

    TimerEnable(TIMER0_BASE, TIMER_A);

    while(1) {
        // chill out
    }
}
