#include <Arduino.h>

// #include "VisualFeedbackService.h"
#include "WifiService.h"
#include "TimeService.h"
#include "UltrasonicSensorService.h"
// #include "HTTPService.h"
#include "SampleSenderService.h"
#include <FirebaseESP32.h>

VisualFeedbackService feedback = VisualFeedbackService();
UltrasonicSensorService sensor = UltrasonicSensorService();
SampleSenderService sender = SampleSenderService();

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
    feedback.FadeInOut(1000);

    String timestring = GetTime();

    Serial.print("Time: ");
    Serial.println(timestring);


    float sample = sensor.GetDistance();
    Serial.print("Distance: ");
    Serial.print(sample);
    Serial.println(" cm");

    sender.SendSample(sample);
}