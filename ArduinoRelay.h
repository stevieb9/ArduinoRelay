#ifndef ARDUINORELAY_H
#define ARDUINORELAY_H

#include "Arduino.h"

using namespace std;

class ArduinoRelay {

    protected:

        uint8_t _init           = false;
        uint8_t _reverseState   = false;
        uint8_t _on             = HIGH;
        uint8_t _off            = LOW;
        uint8_t _state          = _on;

        char*   _name = "";
        int8_t  _pin  = -1;

    public:
        
        ArduinoRelay ();
        ArduinoRelay (int8_t pin) { _pin = pin; }

        uint8_t state () { return _state; }

        char*   name () { return _name; }
        char*   name (char* name) { _name = name; }

        uint8_t reverse () { return _reverseState; }
        uint8_t reverse (uint8_t);

        int8_t  pin () { return _pin; }
        int8_t  pin (int8_t pin) { _pin = pin; return _pin; }
};

#endif

