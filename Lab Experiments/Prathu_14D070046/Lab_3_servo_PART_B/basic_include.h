/*
// File name:      "basic_includes.h"
// Description:    Header file to include basic TivaWare headers and definitions

// Written By:     Yogesh Mahajan (y.mahajan@gmail.com) [14D070022, IIT Bombay]

// Dependencies:   TivaWrae "driverlibs.lib" must be included in project
*/
////////////////////////////////////////////////////////////////////////////////
#ifndef _BASIC_INCLUDES_H_
#define _BASIC_INCLUDES_H_
// define target
#define TARGET_IS_TM4C123_RA1 // required for ROM mapping
// basic data types
#include <string.h>
#include <stdint.h>   // Standard integer Library
#include <stdbool.h>  // Boolean Library
#include <time.h>
// Pin mapping and basic functions
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"  // Base memory address for all pins
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
// device specific
#include "driverlib/gpio.h"     // device libraries
#include "driverlib/systick.h"  // system tick
#include "driverlib/timer.h"    // Timers
#include "driverlib/interrupt.h"// Interrupts
#include "driverlib/sysctl.h"   // System control (clock, power)
#include "driverlib/rom_map.h"  // On chip ROM (containd few functions)
#include "driverlib/rom.h"      // ROM mapping
#include "driverlib/pwm.h"      // PWM
#include "driverlib/ssi.h"      // Sequential Serial Interface
#include "driverlib/pin_map.h"  // Pin Muxes


// Definitions
#define SW1     GPIO_PIN_4      // User switch 1
#define SW2     GPIO_PIN_0      // User switch 2
#define RED     GPIO_PIN_1      // Red LED
#define BLUE    GPIO_PIN_2      // Blue LED
#define GREEN   GPIO_PIN_3      // Green LED
#define IOBASE  GPIO_PORTF_BASE // GPIO Port F

#endif
