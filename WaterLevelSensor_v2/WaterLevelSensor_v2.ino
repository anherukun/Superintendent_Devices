#include <Arduino.h>

#include "WifiService.h"
#include "TimeService.h"

#include <FirebaseESP32.h>

// const char *SSID = DEVICE_NAME;
// const char *PASSWORD = DEVICE_PASSWORD;

void setup()
{
    Serial.begin(115200);
    
    WifiService wifiService = WifiService();
    // StartWifiService();
    //StartWifiService(SSID, PASSWORD);

    StartTimeService();
}

void loop()
{
    delay(1000);

    String timestring = GetTime();
    Serial.println("Time: " + timestring);
}