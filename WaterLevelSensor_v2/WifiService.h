#include <Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>

WiFiManager wifiManager;

void WiFiEvent(WiFiEvent_t event)
{
    Serial.printf("[WiFi-event] event: %d\n", event);

    switch (event)
    {
    case ARDUINO_EVENT_WIFI_READY:
        Serial.println("WiFi interface ready");
        break;
    case ARDUINO_EVENT_WIFI_SCAN_DONE:
        Serial.println("Completed scan for access points");
        break;
    case ARDUINO_EVENT_WIFI_STA_START:
        Serial.println("WiFi client started");
        break;
    case ARDUINO_EVENT_WIFI_STA_STOP:
        Serial.println("WiFi clients stopped");
        break;
    case ARDUINO_EVENT_WIFI_STA_CONNECTED:
        Serial.println("Connected to access point");
        break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
        Serial.println("Disconnected from WiFi access point");
        break;
    case ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE:
        Serial.println("Authentication mode of access point has changed");
        break;
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
        Serial.print("Obtained IP address: ");
        Serial.println(WiFi.localIP());
        break;
    case ARDUINO_EVENT_WIFI_STA_LOST_IP:
        Serial.println("Lost IP address and IP address is reset to 0");
        break;
    case ARDUINO_EVENT_WIFI_AP_START:
        Serial.println("WiFi access point started");
        break;
    case ARDUINO_EVENT_WIFI_AP_STOP:
        Serial.println("WiFi access point  stopped");
        break;
    case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
        Serial.println("Client connected");
        break;
    case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
        Serial.println("Client disconnected");
        break;
    case ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED:
        Serial.println("Assigned IP address to client");
        break;
    case ARDUINO_EVENT_WIFI_AP_PROBEREQRECVED:
        Serial.println("Received probe request");
        break;
    case ARDUINO_EVENT_WIFI_AP_GOT_IP6:
        Serial.println("AP IPv6 is preferred");
        break;
    case ARDUINO_EVENT_WIFI_STA_GOT_IP6:
        Serial.println("STA IPv6 is preferred");
        break;
    case ARDUINO_EVENT_ETH_GOT_IP:
        Serial.println("Obtained IP address");
        break;
    default:
        break;
    }
}

void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(IPAddress(info.got_ip.ip_info.ip.addr));
}

/// @brief Inicia el servicio de wifi, lanzara un sitio web captivo para configurar la conexcion al access point al que se conectara
/// @param ssid Nombre de la red inalambrica que iniciara temporalmente
/// @param password Contraseña de la red inalambrica
void StartWifiService(char const *ssid, char const *password)
{
    if (wifiManager.autoConnect(ssid, password))
    {
        Serial.println("✅  Wifi connected");
    }
    else
    {
        Serial.print("⏳  Wating for connection or setup");
        while (!WiFi.isConnected())
        {
            Serial.print(".");
            delay(1000);
        }
        Serial.println("✅  Wifi connected");
    }

    WiFi.onEvent(WiFiEvent);
    WiFi.onEvent(WiFiGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
    WiFiEventId_t eventID = WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info)
                                         {
        Serial.print("WiFi lost connection. Reason: ");
        Serial.println(info.wifi_sta_disconnected.reason); },
                                         WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
}