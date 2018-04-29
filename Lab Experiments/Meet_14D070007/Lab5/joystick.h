#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/adc.h"
#include "driverlib/timer.h"

#ifndef __JOYSTICK_H

#define JOY_GPIO_BASE GPIO_PORTD_BASE
#define JOY_ADC_BASE ADC0_BASE
#define JOY_TIMER_BASE TIMER0_BASE

#define JOY_SAMPLE_RATE 4

#define JOY_X GPIO_PIN_0
#define JOY_Y GPIO_PIN_1
#define JOY_CH_X ADC_CTL_CH7
#define JOY_CH_Y ADC_CTL_CH6

void configJoystick();
void ADC0IntHandler(void);

#endif // __JOYSTICK_H
