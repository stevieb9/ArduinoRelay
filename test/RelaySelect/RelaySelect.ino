#include <Arduino.h>
#include <ArduinoRelay.h>
#include <MultiButton.h>

#define NUM_RELAYS 2

const char relayTypes[6][8] = {
        "COOL",
        "HEAT",
        "HUMID",
        "DEHUMID",
        "CYCLE",
        "SWITCH"
};

ArduinoRelay r[NUM_RELAYS];
MultiButton mb;

void setup() {
    Serial.begin(9600);
    Serial.println();

    relayAutomate();

    Serial.print(F("Relay0: "));
    Serial.println(r[0].type());
    Serial.println(r[1].state());
    Serial.println(r[0].confSet);

    Serial.print(F("Relay1: "));
    Serial.println(r[1].type());
    Serial.println(r[1].state());
    Serial.println(r[1].confSet);

    relayConfigCheck();
}

void loop() {
    relayConfigCheck();
}

void relayAutomate () {
    for (uint8_t i = 0; i < NUM_RELAYS; i++) {
        r[i].automate(i);
    }
}

void relayConfigCheck () {
    int8_t buttonPressed = mb.pressed();

    for (uint8_t i = 0; i < NUM_RELAYS; i++) {
        if (buttonPressed == r[i].confSet) {
            ArduinoRelay tempRelay;

            // Select relay type
            uint8_t position = 0;

            while (tempRelay.confState < tempRelay.confActions) {
                Serial.print("State: ");
                Serial.println(tempRelay.confState);

                delay(0);  // To avoid reboot of while loop taking too long

                buttonPressed = mb.pressed();

                if (buttonPressed > -1) {
                    Serial.print("Button: ");
                    Serial.println(buttonPressed);
                    if (r[i].confState == 0) {
                        // RELAY TYPE SELECT
                        if (buttonPressed == r[i].confUp) {
                            if (position < RELAY_TYPE_COUNT - 1) {
                                position++;
                            }
                            Serial.println(relayTypes[position]);
                        }
                        else if (buttonPressed == r[i].confDown) {
                            if (position > 0) {
                                position--;
                            }
                            Serial.println(relayTypes[position]);
                        }
                        else if (buttonPressed == r[i].confSet) {
                            tempRelay.type(position);
                            buttonPressed = -1;
                            r[i].confState++;
                        }
                    }
                    else if (tempRelay.type() == RELAY_COOL || tempRelay.type() == RELAY_HEAT || tempRelay.type() == RELAY_HUMID || tempRelay.type() == RELAY_DEHUMID) {

                        Serial.print("TEMP ");
                        Serial.print(tempRelay.type());

                        if (r[i].confState == 1) {

                            if (tempRelay.type() == RELAY_COOL || tempRelay.type() == RELAY_HEAT) {
                                // RELAY TEMP SELECT
                                if (buttonPressed == r[i].confUp) {
                                    if (tempRelay.baseTemp() < RELAY_MAX_TEMP) {
                                        tempRelay.baseTemp(tempRelay.baseTemp() + 1);
                                    }
                                    Serial.print("Temp++: ");
                                    Serial.println(tempRelay.baseTemp());
                                }
                                if (buttonPressed == r[i].confDown) {
                                    if (tempRelay.baseTemp() > RELAY_MIN_TEMP) {
                                        tempRelay.baseTemp(tempRelay.baseTemp() - 1);
                                    }
                                    Serial.print("Temp--: ");
                                    Serial.println(tempRelay.baseTemp());
                                }
                                else if (buttonPressed == r[i].confSet) {
                                    r[i].confState++;
                                }

                            }
                        }
                    }
                }
            }
        }
    }
}
