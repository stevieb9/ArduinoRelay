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

    is(temp.name("temp") == F("temp"), "Temp name");
    is(hum.name("hum") == F("hum"), "Hum name");
    is(cyc.name("cyc") == F("cyc"), "Cyc name");

    is(temp.reverse(true) == true, "Temp rev true"
    is(hum.reverse(true) == true, "Hum rev true"
    is(cyc.reverse(true) == true, "Cyc rev true"

    is(temp.reverse(false) == false, "Temp rev false"
    is(hum.reverse(false) == false, "Hum rev false"
    is(cyc.reverse(false) == false, "Cyc rev false"

    is(temp.mode() == MODE_COOL, "Temp mode default");
    is(temp.mode(MODE_HEAT) == MODE_HEAT, "Temp mode heat");
    is(temp.mode(MODE_COOL) == MODE_COOL, "Temp mode cool");
    is(temp.factor() == 1, "Temp factor default");
    is(temp.factor(3) == 3, "Temp factor 3");
    is(temp.factor(1) == 1, "Temp factor 1");

    is(hum.mode() == MODE_HUMIDIFY, "Hum mode default");
    is(hum.mode(MODE_DEHUMIDIFY) == MODE_DEHUMIDIFY, "Hum mode dehum");
    is(hum.mode(MODE_HUMIDIFY) == MODE_HUMIDIFY, "Hum mode hum");
    is(hum.factor() == 1, "Hum factor default");
    is(hum.factor(3) == 3, "Hum factor 3");
    is(hum.factor(1) == 1, "Hum factor 1");
}

void loop() {
}

void is (bool arg, const char* msg) {
    arg ? Serial.print(F("OK:   ")) : Serial.println(F("FAIL: "));
    Serial.println(msg);
}
