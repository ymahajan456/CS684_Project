/*
 * main.c
 */
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/debug.h"
#include "driverlib/pin_map.h"
#include "inc/hw_gpio.h"

#include "glcd.h"
#include "mickey.h"
#include "logo.h"
#include "one.h"
#include "two.h"
#include "three.h"
#include "four.h"
#include "five.h"
#include "six.h"
#include "seven.h"
#include "eight.h"
#include "joystick.h"

#define BUZZER GPIO_PIN_4

#define DSTEP 1340000

uint32_t delay = 13400000;
const unsigned char* frames[] = {one, two, three, four, five, six, seven, eight};

void configPeripherals() {
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, BUZZER);
    GPIOPinWrite(GPIO_PORTC_BASE, BUZZER, BUZZER);

    glcd_config();

    configJoystick();
}

void sendSampleData(uint32_t x, uint32_t y) {
    if (x < 200) {
        delay -= DSTEP;
    } else if (x > 3900) {
        delay += DSTEP;
    }
    if(delay < DSTEP) delay = DSTEP;
    if(delay > 15*DSTEP) delay = 15*DSTEP;
}

int main(void) {
    uint8_t i=0;
    unsigned char bitseq;

    configPeripherals();
    glcd_init();
    glcd_cleardisplay();
    glcd_printimage(logo);
    SysCtlDelay(1340000);
    glcd_printimage(mickey);
    SysCtlDelay(13400000);
    glcd_cleardisplay();
	
    while(1){
        glcd_printimage(frames[i]);
        i++;
        i = i%8;
        SysCtlDelay(delay);
    }
}
