
#include <stdint.h>
#include <stdbool.h>

#ifndef __SWITCH_H

#define SW_STATE_IDLE 1
#define SW_STATE_PRESS 2
#define SW_STATE_RELEASE 4

#define SW_PRESSED 1
#define SW_RELEASED 2

uint8_t detectKeyPress(uint8_t* currState, uint8_t swVal);

#endif //ndef __SWITCH_H
