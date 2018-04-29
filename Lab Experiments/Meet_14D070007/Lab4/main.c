/*
 * Author: Meet Udeshi
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/adc.h"
#include "driverlib/uart.h"
#include "driverlib/timer.h"

#include "switch.h"
#include "joystick.h"

#define BUZZER GPIO_PIN_4

unsigned char numx[5], numy[5];
uint32_t fifoFull = 0;


void configPeripherals() {
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, BUZZER);
    GPIOPinWrite(GPIO_PORTC_BASE, BUZZER, BUZZER);
}

void configUART() {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);
}

void int2str(uint32_t num, unsigned char * str) {
    int8_t i;
    for(i = 4; i >= 0; i--) {
        str[i] = num%10 + 48;
        num = num/10;
    }
}

void sendSampleData(uint32_t x, uint32_t y) {
    uint8_t i;

    int2str(x, numx);
    int2str(y, numy);

    for(i = 0; i < 5; i++) {
        UARTCharPutNonBlocking(UART0_BASE, numx[i]);
    }

    UARTCharPutNonBlocking(UART0_BASE, ',');

    for(i = 0; i < 5; i++) {
        UARTCharPutNonBlocking(UART0_BASE, numy[i]);
    }

    if(UARTCharPutNonBlocking(UART0_BASE, '\n')) {
        fifoFull ++;
    }
}

int main(void) {
    configPeripherals();
    configJoystick();
    configUART();

    while(1) {
    }
}
