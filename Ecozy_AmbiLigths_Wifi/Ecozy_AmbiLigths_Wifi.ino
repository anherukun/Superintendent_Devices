// #include <WiFi.h>
#include <WiFiUdp.h>
// #include <IotWebConf.h>
#include <WiFiManager.h>
#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 32
#define DATA_PIN 4
#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];

WiFiManager wifiManager;

boolean connected = false;

IPAddress localIP; // Obtener IP local
IPAddress subnet;  // Obtener máscara de subred
IPAddress broadcastIP;

const int udpPort = 3333;
WiFiUDP udp;

void setup()
{
    // WiFi.begin();
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.onEvent(WiFiEvent);
    delay(3000);

    if (!WiFi.isConnected())
    {
        wifiManager.setTitle("Ecozy AmbiLigths");
        wifiManager.setDarkMode(true);
        wifiManager.autoConnect("Ecozy-AmbiLigths");
        
        // ESP.restart();
    }

    Serial.println("WiFi Conectado");

    localIP = WiFi.localIP();   // Obtener IP local
    subnet = WiFi.subnetMask(); // Obtener máscara de subred
    broadcastIP = localIP | ~subnet;
    
    Serial.println(localIP);
    Serial.println(subnet);
    Serial.println(broadcastIP);
    

    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS); // GRB ordering is typical
    FastLED.clearData();
    FastLED.show();
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
        udp.begin(WiFi.localIP(), udpPort);
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
    delay(1000);
    Serial.print(".");

    if (connected)
    {
        String p = "{\"IP\" = \"" + localIP.toString() + "\" }";
        // Send a packet
        udp.beginPacket(broadcastIP, udpPort);
        // udp.printf("Seconds since boot: %lu", millis() / 1000);
        udp.print(p);
        udp.endPacket();

        // String udpString = udp.readString();
        // Serial.println(udpString);
    }
    // Turn the LED on, then pause
    // for (size_t i = 0; i < NUM_LEDS; i++)
    // {
    //     leds[i] = CRGB::Red;
    //     FastLED.show();
    //     delay(50);
    // }

    // for (size_t i = 0; i < NUM_LEDS; i++)
    // {
    //     leds[i] = CRGB::Black;
    //     FastLED.show();
    //     delay(50);
    // }
}
