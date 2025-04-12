/*********************************************************************
  This is an example for our Monochrome OLEDs based on SH110X drivers

  This example is for a 128x64 size display using I2C to communicate
  3 pins are required to interface (2 I2C and one reset)

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada  for Adafruit Industries.
  BSD license, check license.txt for more information
  All text above, and the splash screen must be included in any redistribution

  i2c SH1106 modified by Rupert Hirst  12/09/21
  *********************************************************************/

// Eliminar del directorio C:\Users\angel\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.14\tools\sdk\esp32c3\include
// la carpeta qrcode
#include "qrcode.h"

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#include <uri/UriBraces.h>
#include <uri/UriRegex.h>

#define GPIO_POWER 2
#define GPIO_FOCUS 1
#define GPIO_SHUTTER 0

#define GPIO_SDA 8
#define GPIO_SCL 9

/* Uncomment the initialize the I2C address , uncomment only one, If you get a totally blank screen try the other*/
#define i2c_Address 0x3c // initialize with the I2C addr 0x3C Typically eBay OLED's
// #define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1	 //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// #define WIFI_QR "WIFI:S:MiRedWiFi;T:WPA;P:contraseña123;H:false;;"
#define WIFI_PASSWORD "19961999"

WebServer server(80);

void setup()
{
	pinMode(GPIO_SHUTTER, OUTPUT);
	pinMode(GPIO_FOCUS, OUTPUT);
	pinMode(GPIO_POWER, OUTPUT);

	digitalWrite(GPIO_SHUTTER, LOW);
	digitalWrite(GPIO_FOCUS, LOW);
	digitalWrite(GPIO_POWER, LOW);

	Serial.begin(115200);

	// WiFi.mode(WIFI_STA);
	WiFi.softAP("EZ_" + WiFi.macAddress().substring(12, 17), WIFI_PASSWORD, 1, 0, 1);
	WiFi.onEvent(WiFiEvent);

	Wire.begin(GPIO_SDA, GPIO_SCL);
	// Show image buffer on the display hardware.
	// Since the buffer is intialized with an Adafruit splashscreen
	// internally, this will display the splashscreen.

	delay(250);						  // wait for the OLED to power up
	display.begin(i2c_Address, true); // Address 0x3C default
	// display.setContrast (0); // dim display

	display.display();
	delay(50);

	// Clear the buffer.
	display.clearDisplay();
	display.display();

	server.on(F("/"), [](){
		server.send(200, "text/plain", "hello world");

		display.setCursor(SCREEN_WIDTH - (11 * 6) / 2, (SCREEN_HEIGHT - 8) / 2);
		display.setTextSize(1);
		display.println("Hola mundo");
	});

	server.begin();

	GenerateWifiQR(WiFi.softAPSSID(), WIFI_PASSWORD);
}

void loop()
{
	// digitalWrite(GPIO_FOCUS, HIGH);
	// delay(2);
	// digitalWrite(GPIO_SHUTTER, HIGH);
	// delay(2000);
	// digitalWrite(GPIO_SHUTTER, LOW);
	// digitalWrite(GPIO_FOCUS, LOW);

	// delay(100);
}

// Callback para manejo de eventos de WiFi
void WiFiEvent(WiFiEvent_t event)
{
	Serial.println("Evento: " + event);

	switch (event)
	{
	case WIFI_EVENT_AP_STACONNECTED:
		Serial.println("¡Un cliente se ha conectado al AP!");

		display.clearDisplay();
		display.display();

		ShowInfoBar();
		ShowTaskBar();

		break;

	case WIFI_EVENT_AP_STADISCONNECTED:
		Serial.println("Un cliente se ha desconectado del AP.");
		GenerateWifiQR(WiFi.softAPSSID(), WIFI_PASSWORD);

		break;
	default:
		break;
	}
}

void ShowTaskBar()
{
	display.fillRect(0, 8, 128, 1, SH110X_WHITE);
	display.setCursor(0, 0);
	display.setTextSize(1);
	display.setTextColor(SH110X_BLACK, SH110X_WHITE);
	display.print("Status: ");
	display.setTextColor(SH110X_WHITE, SH110X_BLACK);
	display.print("Idle");
	display.display();
}

void ShowInfoBar()
{
	String ip_string = WiFi.softAPIP().toString();
	Serial.println(ip_string);

	display.fillRect(0, 55, 128, 1, SH110X_WHITE);
	display.setCursor(128 - (ip_string.length() * 6), 56);
	display.setTextSize(1);
	display.setTextColor(SH110X_BLACK, SH110X_WHITE);
	display.print(ip_string);
	display.display();
}

void GenerateWifiQR(String ssid, String password)
{
	//  "WIFI:S:MiRedWiFi;T:WPA;P:contraseña123;H:false;;"
	delay(100);
	String wifiData = "WIFI:S:" + ssid + ";T:WPA;P:" + password + ";H:false;;";
	const char *ptrWifiData = wifiData.c_str();

	Serial.println(ptrWifiData);
	delay(100);

	QRCode qrcode;
	uint8_t qrcodeData[qrcode_getBufferSize(3)];
	qrcode_initText(&qrcode, qrcodeData, 3, 0, ptrWifiData);

	// Clear the display
	display.clearDisplay();

	display.setTextSize(1);
	display.setCursor(0, 0);
	// display.setTextColor(SH110X_BLACK, SH110X_WHITE); // 'inverted' text
	display.println("Escanea para conectar");

	// Calculate the scale factor
	int scale = min(SCREEN_WIDTH / qrcode.size, (SCREEN_HEIGHT - 6) / qrcode.size);

	// Calculate horizontal shift
	int shiftX = (SCREEN_WIDTH - qrcode.size * scale) / 2;

	// Calculate horizontal shift
	int shiftY = ((SCREEN_HEIGHT + 6) - qrcode.size * scale) / 2;

	// Draw the QR code on the display
	for (uint8_t y = 0; y < qrcode.size; y++)
	{
		for (uint8_t x = 0; x < qrcode.size; x++)
		{
			if (qrcode_getModule(&qrcode, x, y))
			{
				display.fillRect(shiftX + x * scale, shiftY + y * scale, scale, scale, SH110X_WHITE);
			}
		}
	}

	// Update the display
	display.display();
}