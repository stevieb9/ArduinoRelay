#include "Arduino.h"
#include "ArduinoRelay.h"

uint8_t ArduinoRelay::reverse (uint8_t set) {

    if (set) {
        _reverseState = true; 
        _on = LOW;
        _off = HIGH;
    }
    else {
        _reverseState = false;
        _on = HIGH;
        _off = LOW;
    }
    return _reverseState;
}
