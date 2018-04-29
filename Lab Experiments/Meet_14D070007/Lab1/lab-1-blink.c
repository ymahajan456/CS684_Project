/*

* Author: Texas Instruments

* Editted by: Parin Chheda
          ERTS Lab, CSE Department, IIT Bombay

* Description: This code structure will assist you in completing Lab 1
* Filename: lab-1.c

* Functions: setup(), config(), main()


*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_ints.h"
#include <time.h>

#define RED GPIO_PIN_1
#define BLUE GPIO_PIN_2
#define GREEN GPIO_PIN_3

#define SW1 GPIO_PIN_4
#define SW2 GPIO_PIN_0

uint8_t LEDColor = RED;
uint32_t BlinkDelay = 50; // 0.5s

/*

* Function Name: setup()

* Input: none

* Output: none

* Description: Set crystal frequency,enable GPIO Peripherals and unlock Port F pin 0 (PF0)

* Example Call: setup();

*/
void setup(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    
	//unlock PF0 based on requirement
    HWREG(GPIO_PORTF_BASE+GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE+GPIO_O_CR) |= SW2;
}

/*

* Function Name: pin_config()

* Input: none

* Output: none

* Description: Set Port F Pin 1, Pin 2, Pin 3 as output. On this pin Red, Blue and Green LEDs are connected.
			   Set Port F Pin 0 and 4 as input, enable pull up on both these pins.

* Example Call: pin_config();

*/

void pin_config(void)
{
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,
                          RED | BLUE | GREEN);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,
                         SW1 | SW2);
    GPIOPadConfigSet(GPIO_PORTF_BASE, SW1 | SW2,
            GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}

uint8_t nextLEDState(uint8_t currState) {
    switch(currState) {
        case RED: return GREEN;
        case BLUE: return RED;
        case GREEN: return BLUE;
        default: return RED;
    }
}

uint32_t nextBlinkDelay(uint32_t delay) {
    if (delay >= 200) {
        return 50;
    }
    return delay*2;
}

int main(void)
{
    uint32_t switchState = SW1 | SW2, newState = 0;
    uint32_t ticks = 0;
    uint8_t LEDState = 0;

    setup();
    pin_config();

    while (1)
    {
        newState = GPIOPinRead(GPIO_PORTF_BASE, SW1 | SW2);

        if (!(newState & SW1)) {
            if (switchState & SW1) { // FALLING EDGE
                BlinkDelay = nextBlinkDelay(BlinkDelay);
            }
        }

        if (!(newState & SW2)) {
            if (switchState & SW2) { // FALLING EDGE
                LEDColor = nextLEDState(LEDColor);
            }
        }

        ticks ++;

        if(ticks >= BlinkDelay) {
            GPIOPinWrite(GPIO_PORTF_BASE, RED | GREEN | BLUE, LEDState ? LEDColor : 0);
            LEDState = !LEDState;
            ticks = 0;
        }

        switchState = newState;

        SysCtlDelay(134000); // 10 ms
    }
}
