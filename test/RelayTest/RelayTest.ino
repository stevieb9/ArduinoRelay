#include "ArduinoRelay.h"
#include "CycleTimerRelay.h"
#include "TemperatureRelay.h"
#include "HumidityRelay.h"

#define PIN   2

#define TEMP        70
#define HUM         60
#define CYC_ON      1000
#define CYC_OFF     1000

TemperatureRelay temp(PIN, TEMP);
HumidityRelay hum(PIN, HUM);
CycleTimerRelay cyc(PIN, CYC_ON, CYC_OFF);

void setup() {
    Serial.begin(9600);

    is (temp.name("temp") == (char*)F("temp"), F("Temp name"));
    is (hum.name("hum") == (char*)F("hum"), F("Hum name"));
    is (cyc.name("cyc") == (char*)F("cyc"), F("Cyc name"));

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

    is (hum.mode() == MODE_HUMIDIFY, F("Hum mode default"));
    is (hum.mode(MODE_DEHUMIDIFY) == MODE_DEHUMIDIFY, F("Hum mode dehum"));
    is (hum.mode(MODE_HUMIDIFY) == MODE_HUMIDIFY, F("Hum mode hum"));
    is (hum.factor() == 1, F("Hum factor default"));
    is (hum.factor(3) == 3, F("Hum factor 3"));
    is (hum.factor(1) == 1, F("Hum factor 1"));
}

void loop() {
}

void is (bool arg, const __FlashStringHelper* msg) {
    arg ? Serial.print(F("OK:   ")) : Serial.println(F("FAIL: "));
    Serial.println(msg);
}
