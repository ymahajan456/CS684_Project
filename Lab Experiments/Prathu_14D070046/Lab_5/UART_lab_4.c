/*
 * rgb_controller.c
 *
 *  Created on: 24-Jan-2018
 *      Author: praton
 */

#define DEFINE_VARIABLES

#include <headers_lab_4.h>
#include <config_header_lab_4.h>
#include <my_functions_lab_4.h>

uint8_t color = 0;
unsigned long red_intensity = 320;
unsigned long green_intensity = 10;
unsigned long blue_intensity = 10;
unsigned long intensity_min = 10;
unsigned long intensity_max = 320;
//unsigned long step = 10;

void delay_ms(int ms)
{
    SysCtlDelay( (SysCtlClockGet()/(3*1000))*ms ) ;
}

void system_setup(void)
{
    //Set the clock
   SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_OSC |   SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

   //Configure PWM Clock to match system
   SysCtlPWMClockSet(SYSCTL_PWMDIV_4);

   // Enable the peripherals used by this program.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);  //The Tiva Launchpad has two modules (0 and 1). Module 1 covers the LED pins
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
}

void gpio_config(void)
{
    GPIOPinConfigure(GPIO_PF1_M1PWM5);
    GPIOPinConfigure(GPIO_PF2_M1PWM6);
    GPIOPinConfigure(GPIO_PF3_M1PWM7);
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
}

void pwm_config(void)
{

    //Configure PWM Options
    //PWM_GEN_2 Covers M1PWM4 and M1PWM5
    //PWM_GEN_3 Covers M1PWM6 and M1PWM7 See page 207 4/11/13 DriverLib doc
    PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
    PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

    //Set the Period (expressed in clock ticks)
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, 320);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, 320);

    //Set PWM duty-50% (Period /2)
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5,100);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,100);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,100);

    // Enable the PWM generator
    PWMGenEnable(PWM1_BASE, PWM_GEN_2);
    PWMGenEnable(PWM1_BASE, PWM_GEN_3);

    // Turn on the Output pins
    PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT | PWM_OUT_6_BIT | PWM_OUT_7_BIT, true);

}
void color_change(void)
{

    switch(color)
    {
    case 0:
        red_intensity -= step;
        green_intensity += step;
        if(red_intensity<10)
        {
            red_intensity = intensity_max;
            green_intensity = intensity_max;
            blue_intensity = intensity_min;
            color = 1;
        }
        else
        {
            PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5,red_intensity);
            PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,intensity_min);
            PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,green_intensity);
        }
        break;
    case 1:
        green_intensity -= step;
        blue_intensity += step;
        if(green_intensity<10)
        {
            green_intensity = intensity_max;
            blue_intensity = intensity_max;
            red_intensity = intensity_min;
            color = 2;
        }
        else
        {
            PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5,intensity_min);
            PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,blue_intensity);
            PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,green_intensity);

        }
        break;
    case 2:
        blue_intensity -= step;
        red_intensity += step;
        if(blue_intensity<10)
        {
            blue_intensity = intensity_max;
            red_intensity = intensity_max;
            green_intensity = intensity_min;
            color = 0;
        }
        else
        {
            PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5,red_intensity);
            PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,blue_intensity);
            PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,intensity_min);
        }
        break;
    }

}

void Timer0IntHandler(void)
{
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    detect_key_press();
}

void main()
{

    system_setup();

    switch_config();

    unlocking_pins();

    gpio_config();

    timer_config();

    pwm_config();

    while(1)
    {
        color_change();
        delay_ms(200);
    }

}


