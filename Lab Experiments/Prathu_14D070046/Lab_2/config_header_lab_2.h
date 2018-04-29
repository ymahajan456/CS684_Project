/*
 * config_header_lab_2.h
 *
 *  Created on: 23-Jan-2018
 *      Author: praton
 */

#ifndef CONFIG_HEADER_LAB_2_H_
#define CONFIG_HEADER_LAB_2_H_

#ifdef DEFINE_VARIABLES
#define EXTERN
#define INITIALIZER(...)        = __VA_ARGS__
#else
#define EXTERN                  extern
#define INITIALIZER(...)
#endif

// global variable for period value
EXTERN uint32_t ui32Period;
//Configuration Functions

extern void setup(void);

extern void led_pin_config(void);

extern void switch_config(void);

extern void unlocking_pins(void);

extern void timer_config(void);

extern void config_lab_2(void);

#endif /* CONFIG_HEADER_LAB_2_H_ */
