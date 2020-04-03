#include "ArduinoRelay.h"

#define coolP       D0
#define heatP       D7
#define humP        D6
#define dehumP      D1
#define cycP        D5
#define swP         D3

#define TEMP        78
#define HUM         60
#define CYC_ON      1000
#define CYC_OFF     1000

int16_t fail = 0;

ArduinoRelay cool(RELAY_COOL, coolP, TEMP);
ArduinoRelay heat(RELAY_HEAT, heatP, TEMP);
ArduinoRelay hum(RELAY_HUMID, humP, HUM);
ArduinoRelay deHum(RELAY_DEHUMID, dehumP, HUM);
ArduinoRelay cyc(RELAY_CYCLE, cycP, CYC_ON, CYC_OFF);
ArduinoRelay sw(RELAY_SWITCH, swP);

void setup() {
    Serial.begin(9600);
    Serial.print(F("\n\n"));

    cycleRelayTests();
    coolRelayTests();
    heatRelayTests();
    humidityRelayTests();
    deHumidityRelayTests();
    switchRelayTests();

    doneTesting();
}

void doneTesting () {
    if (! fail) {
        Serial.print(F("\n\n\n"));
        Serial.println(F("ALL TESTS OK"));
    }
    else {
        Serial.print(F("\n\n\n"));
        Serial.print(fail);
        Serial.println(F(" TESTS FAILED"));
    }
}

void cycTest (uint8_t state, const __FlashStringHelper* msg) {
    is (cyc.state() == state, msg);
    is (digitalRead(cycP) == state, msg);
}

void humTest (uint8_t state, const __FlashStringHelper* msg) {
    is (hum.state() == state, msg);
    is (digitalRead(humP) == state, msg);
}

void deHumTest (uint8_t state, const __FlashStringHelper* msg) {
    is (deHum.state() == state, msg);
    is (digitalRead(dehumP) == state, msg);
}

void coolTest (uint8_t state, const __FlashStringHelper* msg) {
    is (cool.state() == state, msg);
    is (digitalRead(coolP) == state, msg);
}

void heatTest (uint8_t state, const __FlashStringHelper* msg) {
    is (heat.state() == state, msg);
    is (digitalRead(heatP) == state, msg);
}

void loop() {
}

void is (bool arg, const __FlashStringHelper* msg) {
    if (! arg) {
        fail++;
        Serial.print(F("FAIL: "));
        Serial.println(msg);
    }
}

void switchRelayTests () {
    // SWITCH RELAY

    Serial.println(F("SWITCH RELAY"));

    is (sw.type() == RELAY_SWITCH, F("Relay Switch type"));
    is (sw.confSet == -1, F("Switch confState default"));
    is (sw.confUp == -1, F("Switch confUp default"));
    is (sw.confDown == -1, F("Switch confDown default"));
    is (sw.confActions == 0, F("Switch confActions default"));
    is (sw.num == -1, F("Switch num default"));

    sw.automate(0);

    is (sw.type() == RELAY_SWITCH, F("Relay Switch type post auto"));
    is (sw.confSet == 6, F("Switch confState auto"));
    is (sw.confUp == 1, F("Switch confUp auto"));
    is (sw.confDown == 0, F("Switch confDown auto"));
    is (sw.confActions == 1, F("Switch confActions auto"));

    sw.turnOn();
    is (digitalRead(swP) == HIGH, F("Switch turnOn()"));
    is (digitalRead(swP) == sw.on(), F("Switch turnOn() on()"));

    sw.turnOff();
    is (digitalRead(swP) == LOW, F("Switch turnOff()"));
    is (digitalRead(swP) == sw.off(), F("Switch turnOff() off()"));

    is (sw.reverse(true) == true, F("Switch reverse"));

    sw.turnOn();
    is (digitalRead(swP) == LOW, F("Switch turnOn() LOw"));
    is (digitalRead(swP) == sw.on(), F("Switch turnOn() LOW on()"));

    sw.turnOff();
    is (digitalRead(swP) == HIGH, F("Switch turnOff() HIGH"));
    is (digitalRead(swP) == sw.off(), F("Switch turnOff() HIGH off()"));

    is (sw.reverse(false) == false, F("Switch un-reverse"));

    sw.turnOff();
    is (digitalRead(swP) == LOW, F("Switch turnOff()"));
    is (digitalRead(swP) == sw.off(), F("Switch turnOff() off()"));
}

void cycleRelayTests () {
    // CYCLE RELAY

    Serial.println(F("CYCLE RELAY"));

    is (cyc.type() == RELAY_CYCLE, F("Relay cyc type"));
    is (cyc.confSet == -1, F("cyc confState default"));
    is (cyc.confUp == -1, F("cyc confUp default"));
    is (cyc.confDown == -1, F("cyc confDown default"));
    is (cyc.confActions == 0, F("cyc confActions default"));
    is (cyc.num == -1, F("cyc num default"));

    cyc.automate(1);

    is (cyc.type() == RELAY_CYCLE, F("Relay cyc type post auto"));
    is (cyc.confSet == 7, F("cyc confState auto"));
    is (cyc.confUp == 3, F("cyc confUp auto"));
    is (cyc.confDown == 2, F("cyc confDown auto"));
    is (cyc.confActions == 3, F("cyc confActions auto"));

    is (cyc.reverse(true) == true, F("Cyc rev true"));
    is (cyc.reverse(false) == false, F("Cyc rev false"));

    cyc.process();
    cycTest(HIGH, F("On initial"));
    delay(1010);
    cyc.process();
    cycTest(LOW, F("Off initial"));

    is (cyc.onTime(5000) == 5000, F("cyc onTime() 5000"));
    is (cyc.offTime(5000) == 5000, F("cyc offTime() 5000"));

    is (cyc.reverse(true) == true, F("cyc reverse"));
    is (cyc.onTime(1000) == 1000, F("cyc onTime() 1000"));
    is (cyc.offTime(1000) == 1000, F("cyc offTime() 1000"));

    delay(500);
    cyc.process();
    cycTest(LOW, F("On 1000 rev"));
    delay(1010);
    cyc.process();
    cycTest(HIGH, F("Off 1000 rev"));

    is (cyc.reverse(false) == false, F("cyc reverse false"));

    is (cyc.onTime(5000) == 5000, F("cyc onTime() 5000"));
    is (cyc.offTime(5000) == 5000, F("cyc offTime() 5000"));

}

void humidityRelayTests () {
    // HUMIDITY RELAY

    Serial.println(F("HUMIDITY RELAY"));

    is (hum.type() == RELAY_HUMID, F("Relay hum type"));
    is (hum.confSet == -1, F("hum confState default"));
    is (hum.confUp == -1, F("hum confUp default"));
    is (hum.confDown == -1, F("hum confDown default"));
    is (hum.confActions == 0, F("hum confActions default"));
    is (hum.num == -1, F("hum num default"));

    hum.automate(1);

    is (hum.type() == RELAY_HUMID, F("Relay hum type post auto"));
    is (hum.confSet == 7, F("hum confState auto"));
    is (hum.confUp == 3, F("hum confUp auto"));
    is (hum.confDown == 2, F("hum confDown auto"));
    is (hum.confActions == 2, F("hum confActions auto"));

    is (hum.reverse(true) == true, F("Hum rev true"));
    is (hum.reverse(false) == false, F("Hum rev false"));

    is (hum.factor() == 1, F("Hum factor default"));
    is (hum.factor(3) == 3, F("Hum factor 3"));
    is (hum.factor(1) == 1, F("Hum factor 1"));

    is (hum.onHum() == HUM, F("Hum onHum()"));
    is (hum.offHum() == HUM + 1, F("Hum offHum()"));

    humTest(LOW, F("Default"));

    hum.process(60.0);
    humTest(LOW, F("Hum 60.0"));

    hum.process(59.9);
    humTest(HIGH, F("Hum 59.9"));

    hum.process(61.5);
    humTest(LOW, F("Hum 61.5"));

    hum.process(61.1);
    humTest(LOW, F("Hum 61.1"));

    hum.process(60.0);
    humTest(LOW, F("Hum 60.0 rev"));

    hum.process(59.9);
    humTest(HIGH, F("Hum 59.9 rev"));

    hum.process(61.1);
    humTest(LOW, F("Hum 61.1 rev"));

    hum.process(61.1);
    humTest(LOW, F("Hum 61.1"));

    hum.process(58.1);
    humTest(HIGH, F("Hum 58.1"));

    hum.process(66);
    humTest(LOW, F("Hum 66.0"));

    hum.process(62.2);
    humTest(LOW, F("Hum 62.2"));

    hum.process(59.9);
    humTest(HIGH, F("Hum 59.9"));

    hum.process(76.9);
    humTest(LOW, F("Hum 76.9"));

    hum.process(62.2);
    humTest(LOW, F("Hum 62.2"));

    hum.process(59.9);
    humTest(HIGH, F("Hum 59.9"));

    hum.process(76.9);
    humTest(LOW, F("Hum 76.9"));

    hum.reverse(false);
}

void deHumidityRelayTests () {
    // DEHUMIDITY RELAY

    Serial.println(F("DE-HUMIDITY RELAY"));

    is (deHum.type() == RELAY_DEHUMID, F("Relay dehum type"));
    is (deHum.confSet == -1, F("dehum confState default"));
    is (deHum.confUp == -1, F("dehum confUp default"));
    is (deHum.confDown == -1, F("dehum confDown default"));
    is (deHum.confActions == 0, F("dehum confActions default"));
    is (deHum.num == -1, F("dehum num default"));

    deHum.automate(0);

    is (deHum.type() == RELAY_DEHUMID, F("Relay dehum type post auto"));
    is (deHum.confSet == 6, F("dehum confState auto"));
    is (deHum.confUp == 1, F("dehum confUp auto"));
    is (deHum.confDown == 0, F("dehum confDown auto"));
    is (deHum.confActions == 2, F("dehum confActions auto"));

    is (deHum.reverse(true) == true, F("Dehum rev true"));
    is (deHum.reverse(false) == false, F("Dehum rev false"));

    is (deHum.factor() == 1, F("Dehum factor default"));
    is (deHum.factor(3) == 3, F("Dehum factor 3"));
    is (deHum.factor(1) == 1, F("Dehum factor 1"));

    is (deHum.onHum() == HUM, F("Dehum onHum()"));
    is (deHum.offHum() == HUM - 1, F("Dehum offHum()"));

    deHumTest(LOW, F("Default"));

    deHum.process(60.0);
    deHumTest(LOW, F("Dehum 60.0"));

    deHum.process(59.9);
    deHumTest(LOW, F("Dehum 59.9"));

    deHum.process(61.5);
    deHumTest(HIGH, F("Dehum 61.5"));

    deHum.process(61.1);
    deHumTest(HIGH, F("Dehum 61.1"));

    deHum.reverse(true);

    deHum.process(60.0);
    deHumTest(HIGH, F("Dehum 60.0 rev"));

    deHum.process(59.9);
    deHumTest(HIGH, F("Dehum 59.9 rev"));

    deHum.process(61.1);
    deHumTest(LOW, F("Dehum 61.1 rev"));

    deHum.process(61.1);
    deHumTest(LOW, F("Dehum 61.1"));

    deHum.process(58.1);
    deHumTest(HIGH, F("Dehum 58.1"));

    deHum.process(66);
    deHumTest(LOW, F("Dehum 66.0"));

    deHum.process(62.2);
    deHumTest(LOW, F("Dehum 62.2"));

    deHum.reverse(false);

    deHum.process(58.9);
    deHumTest(LOW, F("Dehum 58.9"));

    deHum.process(76.9);
    deHumTest(HIGH, F("Dehum 76.9"));

    deHum.process(62.2);
    deHumTest(HIGH, F("Dehum 62.2"));

    deHum.process(58.4);
    deHumTest(LOW, F("Dehum 58.4"));

    deHum.process(76.9);
    deHumTest(HIGH, F("Dehum 76.9"));

    deHum.reverse(false);
}

void coolRelayTests () {
    // TEMP RELAY

    Serial.println(F("COOL RELAY"));

    is (cool.type() == RELAY_COOL, F("Relay cool type"));
    is (cool.confSet == -1, F("cool confState default"));
    is (cool.confUp == -1, F("cool confUp default"));
    is (cool.confDown == -1, F("cool confDown default"));
    is (cool.confActions == 0, F("cool confActions default"));
    is (cool.num == -1, F("cool num default"));

    cool.automate(1);

    is (cool.type() == RELAY_COOL, F("Relay cool type post auto"));
    is (cool.confSet == 7, F("cool confState auto"));
    is (cool.confUp == 3, F("cool confUp auto"));
    is (cool.confDown == 2, F("cool confDown auto"));
    is (cool.confActions == 2, F("cool confActions auto"));


    is (cool.reverse(true) == true, F("Cool rev true"));
    is (cool.reverse(false) == false, F("Cool rev false"));

    is (cool.factor() == 1, F("Cool factor default"));
    is (cool.factor(3) == 3, F("Cool factor 3"));
    is (cool.factor(1) == 1, F("Cool factor 1"));
    is (cool.onTemp() == TEMP, F("Cool onTemp()"));
    is (cool.offTemp() == TEMP - 1, F("Cool offTemp()"));

    coolTest(LOW, F("Default"));

    cool.process(78.0);
    coolTest(LOW, F("Cool 78.0"));

    cool.process(78.1);
    coolTest(HIGH, F("Cool 78.1"));

    cool.process(77.5);
    coolTest(HIGH, F("Cool 77.5"));

    cool.process(76.9);
    coolTest(LOW, F("Cool 76.9"));

    cool.reverse(true);

    cool.process(78.1);
    coolTest(LOW, F("Cool 78.1 rev"));

    cool.process(77.5);
    coolTest(LOW, F("Cool 77.5 rev"));

    cool.process(76.9);
    coolTest(HIGH, F("Cool 76.9 rev"));

    cool.reverse(false);

    cool.process(78.1);
    coolTest(HIGH, F("Cool 78.1"));

    cool.process(77.5);
    coolTest(HIGH, F("Cool 77.5"));

    cool.process(76.9);
    coolTest(LOW, F("Cool 76.9"));

    cool.reverse(false);
}

void heatRelayTests () {
    // TEMP RELAY

    Serial.println(F("HEAT RELAY"));

    is (heat.type() == RELAY_HEAT, F("Relay heat type"));
    is (heat.confSet == -1, F("heat confState default"));
    is (heat.confUp == -1, F("heat confUp default"));
    is (heat.confDown == -1, F("heat confDown default"));
    is (heat.confActions == 0, F("heat confActions default"));
    is (heat.num == -1, F("heat num default"));

    heat.automate(0);

    is (heat.type() == RELAY_HEAT, F("Relay heat type post auto"));
    is (heat.confSet == 6, F("heat confState auto"));
    is (heat.confUp == 1, F("heat confUp auto"));
    is (heat.confDown == 0, F("heat confDown auto"));
    is (heat.confActions == 2, F("heat confActions auto"));

    is (heat.reverse(true) == true, F("Heat rev true"));
    is (heat.reverse(false) == false, F("Heat rev false"));

    is (heat.factor() == 1, F("Heat factor default"));
    is (heat.factor(3) == 3, F("Heat factor 3"));
    is (heat.factor(1) == 1, F("Heat factor 1"));
    is (heat.onTemp() == TEMP, F("Heat onTemp()"));
    is (heat.offTemp() == TEMP + 1, F("Heat offTemp()"));

    heatTest(LOW, F("Default"));

    heat.process(78.0);
    heatTest(LOW, F("Heat 78.0"));

    heat.process(78.1);
    heatTest(LOW, F("Heat 78.1"));

    heat.process(77.5);
    heatTest(HIGH, F("Heat 77.5"));

    heat.process(76.9);
    heatTest(HIGH, F("Heat 76.9"));

    heat.reverse(true);

    heat.process(78.1);
    heatTest(HIGH, F("Heat 78.1 rev"));

    heat.process(77.5);
    heatTest(LOW, F("Heat 77.5 rev"));

    heat.process(76.9);
    heatTest(LOW, F("Heat 76.9 rev"));

    heat.reverse(false);

    heat.process(78.1);
    heatTest(LOW, F("Heat 78.1"));

    heat.process(77.5);
    heatTest(HIGH, F("Heat 77.5"));

    heat.process(76.9);
    heatTest(HIGH, F("Heat 76.9"));

    heat.reverse(false);
}
