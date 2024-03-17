#include <HTTPClient.h>

class HTTPService
{
private:
    const String URL = "https://backend-ecozy.anheru.com.mx/";

public:
    HTTPService() { }

    void SendRequestHTTP(String path, String body)
    {
        HTTPClient http;
        http.begin(URL + path);
        http.addHeader("Content-Type", "application/json");

        int response = http.POST(body);

        if (response > 0)
        {
            String result = http.getString();
            Serial.println(response);
            Serial.println(result);
        }
        else
        {
            Serial.print("Error: ");
            Serial.println(response);
        }

        http.end();
    }
};