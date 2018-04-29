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
#include "joystick.h"

#define BUZZER GPIO_PIN_4

uint32_t x_sq=28,y_sq=60, tx, ty;
bool dirty = false;

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
    tx = 60 - x/64;
    ty = 60 - y/64;

    if (tx != x_sq || ty != y_sq) {
        dirty = true;
    }
}

unsigned char makeBitseq(uint8_t p) {
    return (1 << (p+1)) - 1;
}

int main(void) {
    uint8_t i, p, p_over;
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
        if(dirty){
            p = y_sq / 8;

            for(i = 0; i < 8; i++) {
                glcd_setpage(p);
                glcd_setcolumn(x_sq + 32 + i);
                glcd_data(0x00);

                glcd_setpage(p+1);
                glcd_setcolumn(x_sq + 32 + i);
                glcd_data(0x00);
            }

            x_sq = tx;
            y_sq = ty;

            p = y_sq / 8;
            p_over = y_sq % 8;
            bitseq = makeBitseq(7 - p_over);

            for(i = 0; i < 8; i++) {
                glcd_setpage(p);
                glcd_setcolumn(x_sq + 32 + i);
                glcd_data(~bitseq);

                glcd_setpage(p+1);
                glcd_setcolumn(x_sq + 32 + i);
                glcd_data(bitseq);
            }

            dirty = false;
        }
    }
}
