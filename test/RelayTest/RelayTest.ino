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
    Serial.print(F("\n"));

    cycleRelayTests();
    coolRelayTests();
    heatRelayTests();
    humidityRelayTests();
    deHumidityRelayTests();

    /*
    switchRelayTests();
    */
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

    is (hum.reverse(true) == true, F("Hum rev true"));
    is (hum.reverse(false) == false, F("Hum rev false"));

    is (hum.factor() == 1, F("Hum factor default"));
    is (hum.factor(3) == 3, F("Hum factor 3"));
    is (hum.factor(1) == 1, F("Hum factor 1"));

    is (hum.onHum() == HUM, F("Hum onHum()"));
    is (hum.offHum() == HUM+1, F("Hum offHum()"));

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

    is (deHum.reverse(true) == true, F("Dehum rev true"));
    is (deHum.reverse(false) == false, F("Dehum rev false"));

    is (deHum.factor() == 1, F("Dehum factor default"));
    is (deHum.factor(3) == 3, F("Dehum factor 3"));
    is (deHum.factor(1) == 1, F("Dehum factor 1"));

    is (deHum.onHum() == HUM, F("Dehum onHum()"));
    is (deHum.offHum() == HUM-1, F("Dehum offHum()"));

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

    is (cool.reverse(true) == true, F("Cool rev true"));
    is (cool.reverse(false) == false, F("Cool rev false"));

    is (cool.factor() == 1, F("Cool factor default"));
    is (cool.factor(3) == 3, F("Cool factor 3"));
    is (cool.factor(1) == 1, F("Cool factor 1"));
    is (cool.onTemp() == TEMP, F("Cool onTemp()"));
    is (cool.offTemp() == TEMP-1, F("Cool offTemp()"));

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

    is (heat.reverse(true) == true, F("Heat rev true"));
    is (heat.reverse(false) == false, F("Heat rev false"));

    is (heat.factor() == 1, F("Heat factor default"));
    is (heat.factor(3) == 3, F("Heat factor 3"));
    is (heat.factor(1) == 1, F("Heat factor 1"));
    is (heat.onTemp() == TEMP, F("Heat onTemp()"));
    is (heat.offTemp() == TEMP+1, F("Heat offTemp()"));

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
