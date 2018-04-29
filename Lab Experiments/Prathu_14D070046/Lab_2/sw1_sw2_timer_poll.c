/*
 * lab-1.c
 *
 *  Created on: 07-Jan-2018
 *      Author: praton
 */

#define DEFINE_VARIABLES
#include "headers_lab_2.h"
#include "config_header_lab_2.h"
#include "my_functions.h"

void Timer0IntHandler(void)
{
    // Clear the timer interrupt
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    detect_key_press();
//    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1, led_value);
//    led_value++;
//    led_value = led_value%8;

}

int main(void){

    config_lab_2();

    while(1);
}
