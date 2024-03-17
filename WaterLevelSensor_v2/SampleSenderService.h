#include "HTTPService.h"
#include <ArduinoJson.h>

class SampleSenderService
{
private:
    HTTPService HTTP = HTTPService();
    const char *CONFIDENCE_TOKEN = "81A511E3-5084-431E-9B5A-907C4AFA6527";

public:
    SampleSenderService() { }

    void SendSample(float value)
    {
        StaticJsonDocument<150> doc;
        doc["Device_ID"] = WiFi.macAddress();
        doc["Value"] = value;

        String payload;
        serializeJson(doc, payload);

        Serial.println("Sending to server: ");
        Serial.println(payload);

        HTTP.SendRequestHTTP("api/samples/" + WiFi.macAddress() + "?token=" + CONFIDENCE_TOKEN, payload);
    }
};