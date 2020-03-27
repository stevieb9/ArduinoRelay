#include <RelayOTA.h>
#include <RelayWifiConfig.h>
#include <CycleTimerRelay.h>

#define CYC_PIN     12
#define CYC_ON      5000
#define CYC_OFF     1000

RelayOTA ota;
CycleTimerRelay ctr(CYC_PIN, CYC_ON, CYC_OFF);

void setup() {
    Serial.begin(9600);
    relayConfig(false);
    ota.begin();
}

void loop() {
    ota.handle();
    ctr.process();
}