#ifndef ARDUINORELAY_H
#define ARDUINORELAY_H

#include "Arduino.h"

#define RELAY_COOL      0
#define RELAY_HEAT      1
#define RELAY_HUMID     2
#define RELAY_DEHUMID   3
#define RELAY_CYCLE     4
#define RELAY_SWITCH    5

using namespace std;

class ArduinoRelay {

    private:

        void processTempRelay   (float tempF);
        void processHumidRelay  (float humidity);
        void processCycleRelay  ();

        /* Core */

        int8_t  _type           =  RELAY_SWITCH;
        int8_t  _pin            = -1;
        uint8_t _on             = HIGH;
        uint8_t _off            = LOW;
        uint8_t _reverseState   = false;
        uint8_t _state          = _off;
        uint8_t _factor         = 1;

        /* Temp relay */

        uint8_t _temp           = 78;
        uint8_t _onTemp         = 0;
        uint8_t _offTemp        = 0;

        /* Hum relay */

        uint8_t _humidity       = 45;
        uint8_t _onHum          = 0;
        uint8_t _offHum         = 0;

        /* Cycle relay */

        uint8_t       _init     = false;
        unsigned long _onTime   = 5000;
        unsigned long _offTime  = 5000;
        unsigned long _pMillis  = 0;

    public:
        /* Config */

        uint8_t confState      =  0; // In conf mode?
        int8_t confSet         = -1; // Set button
        int8_t confUp          = -1; // + button
        int8_t confDown        = -1; // - button
        uint8_t confActions    =  0; // Number of set screens
        int8_t num             = -1;

        ArduinoRelay () = default;
        explicit ArduinoRelay (int8_t type) { _type = type; }
        ArduinoRelay (int8_t type, int8_t pin) {
            _type = type;
            _pin = pin;
            pinMode(_pin, OUTPUT);
        }
        ArduinoRelay (int8_t type, int8_t pin, uint8_t value); // temp/hum
        ArduinoRelay (int8_t type, int8_t pin, unsigned long onTime, unsigned long offTime);

        ~ArduinoRelay () { if (pin() != -1){ pinMode(pin(), INPUT); } }

        /* For multi-relay configurations */
        void automate (uint8_t relayNum);

        void process ();                // cycle
        void process (double value);    // cool, heat, hum, dehum, sw

        int8_t type () { return _type; }

        int8_t pin () { return _pin; }
        int8_t pin (int8_t pin) { _pin = pin; return _pin; }

        uint8_t on ()  { return _on; }
        uint8_t off () { return _off; }

        uint8_t reverse () { return _reverseState; }
        uint8_t reverse (uint8_t rev);

        uint8_t factor () { return _factor; }
        uint8_t factor (uint8_t factor) { _factor = factor; return _factor; }

        uint8_t state () { return _state; }
        uint8_t state (uint8_t state) { _state = state; return _state; }

        void turnOn ()  { digitalWrite(pin(), on()); }
        void turnOff () { digitalWrite(pin(), off()); }

        /* Temperature Relay */

        uint8_t baseTemp () { return _temp; }
        uint8_t baseTemp (uint8_t t);

        uint8_t onTemp () { return _onTemp; }
        uint8_t onTemp (uint8_t onTemp) { _onTemp = onTemp; return _onTemp; }

        uint8_t offTemp () { return _offTemp; }
        uint8_t offTemp (uint8_t offTemp) { _offTemp = offTemp; return _offTemp; }

        /* Humidity Relay */

        uint8_t baseHumidity () { return _humidity; }
        uint8_t baseHumidity (uint8_t humidity);

        uint8_t onHum () { return _onHum; }
        uint8_t onHum (uint8_t onHum) { _onHum = onHum; return _onHum; }

        uint8_t offHum () { return _offHum; }
        uint8_t offHum (uint8_t offHum) { _offHum = offHum; return _offHum; }

        /* Cycle Relay */

        uint8_t init () { return _init; }
        uint8_t init (uint8_t init) { _init = init; return _init; }

        unsigned long prevMillis () { return _pMillis; }
        unsigned long prevMillis (unsigned long millis) { _pMillis = millis; return _pMillis; }

        unsigned long onTime () { return _onTime; }
        unsigned long onTime (unsigned long millis) { _onTime = millis; return _onTime; }

        unsigned long offTime () { return _offTime; }
        unsigned long offTime (unsigned long millis) { _offTime = millis; return _offTime; }
};

#endif

