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

#define ST_RED_INC 1
#define ST_RED_DEC 2
#define ST_BLUE_INC 3
#define ST_BLUE_DEC 4
#define ST_GREEN_INC 5
#define ST_GREEN_DEC 6

#define MODE_AUTO 1
#define MODE_MANUAL 2

#define MANUAL_SELECT 4
#define MANUAL_RED 8
#define MANUAL_BLUE 16
#define MANUAL_GREEN 32

#define SW_STATE_IDLE 1
#define SW_STATE_PRESS 2
#define SW_STATE_RELEASE 4

#define SW_PRESSED 1
#define SW_RELEASED 2

uint8_t sw1State = SW_STATE_IDLE;
uint8_t sw2State = SW_STATE_IDLE;
uint8_t sw1Trans = 0;
uint8_t sw2Trans = 0;

uint8_t mode = MODE_AUTO;

uint32_t pwmClock;
uint32_t pwmLoad;
uint8_t pwmDutyRed = 255;
uint8_t pwmDutyBlue = 0;
uint8_t pwmDutyGreen = 0;

uint8_t autoModeState = ST_GREEN_INC;
uint32_t autoModeRate = 13400;

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

    // LEDs as PWM output
    GPIOPinTypePWM(GPIO_PORTF_BASE, RED|BLUE|GREEN);
    GPIOPinConfigure(GPIO_PF1_M1PWM5);
    GPIOPinConfigure(GPIO_PF2_M1PWM6);
    GPIOPinConfigure(GPIO_PF3_M1PWM7);

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
    PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, pwmLoad);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, pwmLoad);

    PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT | PWM_OUT_6_BIT | PWM_OUT_7_BIT, true);

}

/*
 * Sets LED brightness by adjusting PWM duty cycle
 *
 * LED: LED pin to set brightness on
 * bright: brightness value 0-255
 */
void setLEDBrightness(uint32_t LED, uint8_t bright) {
    if(bright < 10) bright = 10;
    if(bright > 250) bright = 250;

    PWMPulseWidthSet(PWM1_BASE, LED, bright * pwmLoad / 1000);
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
    uint32_t sw1Time = 0;
    uint8_t sw1Clicks = 0;

    configPeripherals();
    configPWMLoad();
    setTimerFreq(100); // Hz
    TimerEnable(TIMER0_BASE, TIMER_A);
    PWMGenEnable(PWM1_BASE, PWM_GEN_2);
    PWMGenEnable(PWM1_BASE, PWM_GEN_3);

    while(1) {
        // Each cycle set brightness which was updated in last loop
        setLEDBrightness(PWM_RED, pwmDutyRed);
        setLEDBrightness(PWM_BLUE, pwmDutyBlue);
        setLEDBrightness(PWM_GREEN, pwmDutyGreen);

        if(mode & MODE_AUTO) { // Auto mode

            // Keep cycling between 6 states to increment and decrement brightness
            switch (autoModeState) {
            case ST_GREEN_INC:
                pwmDutyGreen ++;
                if(pwmDutyGreen == 255) autoModeState = ST_RED_DEC;
                break;
            case ST_RED_DEC:
                pwmDutyRed --;
                if(pwmDutyRed == 0) autoModeState = ST_BLUE_INC;
                break;
            case ST_BLUE_INC:
                pwmDutyBlue ++;
                if(pwmDutyBlue == 255) autoModeState = ST_GREEN_DEC;
                break;
            case ST_GREEN_DEC:
                pwmDutyGreen --;
                if(pwmDutyGreen == 0) autoModeState = ST_RED_INC;
                break;
            case ST_RED_INC:
                pwmDutyRed ++;
                if(pwmDutyRed == 255) autoModeState = ST_BLUE_DEC;
                break;
            case ST_BLUE_DEC:
                pwmDutyBlue --;
                if(pwmDutyBlue == 0) autoModeState = ST_GREEN_INC;
                break;
            }

            // Increase/decrease rate i.e. speed of circle movement on switch press
            if(sw1State & SW_STATE_RELEASE) {autoModeRate -= 10;}
            if(sw2State & SW_STATE_RELEASE) {autoModeRate += 10;}
            if(autoModeRate < 3000) autoModeRate = 3000;

            // If both switch pressed go to manual mode
            if((sw1State & SW_STATE_RELEASE) && (sw2State & SW_STATE_RELEASE)) {
                mode = MODE_MANUAL | MANUAL_SELECT;

                pwmDutyRed = 125;
                pwmDutyBlue = 125;
                pwmDutyGreen = 125;
            }

            SysCtlDelay(autoModeRate);

        } else if (mode & MODE_MANUAL) {

            // SELECT state: based on click patterns, it will go to red blue or green state
            if (mode & MANUAL_SELECT) {
                if(sw1State & SW_STATE_RELEASE) {sw1Time ++;}

                if((sw2State & SW_STATE_RELEASE) && (sw1Trans & SW_RELEASED)) {
                    sw1Clicks ++;
                    sw1Trans = 0;
                } else if (sw2Trans & SW_RELEASED) {
                    if(sw1Clicks == 2) {
                        mode = MODE_MANUAL | MANUAL_BLUE;
                    } else if(sw1Time > 100) {
                        mode = MODE_MANUAL | MANUAL_GREEN;
                    } else {
                        mode = MODE_MANUAL | MANUAL_RED;
                    }

                    sw1Time = 0;
                    sw1Clicks = 0;
                }
            }
            /*
             * In each color state, single switch press changes brightness
             * and both switch press goes to state SELECT
             */
            else if (mode & MANUAL_RED) {
                if ((sw2State & SW_STATE_IDLE) && (sw1Trans & SW_RELEASED)) {
                    pwmDutyRed -= 10;
                    sw1Trans = 0;
                } else if ((sw1State & SW_STATE_IDLE) && (sw2Trans & SW_RELEASED)) {
                    pwmDutyRed += 10;
                    sw2Trans = 0;
                } else if((sw1State & SW_STATE_RELEASE) && (sw2State & SW_STATE_RELEASE)) {
                    mode = MODE_MANUAL | MANUAL_SELECT;
                }
            } else if (mode & MANUAL_BLUE) {
                if ((sw2State & SW_STATE_IDLE) && (sw1Trans & SW_RELEASED)) {
                    pwmDutyBlue -= 10;
                    sw1Trans = 0;
                } else if ((sw1State & SW_STATE_IDLE) && (sw2Trans & SW_RELEASED)) {
                    pwmDutyBlue += 10;
                    sw2Trans = 0;
                } else if((sw1State & SW_STATE_RELEASE) && (sw2State & SW_STATE_RELEASE)) {
                    mode = MODE_MANUAL | MANUAL_SELECT;
                }
            } else if (mode & MANUAL_GREEN) {
                if ((sw2State & SW_STATE_IDLE) && (sw1Trans & SW_RELEASED)) {
                    pwmDutyGreen -= 10;
                    sw1Trans = 0;
                } else if ((sw1State & SW_STATE_IDLE) && (sw2Trans & SW_RELEASED)) {
                    pwmDutyGreen += 10;
                    sw2Trans = 0;
                } else if((sw1State & SW_STATE_RELEASE) && (sw2State & SW_STATE_RELEASE)) {
                    mode = MODE_MANUAL | MANUAL_SELECT;
                }
            }

            SysCtlDelay(134000); // 10 ms
        }
    }
}
