#include "GPIO_Definitions.h"
#include <ESPAsyncWebSrv.h>

void HelloWorldRequest(AsyncWebServerRequest *request)
{
    request->send(200, "text/plain", "hello world");
}

void ShootRequest(AsyncWebServerRequest *request)
{   
    digitalWrite(GPIO_FOCUS, HIGH);
    delay(2);
    digitalWrite(GPIO_SHUTTER, HIGH);
    delay(2000);
    digitalWrite(GPIO_SHUTTER, LOW);
    digitalWrite(GPIO_FOCUS, LOW);

    delay(100);
    request->send(200, "text/plain", "ok");
}
