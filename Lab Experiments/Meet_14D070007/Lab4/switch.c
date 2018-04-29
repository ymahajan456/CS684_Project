#include "switch.h"

/*
 * Detect key pressed or released using state machine
 *
 * currState: pointer to current state variable
 * swVal: Value of switch which was read
 *
 * Return: SW_PRESSED if switch pressed, SW_RELEASED if released else 0
 */
uint8_t detectKeyPress(uint8_t* currState, uint8_t swVal) {
    uint8_t nextState = SW_STATE_IDLE;
    uint8_t ret = 0;

    if(swVal) { // released
        nextState = SW_STATE_IDLE;
        if(*currState == SW_STATE_RELEASE) {
            ret = SW_RELEASED;
        }
    } else { // pressed
        switch(*currState) {
            case SW_STATE_IDLE:
                nextState = SW_STATE_PRESS;
                break;
            case SW_STATE_PRESS:
                nextState = SW_STATE_RELEASE;
                ret = SW_PRESSED;
                break;
            case SW_STATE_RELEASE:
                nextState = SW_STATE_RELEASE;
                break;
        }
    }
    *currState = nextState;
    return ret;
}
