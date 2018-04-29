/*
// File name:      "Lab1_C.c"
// Description:    Change toggle delay and color of LED using switches

// Written By:     Yogesh Mahajan (y.mahajan@gmail.com) [14D070022, IIT Bombay]

// Dependencies:   "basic_includes.h"
                   "Lab1_C.h"

// Functions        taskSetup():        Configure interrupts and timer
                    toggleLED():        Timer ISR. Toggle LED on overflow
                    changeLEDConfig():  Change LED toggle period and color
*/
////////////////////////////////////////////////////////////////////////////////
#include "basic_include.h"
#include "Tiva_SwitchManager.h"

int32_t ui32SwitchStatus;

void switchManager(int32_t *ui32PortStatus, int32_t ui32SWMask, int32_t *ui32SWFSMState, int32_t *ui32SWCount , int32_t *ui32PressCount, int32_t  *ui32LongPress)
{
    // switch status will be high if switch is pressed
    ui32SwitchStatus = (~ *ui32PortStatus) & ui32SWMask;
    switch(*ui32SWFSMState)
    {
        case SW_UNPRESSED:
            if(ui32SwitchStatus)
            {
                // if switch is low, move to wait state for debouncing
                *ui32SWFSMState = *ui32SWFSMState << 1;
            }
            break;

        case SW_PRESSWAIT:
            if(ui32SwitchStatus)
            {
                // if switch is pressed, move to pressed state
                *ui32SWFSMState = *ui32SWFSMState << 1;
                // increase switch press count
                *ui32SWCount  = *ui32SWCount + 1;
            }
            else
            {
                // if switch is high, move back to unpressed state
                *ui32SWFSMState = *ui32SWFSMState >> 1;
            }
            break;

        case SW_PRESSED:
            if(ui32SwitchStatus)
            {
                // if switch is still pressed
                *ui32SWCount  = *ui32SWCount + 1;
            }
            else
            {
                // if switch is released move to release wait state
                *ui32SWFSMState = *ui32SWFSMState << 1;
            }
            break;
        case SW_RELESEWAIT:
            if(ui32SwitchStatus)
            {
                // if switch is still pressed
                *ui32SWCount++;
                // move back to pressed state
                *ui32SWFSMState = *ui32SWFSMState >> 1;
            }
            else
            {
                // switch is actually released
                if(*ui32SWCount > g_ui32LongPressTime)
                {
                    *ui32LongPress = 1;
                }
                else
                {
                    *ui32PressCount = *ui32PressCount + 1;
                }
                *ui32SWCount = 0;
                *ui32SWFSMState = SW_UNPRESSED;
            }
    }
}
