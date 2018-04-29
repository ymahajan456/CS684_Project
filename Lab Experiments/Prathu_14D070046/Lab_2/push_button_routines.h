/*
 * push_button_routines.h
 *
 *  Created on: 23-Jan-2018
 *      Author: praton
 */

#ifndef PUSH_BUTTON_ROUTINES_H_
#define PUSH_BUTTON_ROUTINES_H_

//ISRs

void Pushbutton_down(void){

    if (GPIOIntStatus(GPIO_PORTF_BASE, false) & GPIO_PIN_0)
    {
        printf("SW2 Down first check");
        TimerEnable(TIMER0_BASE, TIMER_A);
        sw2_press = 1;
    }

    if (GPIOIntStatus(GPIO_PORTF_BASE, false) & GPIO_PIN_4)
    {
        printf("SW1 Down first check");
        TimerEnable(TIMER0_BASE, TIMER_A);
        sw1_press = 1;
    }


}

void Pushbutton_up(void){

    if (GPIOIntStatus(GPIO_PORTF_BASE, false) & GPIO_PIN_0)
    {
        printf("SW2 Up\n");
        sw2_press = 0;
        GPIOIntRegister(GPIO_PORTF_BASE, Pushbutton_down);                      // Register our handler function for port F
        GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_FALLING_EDGE);         // Configure PF4 for falling edge trigger
        GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0);                              // Clear interrupt flag
    }

    if (GPIOIntStatus(GPIO_PORTF_BASE, false) & GPIO_PIN_4)
    {
        printf("SW1 Up\n");
        sw1_press = 0;
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1, 0);
        GPIOIntRegister(GPIO_PORTF_BASE, Pushbutton_down);                      // Register our handler function for port F
        GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_FALLING_EDGE);         // Configure PF4 for falling edge trigger
        GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_4);                              // Clear interrupt flag
    }
}



#endif /* PUSH_BUTTON_ROUTINES_H_ */
