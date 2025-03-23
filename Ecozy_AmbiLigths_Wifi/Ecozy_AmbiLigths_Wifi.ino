#include <WiFiUdp.h>
#include <WiFiManager.h>
#include <FastLED.h>

#define NUM_LEDS 48
#define DATA_PIN 4
#define CLOCK_PIN 13
#define LED_TYPE WS2812B

// Define the array of leds
CRGB leds[NUM_LEDS];

WiFiManager wifiManager;

boolean connected = false;

IPAddress localIP; // Obtener IP local
IPAddress subnet;  // Obtener máscara de subred
IPAddress broadcastIP;

// const int udpPort = 3333;
#define TX_UDP_PORT 1996
#define RX_UDP_PORT 1999
WiFiUDP udp;

byte DEVICE_FAMILY = 0; // AmbientLigths
byte DEVICE_MODEL = 0;  // AmbientLigths_Backligth_Lite

void setup()
{
    FastLED.addLeds<LED_TYPE, DATA_PIN, GRB>(leds, NUM_LEDS); // GRB ordering is typical
    FastLED.clearData();
    FastLED.show();

    Serial.begin(115200);
    delay(3000);

    // wifiManager.resetSettings();

    WiFi.onEvent(WiFiEvent);
    wifiManager.setTitle("Ecozy AmbiLigths");
    wifiManager.setDarkMode(true);
    wifiManager.autoConnect("Ecozy-AmbiLigths");

    Serial.println("WiFi Conectado");

    localIP = WiFi.localIP();   // Obtener IP local
    subnet = WiFi.subnetMask(); // Obtener máscara de subred
    broadcastIP = localIP | ~subnet;

    Serial.println(localIP);
    Serial.println(subnet);
    Serial.println(broadcastIP);
}

void WiFiEvent(WiFiEvent_t event)
{
    switch (event)
    {
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
        // When connected set
        Serial.print("WiFi connected! IP address: ");
        Serial.println(WiFi.localIP());
        // initializes the UDP state
        // This initializes the transfer buffer
        udp.setTimeout(66);
        udp.begin(localIP, RX_UDP_PORT);
        Serial.print("UDP Timeout: ");
        Serial.println(udp.getTimeout());
        connected = true;
        break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
        Serial.println("WiFi lost connection");
        connected = false;
        break;
    default:
        break;
    }
}

void loop()
{
    if (!connected)
    {
        Serial.print("..WIFI_DISCONNECTED..");
        delay(1000);
        return;
    }

    else
    {
        // {"IP":"0.0.0.0", "MACAddress":"00-00-00-00-00-00", "ConnectionType":0,  "DeviceInformation":{"DeviceFamily":0, "DeviceModel":0}}
        String p = "{\"IP\":\"" + localIP.toString() + "\", \"MACAddress\":\"" + WiFi.macAddress() + "\", \"ConnectionType\":0, \"DeviceInformation\":{\"DeviceFamily\":" + DEVICE_FAMILY + ", \"DeviceModel\":" + DEVICE_MODEL + "}}";
        // Send a packet
        udp.beginPacket(broadcastIP, TX_UDP_PORT);
        udp.print(p);
        udp.endPacket();

        // delay(1);
    }

    delay(2);

    int packetSize = udp.parsePacket();

    if (packetSize > 0)
    {
        // Serial.print("Incomming message, ");
        // Serial.println("Size: " + packetSize);

        byte buffer[packetSize];

        udp.readBytes(buffer, packetSize);

        // for (size_t i = 0; i < packetSize; i++)
        // {
        //     Serial.println((char)buffer[i]);
        // }

        for (size_t i = 0; i < packetSize; i += 4)
        {
            byte pByte = buffer[i + 0];
            byte rByte = buffer[i + 1];
            byte gByte = buffer[i + 2];
            byte bByte = buffer[i + 3];

            leds[pByte] = CRGB(rByte, gByte, bByte);
        }

        FastLED.show();

        // byte pByte = buffer[0];
        // byte rByte = buffer[1];
        // byte gByte = buffer[2];
        // byte bByte = buffer[3];
        // leds[pByte] = CRGB(rByte, gByte, bByte);
        // FastLED.show();
        // return;
    }

    // Serial.print(".");
    // Serial.flush();
    
    udp.flush();

    delay(2);
}
