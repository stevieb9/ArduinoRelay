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

uint8_t ArduinoRelay::mode (uint8_t opMode) {
    _mode = opMode;

    if (type() == RELAY_TEMP) {
        if (mode() == RELAY_MODE_COOL) {
            onTemp(baseTemp());
            offTemp(baseTemp() - factor());
        } else if (mode() == RELAY_MODE_HEAT) {
            onTemp(baseTemp());
            offTemp(baseTemp() + factor());
        }
    }

    if (type() == RELAY_HUMID) {
        if (mode() == RELAY_MODE_HUMIDIFY) {
            onHum(baseHumidity());
            offHum(baseHumidity() - factor());
        } else if (_mode() == RELAY_MODE_DEHUMIDIFY) {
            onHum(baseHumidity());
            offHum(baseHumidity() + factor());
        }
    }

    return _mode;
}

void ArduinoRelay::process(float value) {
    if (pin() == -1) {
        return;
    }

    switch (type()) {
        case RELAY_TEMP:
            break;
        case RELAY_HUMID:
            processHumidRelay(value);
            break;
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

