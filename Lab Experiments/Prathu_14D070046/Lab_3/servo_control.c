/*
 * servo_control.c
 *
 *  Created on: 24-Jan-2018
 *      Author: praton
 */

#define DEFINE_VARIABLES
#include "headers_lab_3_servo.h"
#include "config_header_lab_3_servo.h"
#include "my_functions_servo.h"

#define PWM_FREQUENCY 55

void Timer0IntHandler(void)
{
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    detect_key_press();
}

int main(void)
{
    ui8Adjust = 105;
    ui32PWMClock = SysCtlClockGet() / 64;
    ui32Load = (ui32PWMClock / PWM_FREQUENCY) - 1;

    config_lab_3();

    while(1);
}

