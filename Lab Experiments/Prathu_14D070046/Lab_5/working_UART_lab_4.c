/*
 * working_UART_lab_4.c
 *
 *  Created on: 03-Feb-2018
 *      Author: praton
 */

#define DEFINE_VARIABLES

#include "headers_lab_4.h"
#include "config_header_lab_4.h"
#include "my_functions_lab_4.h"

uint8_t sent = 0;
void setup_system(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
}
void setup_uart(void)
{
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
    UARTEnable(UART0_BASE);
}

void gpio_config_uart(void)
{
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 2);
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4,16);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
}

void delay_ms(int ms)
{
    SysCtlDelay( (SysCtlClockGet()/(3*1000))*ms ) ;
}

int main(void)
{

    setup_system();
    setup_uart();
    gpio_config_uart();

    while (1)
    {
        UARTCharPut(UART0_BASE, 'E');
        sent = 1;
        delay_ms(200);
        sent = 0;
        delay_ms(200);
//        if (UARTCharsAvail(UART0_BASE)) UARTCharPut(UART0_BASE, UARTCharGet(UART0_BASE));
    }
}


