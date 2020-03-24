#include "ArduinoRelay.h"
#include "CycleTimerRelay.h"
#include "TemperatureRelay.h"
#include "HumidityRelay.h"

#define TP          D7
#define HP          D6
#define CP          D5

#define TEMP        78
#define HUM         60
#define CYC_ON      1000
#define CYC_OFF     1000

TemperatureRelay temp(TP, TEMP);
HumidityRelay hum(HP, HUM);
CycleTimerRelay cyc(CP, CYC_ON, CYC_OFF);


void setup() {
    Serial.begin(9600);

    //is (temp.name("temp") == (char*)F("temp"), F("Temp name"));
    //is (hum.name("hum") == (char*)F("hum"), F("Hum name"));
    //is (cyc.name("cyc") == (char*)F("cyc"), F("Cyc name"));

    is (temp.reverse(true) == true, F("Temp rev true"));
    is (hum.reverse(true) == true, F("Hum rev true"));
    is (cyc.reverse(true) == true, F("Cyc rev true"));

    is (temp.reverse(false) == false, F("Temp rev false"));
    is (hum.reverse(false) == false, F("Hum rev false"));
    is (cyc.reverse(false) == false, F("Cyc rev false"));

    is (temp.mode() == MODE_COOL, F("Temp mode default"));
    is (temp.mode(MODE_HEAT) == MODE_HEAT, F("Temp mode heat"));
    is (temp.mode(MODE_COOL) == MODE_COOL, F("Temp mode cool"));
    is (temp.factor() == 1, F("Temp factor default"));
    is (temp.factor(3) == 3, F("Temp factor 3"));
    is (temp.factor(1) == 1, F("Temp factor 1"));
    is (temp.onTemp() == TEMP, F("Temp onTemp()"));
    is (temp.offTemp() == TEMP-1, F("Temp offTemp()"));
    is (temp.mode(MODE_HEAT) == MODE_HEAT, F("Temp mode heat"));
    is (temp.onTemp() == TEMP, F("Temp heat onTemp()"));
    is (temp.offTemp() == TEMP+1, F("Temp heat offTemp()"));
    is (temp.mode(MODE_COOL) == MODE_COOL, F("Temp mode cool"));

    is (hum.mode() == MODE_HUMIDIFY, F("Hum mode default"));
    is (hum.mode(MODE_DEHUMIDIFY) == MODE_DEHUMIDIFY, F("Hum mode dehum"));
    is (hum.mode(MODE_HUMIDIFY) == MODE_HUMIDIFY, F("Hum mode hum"));
    is (hum.factor() == 1, F("Hum factor default"));
    is (hum.factor(3) == 3, F("Hum factor 3"));
    is (hum.factor(1) == 1, F("Hum factor 1"));
    is (hum.onHum() == HUM, F("Hum onHum()"));
    is (hum.offHum() == HUM-1, F("Hum offHum()"));
    is (hum.mode(MODE_DEHUMIDIFY) == MODE_DEHUMIDIFY, F("Hum mode dehumidify"));
    is (hum.onHum() == HUM, F("Hum dehum onHum()"));
    is (hum.offHum() == HUM+1, F("Hum dehum offHum()"));
    is (hum.mode(MODE_HUMIDIFY) == MODE_HUMIDIFY, F("Hum mode humidify"));

    // TEMP RELAY

    tempTest(LOW, F("Default"));

    temp.process(78.0);
    tempTest(LOW, F("Temp 78.0"));

    temp.process(78.1);
    tempTest(HIGH, F("Temp 78.1"));

    temp.process(77.5);
    tempTest(HIGH, F("Temp 77.5"));

    temp.process(76.9);
    tempTest(LOW, F("Temp 76.9"));

    temp.reverse(true);

    temp.process(78.1);
    tempTest(LOW, F("Temp 78.1 rev"));

    temp.process(77.5);
    tempTest(LOW, F("Temp 77.5 rev"));

    temp.process(76.9);
    tempTest(HIGH, F("Temp 76.9 rev"));

    temp.reverse(false);

    temp.process(78.1);
    tempTest(HIGH, F("Temp 78.1"));

    temp.process(77.5);
    tempTest(HIGH, F("Temp 77.5"));

    temp.process(76.9);
    tempTest(LOW, F("Temp 76.9"));

    temp.mode(MODE_HEAT);
    is (temp.mode() == MODE_HEAT, F("Temp mode heat"));

    temp.process(77.9);
    tempTest(HIGH, F("Heat 77.9"));

    temp.process(81.5);
    tempTest(LOW, F("Heat 81.5"));

    temp.process(76.9);
    tempTest(HIGH, F("Heat 76.9"));

    temp.reverse(true);

    temp.process(77.9);
    tempTest(LOW, F("Heat 77.9 rev"));

    temp.process(81.5);
    tempTest(HIGH, F("Heat 81.5 rev"));

    temp.process(76.9);
    tempTest(LOW, F("Heat 76.9 rev"));

    temp.reverse(false);

    // HUMIDITY RELAY

    humTest(LOW, F("Default"));

    hum.process(60.0);
    humTest(LOW, F("Hum 60.0"));

    hum.process(59.9);
    humTest(HIGH, F("Hum 59.9"));

    hum.process(60.5);
    humTest(LOW, F("Hum 60.5"));

    hum.process(61.1);
    humTest(LOW, F("Hum 61.1"));

    hum.reverse(true);

    hum.process(60.0);
    humTest(HIGH, F("Hum 60.0 rev"));

    hum.process(59.9);
    humTest(LOW, F("Hum 59.9 rev"));

    hum.process(61.1);
    humTest(HIGH, F("Hum 60.5 rev"));

    hum.reverse(false);

    hum.process(61.1);
    humTest(LOW, F("Hum 61.1"));

    hum.process(58.1);
    humTest(HIGH, F("Hum 58.1"));

    hum.process(66);
    humTest(LOW, F("Hum 66.0"));

    hum.mode(MODE_DEHUMIDIFY);
    is (hum.mode() == MODE_DEHUMIDIFY, F("Hum mode dehum"));

    hum.process(62.2);
    humTest(HIGH, F("Dehum 62.2"));

    hum.process(59.9);
    humTest(LOW, F("Dehum 59.9"));

    hum.process(76.9);
    humTest(HIGH, F("Dehum 76.9"));

    hum.reverse(true);

    hum.process(62.2);
    humTest(LOW, F("Dehum 62.2 rev"));

    hum.process(59.9);
    humTest(HIGH, F("Dehum 59.9 rev"));

    hum.process(76.9);
    humTest(LOW, F("Dehum 76.9 rev"));

    hum.reverse(false);

}

void humTest (uint8_t state, const __FlashStringHelper* msg) {
    is (hum.state() == state, msg);
    is (digitalRead(HP) == state, msg);
}

void tempTest (uint8_t state, const __FlashStringHelper* msg) {
    is (temp.state() == state, msg);
    is (digitalRead(TP) == state, msg);
}

void loop() {
}

void is (bool arg, const __FlashStringHelper* msg) {
    if (! arg) {
        Serial.print(F("FAIL: "));
        Serial.println(msg);
    }
//    arg ? Serial.print(F("OK:   ")) : Serial.println(F("FAIL: "));
//    Serial.println(msg);
}
