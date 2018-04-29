/*
 * lab-1.c
 *
 *  Created on: 07-Jan-2018
 *      Author: praton
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "inc/hw_ints.h"
#include <time.h>
#include "inc/hw_gpio.h"

uint8_t ui8LED_sw1 = 2;

uint8_t ui8LED_sw2 = 2;

uint8_t sw2Status = 0;

uint8_t sw1Status = 0;

void Pushbutton_up(void);
void Pushbutton_down(void);

void setup(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
}

void led_pin_config(void)
{
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
}

void switch_config(void)
{
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_0|GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU); // enable F0's pullup, the drive strength won't affect the input
//    GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
}

void unlocking_pins(void)
{
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
//    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
}

void Pushbutton_down(void){

    if (GPIOIntStatus(GPIO_PORTF_BASE, false) & GPIO_PIN_0)
    {
            // PF4 was interrupt cause
            printf("SW2 Down\n");

            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1, ui8LED_sw1);

            sw2Status++;

            if(ui8LED_sw1==8)
                {
                   ui8LED_sw1=2;
                }
            else
                {
                   ui8LED_sw1 = 2*ui8LED_sw1;
                }
            GPIOIntRegister(GPIO_PORTF_BASE, Pushbutton_up);                        // Register our handler function for port F
            GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0,GPIO_RISING_EDGE);           // Configure PF4 for rising edge trigger
            GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0);                              // Clear interrupt flag
        }

    if (GPIOIntStatus(GPIO_PORTF_BASE, false) & GPIO_PIN_4)
    {
            printf("SW1 Down\n");
            sw1Status++;
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1, ui8LED_sw2);
            GPIOIntRegister(GPIO_PORTF_BASE, Pushbutton_up);                        // Register our handler function for port F
            GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4,GPIO_RISING_EDGE);           // Configure PF4 for rising edge trigger
            GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_4);                              // Clear interrupt flag
    }


}

void Pushbutton_up(void){

    if (GPIOIntStatus(GPIO_PORTF_BASE, false) & GPIO_PIN_0)
        {
            printf("SW2 Up\n");
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1, 0);
            GPIOIntRegister(GPIO_PORTF_BASE, Pushbutton_down);                      // Register our handler function for port F
            GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_FALLING_EDGE);         // Configure PF4 for falling edge trigger
            GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0);                              // Clear interrupt flag
        }

    if (GPIOIntStatus(GPIO_PORTF_BASE, false) & GPIO_PIN_4)
        {
            printf("SW1 UP");
            if(sw1Status==1)
            {
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1, ui8LED_sw2);
                SysCtlDelay(6700000);
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1, 0);
                SysCtlDelay(6700000);
            }
            else if(sw1Status==2)
            {
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1, ui8LED_sw2);
                SysCtlDelay(6700000*2);
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1, 0);
                SysCtlDelay(6700000*2);
            }
            else if(sw1Status==3)
            {
                sw1Status = 0;
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1, ui8LED_sw2);
                SysCtlDelay(6700000*4);
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1, 0);
                SysCtlDelay(6700000*4);
            }

            GPIOIntRegister(GPIO_PORTF_BASE, Pushbutton_down);                       // Register our handler function for port F
            GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4,GPIO_FALLING_EDGE);           // Configure PF4 for rising edge trigger
            GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_4);                               // Clear interrupt flag
        }

}

int main(void){

    setup();

    unlocking_pins();

    switch_config();

    led_pin_config();

    GPIOIntDisable(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4);
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4);
    GPIOIntRegister(GPIO_PORTF_BASE, Pushbutton_down);
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4, GPIO_FALLING_EDGE);
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4);

    while(1);
}
