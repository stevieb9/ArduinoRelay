#include "Arduino.h"
#include "ArduinoRelay.h"

/* METHODS */

uint8_t ArduinoRelay::reverse (uint8_t set) {

    if (set) {
        _reverseState = true; 
        _on = LOW;
        _off = HIGH;
    }

    return _reverseState;
}
