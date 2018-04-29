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
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "driverlib/timer.h"

#define PWM_FREQUENCY 50

#define RED   GPIO_PIN_1
#define BLUE  GPIO_PIN_2
#define GREEN GPIO_PIN_3

#define PWM_RED   PWM_OUT_5
#define PWM_BLUE  PWM_OUT_6
#define PWM_GREEN PWM_OUT_7

#define SW1 GPIO_PIN_4
#define SW2 GPIO_PIN_0

#define SW_STATE_IDLE 1
#define SW_STATE_PRESS 2
#define SW_STATE_RELEASE 4

#define SW_PRESSED 1
#define SW_RELEASED 2

uint8_t sw1State = SW_STATE_IDLE;
uint8_t sw2State = SW_STATE_IDLE;
uint8_t sw1Trans = 0;
uint8_t sw2Trans = 0;

uint32_t pwmClock;
uint32_t pwmLoad;

uint8_t servoAngle = 90;

void setServoAngle(uint8_t angle);

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

    sw1Trans |= detectKeyPress(&sw1State, switchValue & SW1);
    sw2Trans |= detectKeyPress(&sw2State, switchValue & SW2);

    if(sw1Trans & SW_RELEASED) {
        servoAngle -= 10;
        sw1Trans = 0;
    }

    if(sw2Trans & SW_RELEASED) {
        servoAngle += 10;
        sw2Trans = 0;
    }
    if(servoAngle < 10) servoAngle = 10;
    if(servoAngle > 170) servoAngle = 170;

    setServoAngle(servoAngle);
}

void configPeripherals() {
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
    SysCtlPWMClockSet(SYSCTL_PWMDIV_64);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    HWREG(GPIO_PORTF_BASE+GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE+GPIO_O_CR) |= SW2;

    // SW1 and SW2 as input weak pull-up
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, SW1|SW2);
    GPIOPadConfigSet(GPIO_PORTF_BASE, SW1 | SW2,
                    GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // PF1 as PWM output
    GPIOPinTypePWM(GPIO_PORTF_BASE, RED|BLUE|GREEN);
    GPIOPinConfigure(GPIO_PF1_M1PWM5);

    // Enable timer clock
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

    // Set-up timer interrupt
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    IntMasterEnable();
}

/*
 * Enable and set PWM load and frequency
 */
void configPWMLoad() {
    pwmClock = SysCtlClockGet() / 64;
    pwmLoad = (pwmClock / PWM_FREQUENCY) - 1;

    PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, pwmLoad);

    PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, true);
}

/*
 * Sets LED brightness by adjusting PWM duty cycle
 *
 * LED: LED pin to set brightness on
 * bright: brightness value 0-255
 */
void setServoAngle(uint8_t angle) {
    if(angle < 10) angle = 10;
    if(angle > 170) angle = 170;

    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, angle * pwmLoad / 180);
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
    configPWMLoad();
    setTimerFreq(100); // Hz
    TimerEnable(TIMER0_BASE, TIMER_A);
    PWMGenEnable(PWM1_BASE, PWM_GEN_2);

    while(1) {
    }
}
