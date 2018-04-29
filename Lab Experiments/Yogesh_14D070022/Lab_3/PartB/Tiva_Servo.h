/*

    Functions to set up on-board LEDs on LauncPad board (TM4C123GH6PM)
 
    Description:
    ------------
        Pins:
        1. Red LED : GPIO = PF1; PWM = M1PWM5 Gen 2
        2. Green Led : GPIO = PF2; PWM = M1PWM6 GEN 3
        3. Blue Led : GPIO = PF3; PWM = M1PWM7 GEN 3 
*/

#ifndef _TIVAC_SERVO_H_
#define _TIVAC_SERVO_H_

#include "basic_include.h"

#ifdef DEFINE_VARIABLES
#define EXTERN                  
#define INITIALIZER(...)        = __VA_ARGS__
#else
#define EXTERN                  extern
#define INITIALIZER(...)       
#endif 

#define SERVO_PWM_PERIPHERAL    SYSCTL_PERIPH_PWM1
#define SERVO_GPIO_PERIPHERAL   SYSCTL_PERIPH_GPIOD
#define SERVO_GPIO_BASE         GPIO_PORTD_BASE
#define SERVO_GPIO_PIN          GPIO_PIN_0
#define SERVO_PIN_CONFIG        GPIO_PD0_M1PWM0
#define SERVO_PWM_BASE          PWM1_BASE
#define SERVO_PWM_GEN           PWM_GEN_0
#define SERVO_PIN               PWM_OUT_0
#define SERVO_PIN_BIT           PWM_OUT_0_BIT

#define SERVO_PWM_FREQENCY      50                  // typical for servos

#define SERVO_PWM_CLOCK_DIV     SYSCTL_PWMDIV_64

EXTERN uint32_t g_ui32ServoRest;
EXTERN uint32_t g_ui32ServoMin;
EXTERN uint32_t g_ui32ServoMax;
EXTERN uint32_t g_ui32ServoStep;

extern void initServo();

#endif
