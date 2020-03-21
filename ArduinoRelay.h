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
        ArduinoRelay (int8_t pin) { _pin = pin; pinMode(_pin, OUTPUT); }
        ~ArduinoRelay () { if (pin() != -1){ pinMode(pin(), INPUT); } }

        uint8_t state () { return _state; }
        uint8_t state (uint8_t state) { _state = state; return _state; }

        char*   name () { return _name; }
        char*   name (char* name) { _name = name; return _name;}

        uint8_t reverse () { return _reverseState; }
        uint8_t reverse (uint8_t rev);

        int8_t  pin () { return _pin; }
        int8_t  pin (int8_t pin) { _pin = pin; return _pin; }

        uint8_t init () { return _init; }
        uint8_t init (uint8_t init) { _init = init; return _init; }

        uint8_t on ()  { return _on; }
        uint8_t off () { return _off; }

        void    turnOn ()  { digitalWrite(_pin, _on); }
        void    turnOff () { digitalWrite(_pin, _off); }
};

#endif

