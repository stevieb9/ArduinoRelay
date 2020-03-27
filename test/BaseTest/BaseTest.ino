#include <RelayOTA.h>
#include <RelayWifiConfig.h>
#include <CycleTimerRelay.h>

#define CYC_PIN     D6
#define CYC_ON      2000
#define CYC_OFF     3500

RelayOTA ota;
CycleTimerRelay ctr(CYC_PIN, CYC_ON, CYC_OFF);

void setup() {
    Serial.begin(9600);
    relayConfig(false);
    ota.begin();
}

void loop() {
    process();
}

void process () {
    ota.handle();
    ctr.process();
}
