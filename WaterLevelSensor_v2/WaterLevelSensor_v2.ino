#include <Arduino.h>

// #include "VisualFeedbackService.h"
#include "WifiService.h"
#include "TimeService.h"
#include "UltrasonicSensorService.h"
#include <FirebaseESP32.h>

VisualFeedbackService feedback = VisualFeedbackService();
UltrasonicSensorService sensor = UltrasonicSensorService();

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

    Serial.print("Distance: ");
    Serial.print(sensor.GetDistance());
    Serial.println(" cm");
}