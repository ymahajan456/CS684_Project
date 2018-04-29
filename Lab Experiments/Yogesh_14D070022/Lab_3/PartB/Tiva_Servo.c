#include "Tiva_Servo.h"

void initServo()
{
    ROM_SysCtlPWMClockSet(SERVO_PWM_CLOCK_DIV);
    // Enable PWN peripheral
    ROM_SysCtlPeripheralEnable(SERVO_PWM_PERIPHERAL);
    // Enable GPIO peripheral
    ROM_SysCtlPeripheralEnable(SERVO_GPIO_PERIPHERAL);

    ROM_GPIOPinTypePWM(SERVO_GPIO_BASE, SERVO_GPIO_PIN);
    ROM_GPIOPinConfigure(SERVO_PIN_CONFIG);

    int32_t ui32ServoClock = SysCtlClockGet() / 64;
    int32_t ui32Load = (ui32ServoClock / SERVO_PWM_FREQENCY) - 1;
    g_ui32ServoMax = ui32Load/8;
    g_ui32ServoMin = ui32Load/28;
    g_ui32ServoStep = (g_ui32ServoMax-g_ui32ServoMin)/18;


    g_ui32ServoRest = (g_ui32ServoMax + g_ui32ServoMin)/2;
    PWMGenConfigure(SERVO_PWM_BASE, SERVO_PWM_GEN, PWM_GEN_MODE_DOWN);
    PWMGenPeriodSet(SERVO_PWM_BASE, SERVO_PWM_GEN, ui32Load);
    ROM_PWMPulseWidthSet(SERVO_PWM_BASE, SERVO_PIN, g_ui32ServoRest);
    ROM_PWMOutputState(SERVO_PWM_BASE, SERVO_PIN_BIT, true);
    ROM_PWMGenEnable(SERVO_PWM_BASE, SERVO_PWM_GEN);
}
