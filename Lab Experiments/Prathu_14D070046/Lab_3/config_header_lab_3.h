/*
 * config_header_lab_3.h
 *
 *  Created on: 31-Jan-2018
 *      Author: praton
 */

#ifndef CONFIG_HEADER_LAB_3_H_
#define CONFIG_HEADER_LAB_3_H_

#ifdef DEFINE_VARIABLES
#define EXTERN
#define INITIALIZER(...)        = __VA_ARGS__
#else
#define EXTERN                  extern
#define INITIALIZER(...)
#endif

// global variable for period value
EXTERN uint32_t ui32Period;
EXTERN volatile uint32_t ui32Load INITIALIZER(0);
EXTERN volatile uint32_t ui32PWMClock INITIALIZER(0);
EXTERN volatile uint8_t ui8Adjust INITIALIZER(0);
EXTERN volatile uint8_t ui8Adjust_min INITIALIZER(80);
EXTERN volatile uint8_t ui8Adjust_max INITIALIZER(130);
//Configuration Functions

extern void setup(void);

extern void led_pin_config(void);

extern void switch_config(void);

extern void unlocking_pins(void);

extern void timer_config(void);

extern void pwm_configure(void);

extern void config_lab_3(void);

#endif /* CONFIG_HEADER_LAB_3_H_ */
