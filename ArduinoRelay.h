#ifndef ARDUINORELAY_H
#define ARDUINORELAY_H

#include "Arduino.h"

using namespace std;

class ArduinoRelay {

    protected:

        int8_t  _pin            = -1;
        uint8_t _on             = HIGH;
        uint8_t _off            = LOW;
        uint8_t _reverseState   = false;
        uint8_t _state          = _off;

    public:
        
        ArduinoRelay () = default;
        explicit ArduinoRelay (int8_t pin) { _pin = pin; pinMode(_pin, OUTPUT); }
        ~ArduinoRelay () { if (pin() != -1){ pinMode(pin(), INPUT); } }

        int8_t  pin () { return _pin; }
        int8_t  pin (int8_t pin) { _pin = pin; return _pin; }

        uint8_t on ()  { return _on; }
        uint8_t off () { return _off; }

        uint8_t reverse () { return _reverseState; }
        uint8_t reverse (uint8_t rev);

        uint8_t state () { return _state; }
        uint8_t state (uint8_t state) { _state = state; return _state; }

        void    turnOn ()  { digitalWrite(pin(), on()); }
        void    turnOff () { digitalWrite(pin(), off()); }
};

#endif

