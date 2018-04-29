#include "joystick.h"
#include "driverlib/uart.h"

extern void sendSampleData(uint32_t x, uint32_t y);

uint32_t adcSamples[4];

void configJoystick() {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    GPIOPinTypeADC(JOY_GPIO_BASE, JOY_X | JOY_Y);

    ADCSequenceDisable(JOY_ADC_BASE, 1);

    ADCSequenceConfigure(JOY_ADC_BASE, 1, ADC_TRIGGER_TIMER, 0);
    ADCSequenceStepConfigure(JOY_ADC_BASE, 1, 0, JOY_CH_X);
    ADCSequenceStepConfigure(JOY_ADC_BASE, 1, 1, JOY_CH_Y);
    ADCSequenceStepConfigure(JOY_ADC_BASE, 1, 2, JOY_CH_X);
    ADCSequenceStepConfigure(JOY_ADC_BASE, 1, 3, JOY_CH_Y | ADC_CTL_IE | ADC_CTL_END);

    ADCIntRegister(JOY_ADC_BASE, 1, ADC0IntHandler);
    ADCIntEnable(JOY_ADC_BASE, 1);

    TimerConfigure(JOY_TIMER_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(JOY_TIMER_BASE, TIMER_A, SysCtlClockGet()/JOY_SAMPLE_RATE);
    TimerControlTrigger(JOY_TIMER_BASE, TIMER_A, true);

    ADCSequenceEnable(JOY_ADC_BASE, 1);
    TimerEnable(JOY_TIMER_BASE, TIMER_A);
}


void ADC0IntHandler(void) {
    ADCIntClear(JOY_ADC_BASE, 1);
    ADCSequenceDataGet(JOY_ADC_BASE, 1, adcSamples);
    sendSampleData(adcSamples[1], adcSamples[0]);
}
