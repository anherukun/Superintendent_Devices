#include <Arduino.h>

// #include "VisualFeedbackService.h"
#include "WifiService.h"
#include "TimeService.h"

#include <FirebaseESP32.h>

VisualFeedbackService feedback = VisualFeedbackService();

const byte FEEDBACK_LED = 9;

void setup()
{
    Serial.begin(115200);
    delay(500);
    Serial.println("Starting systems...");

    WifiService wifiService = WifiService();

    feedback.Off();
    StartTimeService();
}

void loop()
{
    feedback.FadeInOut(750);

    String timestring = GetTime();

    Serial.print("Time: ");
    Serial.println(timestring);
}