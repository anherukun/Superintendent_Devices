#include <Arduino.h>

#include "WifiService.h"
#include "TimeService.h"

#include <FirebaseESP32.h>

// const char *SSID = DEVICE_NAME;
// const char *PASSWORD = DEVICE_PASSWORD;

const byte FEEDBACK_LED = 9;

void setup()
{
    Serial.begin(115200);
    delay(500);
    Serial.println("Starting systems...");

    WifiService wifiService = WifiService();

    StartTimeService();
}

void loop()
{
    delay(1000);

    String timestring = GetTime();
    Serial.print("Time: ");
    Serial.println(timestring);
}