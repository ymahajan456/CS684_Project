/*
 * lab-1.c
 *
 *  Created on: 07-Jan-2018
 *      Author: praton
 */

#include "headers_lab_2.h"
#include "config_header_lab_2.h"


uint8_t ui8LED_sw1 = 2;
uint8_t sw2Status = 0;
uint8_t sw1Status = 0;
uint8_t sw1_press = 0;
uint8_t sw2_press = 0;

void Pushbutton_up(void);
void Pushbutton_down(void);

void Timer0IntHandler(void)
{

    printf(" Timer interrupt handler");
    // Clear the timer interrupt
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    if (GPIOIntStatus(GPIO_PORTF_BASE, false) & GPIO_PIN_4 & sw1_press)
    {
        printf("SW1 Down second check \n");
        sw1Status++;
        if(ui8LED_sw1==8)
        {
            ui8LED_sw1 = 2;
        }
        else
        {
            ui8LED_sw1 *= 2;
        }
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1, ui8LED_sw1);
        GPIOIntRegister(GPIO_PORTF_BASE, Pushbutton_up);                        // Register our handler function for port F
        GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4,GPIO_RISING_EDGE);           // Configure PF4 for rising edge trigger
        GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_4);                              // Clear interrupt flag
    }

    if (GPIOIntStatus(GPIO_PORTF_BASE, false) & GPIO_PIN_0 & sw2_press)
    {
        // PF4 was interrupt cause
        printf("SW2 Down second check \n");
        sw2Status++;
        GPIOIntRegister(GPIO_PORTF_BASE, Pushbutton_up);                        // Register our handler function for port F
        GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0,GPIO_RISING_EDGE);           // Configure PF4 for rising edge trigger
        GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0);                              // Clear interrupt flag
    }

}

int main(void){

    config_lab_2();

    GPIOIntDisable(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4);
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4);
    GPIOIntRegister(GPIO_PORTF_BASE, Pushbutton_down);
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4, GPIO_FALLING_EDGE);
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4);

    while(1);
}
