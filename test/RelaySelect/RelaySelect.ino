#include <Arduino.h>
#include <ArduinoRelay.h>

#define NUM_RELAYS 2

ArduinoRelay r[NUM_RELAYS];

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

}

void relayAutomate () {
    for (uint8_t i=0; i<NUM_RELAYS; i++) {
        r[i].automate(i);
    }
}

void relayConfigCheck () {
    for (uint8_t i=0; i<NUM_RELAYS; i++) {
        if (r[i].confSet
    }

}

