// #include "BLEService.h"
#include "BluetoothService.h"
#include "PreferencesService.h"

#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>

class BLE_WiFiCredentialsCallback : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        std::string rxValue = pCharacteristic->getValue();

        if (rxValue.length() > 0)
        {
            Serial.println("Reciving credentials");

            String raw = "";

            for (int i = 0; i < rxValue.length(); i++)
                raw += rxValue[i];

            StaticJsonDocument<150> doc;

            DeserializationError error = deserializeJson(doc, raw);

            if (error)
            {
                Serial.print("deserializeJson() failed: ");
                Serial.println(error.c_str());
                return;
            }

            String SSID = doc["SSID"];
            String Password = doc["Password"];

            Preferences prefs;
            prefs.begin("credentials", false);

            prefs.putString("STA_SSID", SSID);
            prefs.putString("STA_PASSWORD", Password);

            // Serial.println(prefs.getString("STA_SSID"));
            // Serial.println(prefs.getString("STA_PASSWORD"));

            // Serial.println(raw);
            Serial.println("Saving in Preferences");

            prefs.end();
            delay(2000);

            ESP.restart();
        }
    }
};

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
    default:
        break;
    }
}

class WifiService
{
private:
public:
    /// @brief Inicia el servicio de red y se conecta a una conexion WIFI con las credenciales guardadas en las preferencias del dispositivo.
    /// Si no hay credenciales registradas o validas lanzara un servicio bluetooth para obtener las credenciales desde un callback
    WifiService()
    {
        Serial.println("Looking for wifi credentials");

        PreferencesService prefsService = PreferencesService("credentials", true);

        if (prefsService.GetString("STA_SSID") == "")
        {
            BluetoothService blService = BluetoothService(BLE_ECOZY_DEVICE_UUID);
            blService.CreateCharacteristic(WIFI_CHARACTERISTIC_UUID, BLE_SERVICE_UUID, BLECharacteristic::PROPERTY_WRITE, new BLE_WiFiCredentialsCallback());
            blService.StartService();

            delay(1000);

            prefsService.Close();

            while (true)
            {
                Serial.print(".");
                delay(1000);
            }
        }

        Serial.println("Credentials found");

        String const sta_ssid = prefsService.GetString("STA_SSID");
        String const sta_password = prefsService.GetString("STA_PASSWORD");

        delay(100);

        //Serial.println("Wifi credentials: SSID=" + sta_ssid + " PSWD=" + sta_password);
        Serial.print("Wifi credentials: SSID=");
        Serial.print(sta_ssid);
        Serial.print(" PSWD=");
        Serial.println(sta_password);

        WiFi.setHostname("XIAO_C3");
        WiFi.begin(sta_ssid, sta_password);

        WiFi.onEvent(WiFiEvent);
        WiFiEventId_t eventID = WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info)
                                             {
        Serial.print("WiFi lost connection. Reason: ");
        Serial.println(info.wifi_sta_disconnected.reason); },
                                             WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

        prefsService.Close();
    }
};