
#include <Arduino.h>
#include <WiFi.h>
#include "time.h"
#include "sntp.h"

const char *ntpServer1 = "0.mx.pool.ntp.org";
const char *ntpServer2 = "3.north-america.pool.ntp.org";

/// @brief Obtiene la hora UTC actual 
/// @return Devuelve la hora UTC
String GetTime()
{
    struct tm timeinfo;
    
    if (!getLocalTime(&timeinfo)) { return "No time available (yet)"; }

    char str[50];

    strftime(str, sizeof str, "%A, %B %d %Y %H:%M:%S", &timeinfo); 

    return (str);
}

/// @brief Imprime en consola la hora UTC
void printTime()
{
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        Serial.println("No time available (yet)");
        return;
    }

    Serial.println(GetTime());
}

void timeavailable(struct timeval *t)
{
    Serial.println("Got time adjustment from NTP!");
    printTime();
}

/// @brief Inicia el servicio de Tiempo, para obtener la hora y fecha usando el protocolo NTP
void StartTimeService()
{
    sntp_set_time_sync_notification_cb(timeavailable);
    sntp_servermode_dhcp(1);

    configTime(0, 0, ntpServer1, ntpServer2);
}