/*
 * adc_lab_4.c
 *
 *  Created on: 03-Feb-2018
 *      Author: praton
 */

#define DEFINE_VARIABLES

#include "headers_lab_4.h"
#include "config_header_lab_4.h"
#include "my_functions_lab_4.h"

uint32_t ui32ADC6Value[4];
uint32_t ui32ADC7Value[4];
volatile uint32_t ui32ADC6Avg;
volatile uint32_t ui32ADC7Avg;

void main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);
    GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_0|GPIO_PIN_1);

    /* AIN6 = PD1
     * AIN7 = PD0
     * */
//    ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
//    ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH6);
//    ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_CH6);
//    ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_CH7);
//    ADCSequenceStepConfigure(ADC0_BASE,1,3,ADC_CTL_CH7|ADC_CTL_IE|ADC_CTL_END);
//    ADCSequenceEnable(ADC0_BASE, 1);

    ADCSequenceConfigure(ADC0_BASE, 2, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 2, 0, ADC_CTL_CH6);
    ADCSequenceStepConfigure(ADC0_BASE, 2, 1, ADC_CTL_CH6);
    ADCSequenceStepConfigure(ADC0_BASE, 2, 2, ADC_CTL_CH7);
    ADCSequenceStepConfigure(ADC0_BASE,2,3,ADC_CTL_CH7|ADC_CTL_IE|ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 2);

    while(1)
    {
//        ADCIntClear(ADC0_BASE, 1);
//
//        ADCProcessorTrigger(ADC0_BASE, 1);
//
//        while(!ADCIntStatus(ADC0_BASE, 1, false))
//        {}
//
//        ADCSequenceDataGet(ADC0_BASE, 2, ui32ADC6Value);

        ADCIntClear(ADC0_BASE, 2);

        ADCProcessorTrigger(ADC0_BASE, 2);

        while(!ADCIntStatus(ADC0_BASE, 2, false))
        {}

        ADCSequenceDataGet(ADC0_BASE, 2, ui32ADC7Value);

        ui32ADC6Avg = (ui32ADC7Value[0] + ui32ADC7Value[1] + 2)/4;
        ui32ADC7Avg = (ui32ADC7Value[2] + ui32ADC7Value[3] + 2)/4;

    }
}
