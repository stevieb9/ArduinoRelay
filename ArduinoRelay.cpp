#include "Arduino.h"
#include "ArduinoRelay.h"

ArduinoRelay::ArduinoRelay (int8_t type, int8_t pin, uint8_t value) {
    _type = type;
    _pin = pin;

    pinMode(_pin, OUTPUT);

    if (_type == RELAY_COOL || _type == RELAY_HEAT) {
        baseTemp(value);
    }
    else if (_type == RELAY_HUMID || _type == RELAY_DEHUMID) {
        baseHumidity(value);
    }
}

ArduinoRelay::ArduinoRelay (int8_t type, int8_t pin, unsigned long onTime, unsigned long offTime) {
    _type       = type;
    _pin        = pin;
    _onTime     = onTime;
    _offTime    = offTime;

    pinMode(_pin, OUTPUT);
}

void ArduinoRelay::automate (uint8_t relayNum) {
    if (relayNum == 0) {
        this->confDown  = 0;
        this->confUp    = 1;
        this->confSet   = 6;
        this->confNum   = 0;
    }

    if (relayNum == 1) {
        this->confDown  = 2;
        this->confUp    = 3;
        this->confSet   = 7;
        this->confNum   = 1;
    }

    switch (this->type()) {
        case RELAY_SWITCH:
            this->confActions = 1; // type
            break;

        case RELAY_COOL:
        case RELAY_HEAT:
        case RELAY_HUMID:
        case RELAY_DEHUMID:
            this->confActions = 2; // type, (humidity|temp)
            break;

        case RELAY_CYCLE:
            this->confActions = 3; // type, onTime, offTime
            break;

        default:
            break;
    }
}

uint8_t ArduinoRelay::reverse (uint8_t set) {
    if (set) {
        _reverseState   = true;
        _on             = LOW;
        _off            = HIGH;
    }
    else {
        _reverseState   = false;
        _on             = HIGH;
        _off            = LOW;
    }
    return _reverseState;
}

uint8_t ArduinoRelay::baseTemp (uint8_t t) {
    _temp = t;

    if (type() == RELAY_COOL) {
        onTemp(baseTemp());
        offTemp(baseTemp() - factor());
    } else if (type() == RELAY_HEAT) {
        onTemp(baseTemp());
        offTemp(baseTemp() + factor());
    }

    return baseTemp();
}

uint8_t ArduinoRelay::baseHumidity (uint8_t h) {
    _humidity = h;

    if (type() == RELAY_HUMID) {
        onHum(baseHumidity());
        offHum(baseHumidity() + factor());
    } else if (type() == RELAY_DEHUMID) {
        onHum(baseHumidity());
        offHum(baseHumidity() - factor());
    }

    return baseHumidity();
}

void ArduinoRelay::process () {
    if (pin() == -1) {
        return;
    }

    switch (type()) {
        case RELAY_CYCLE:
            processCycleRelay();
            break;

        default:
            break;
    }
}

void ArduinoRelay::process (double value) {
    if (pin() == -1) {
        return;
    }

    switch (type()) {
        case RELAY_COOL:
        case RELAY_HEAT:
            processTempRelay((float)value);
            break;

        case RELAY_HUMID:
        case RELAY_DEHUMID:
            processHumidRelay((float)value);
            break;

        case RELAY_SWITCH:
            (uint8_t)value ? turnOn() : turnOff();
            break;

        default:
            break;
    }
}

void ArduinoRelay::processTempRelay (float tempF) {

    if (type() == RELAY_COOL) {
        // Turn the outlet on

        if (state() == off() && tempF > (float)onTemp()) {
            state(on());
            turnOn();
        }

        // Turn the outlet off

        else if (state() == on() && tempF < (float)offTemp()) {
            state(off());
            turnOff();
        }
    }

    else if (type() == RELAY_HEAT) {
        // Turn the outlet on

        if (state() == off() && tempF < (float)onTemp()) {
            state(on());
            turnOn();
        }

        // Turn the outlet off

        else if (state() == on() && tempF > (float)offTemp()) {
            state(off());
            turnOff();
        }
    }
}

void ArduinoRelay::processHumidRelay (float humidity) {

    if (type() == RELAY_HUMID) {
        // Turn the outlet on

        if (state() == off() && humidity < (float)onHum()) {
            state(on());
            turnOn();
        }

        // Turn the outlet off

        else if (state() == on() && humidity > (float)offHum()) {
            state(off());
            turnOff();
        }
    }

    else if (type() == RELAY_DEHUMID) {

        // Turn the outlet on

        if (state() == off() && humidity > (float)onHum()) {
            state(on());
            turnOn();
        }

        // Turn the outlet off

        else if (state() == on() && humidity < (float)offHum()) {
            state(off());
            turnOff();
        }
    }
}

void ArduinoRelay::processCycleRelay () {
    if (prevMillis() == 0) {
        prevMillis(millis());
    }

    unsigned long currentMillis = millis();

    // Turn the outlet on

    if ((state() == off() && currentMillis - prevMillis() >= offTime()) || ! init()) {
        if (! init()) {
            init(true);
        }
        state(on());
        prevMillis(currentMillis);
        turnOn();
    }

    // Turn the outlet off

    else if (state() == on() && currentMillis - prevMillis() >= onTime()) {
        state(off());
        prevMillis(currentMillis);
        turnOff();
    }
}

