/*
 * my_functions.c
 *
 *  Created on: 23-Jan-2018
 *      Author: praton
 */

#include "headers_lab_4.h"
#include "my_functions_lab_4.h"
#include "config_header_lab_4.h"

void led_changer(void)
{

    if(led_value==8)
    {
        led_value = 2;
    }
    else
    {
        led_value *= 2;
    }
}

void detect_key_press(void)
{

    uint32_t pinVal=0;  // variable to hold the pinRead
    pinVal= GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4|GPIO_PIN_0);    // read F4 and F0
    /* State Encoding
     *
     * idle = 0;
     * press = 1;
     * release = 2;
     *
     * */
    switch(sw1_state)
    {
    case 0:
        if((pinVal & GPIO_PIN_4)==0)
        {
            sw1_state = 1;
            break;
        }
        else
        {
            sw1_state = 0;
            break;
        }
    case 1:
        if((pinVal & GPIO_PIN_4)==0)
        {
            sw1_state = 2;
            sw1Status++;
            flag_sw1 = 1;
//            led_changer();
//            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1, led_value);
            if(step>step_max)
            {
                step = step_max;
            }
            else
            {
                step++;
            }
            break;
        }
        else
        {
            sw1_state = 0;
            break;
        }
    case 2:
        if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4))    // read F4 and F0
        {
            sw1_state = 0;
            flag_sw1 = 0;
            break;
        }
        else
        {
            sw1_state = 2;
            break;
        }
    }


    switch(sw2_state)
        {
        case 0:
            if((pinVal & GPIO_PIN_0)==0)
            {
                sw2_state = 1;
                break;
            }
            else
            {
                sw2_state = 0;
                break;
            }
        case 1:
            if((pinVal & GPIO_PIN_0)==0)
            {
                sw2_state = 2;
                flag_sw2 = 1;
                sw2Status++;
//                led_changer();
//                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1, led_value);

                if(step<=step_min)
                {
                    step = step_min;
                }
                else
                {
                    step = step - 1;
                }
                break;
            }
            else
            {
                sw2_state = 0;
                break;
            }
        case 2:
            if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0))    // read F4 and F0
            {
                sw2_state = 0;
                flag_sw2 = 0;
                break;
            }
            else
            {
                sw2_state = 2;
                break;
            }
        }
}

