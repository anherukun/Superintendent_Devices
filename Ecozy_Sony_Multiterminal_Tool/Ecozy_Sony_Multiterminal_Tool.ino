// Eliminar del directorio C:\Users\angel\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.14\tools\sdk\esp32c3\include
// la carpeta qrcode
// #include "qrcode.h"
// #include "UIManager.h"
#include "ShooterManager.h"
#include "WebServerRequests.h"
#include "GPIO_Definitions.h"

#include <WiFi.h>
#include <WiFiClient.h>
// #include <WebServer.h>
// #include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
// #include <ESPAsyncWebSrv.h>
#include <ESPmDNS.h>

// #include <uri/UriBraces.h>
// #include <uri/UriRegex.h>

// #define GPIO_POWER 2
// #define GPIO_FOCUS 1
// #define GPIO_SHUTTER 0

// #define GPIO_SDA 8
// #define GPIO_SCL 9

// /* Uncomment the initialize the I2C address , uncomment only one, If you get a totally blank screen try the other*/
// #define i2c_Address 0x3c // initialize with the I2C addr 0x3C Typically eBay OLED's
// // #define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's

// #define SCREEN_WIDTH 128 // OLED display width, in pixels
// #define SCREEN_HEIGHT 64 // OLED display height, in pixels
// #define OLED_RESET -1	 //   QT-PY / XIAO
// Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// #define WIFI_QR "WIFI:S:MiRedWiFi;T:WPA;P:contraseña123;H:false;;"
#define WIFI_PASSWORD "19961999"

AsyncWebServer server(80);
UIManager ui = UIManager();;
ShooterManager shooter(ui);

bool ClientConnected = false;

void setup()
{
	// pinMode(GPIO_SHUTTER, OUTPUT);
	// pinMode(GPIO_FOCUS, OUTPUT);
	// pinMode(GPIO_POWER, OUTPUT);

	// digitalWrite(GPIO_SHUTTER, LOW);
	// digitalWrite(GPIO_FOCUS, LOW);
	// digitalWrite(GPIO_POWER, LOW);

	Serial.begin(115200);

	// WiFi.mode(WIFI_STA);
	WiFi.softAP("EZ_" + WiFi.macAddress().substring(12, 17), WIFI_PASSWORD, 1, 0, 1);
	WiFi.onEvent(WiFiEvent);

	// ui = UIManager(GPIO_SDA, GPIO_SCL);
	ui.Setup(GPIO_SDA, GPIO_SCL);
	// shooter = ShooterManager();

	delay(500);

	server.on("/", [](AsyncWebServerRequest *request) { HelloWorldRequest(request); });
			//   {
				  
			// 	  // display.setCursor((SCREEN_WIDTH / 2) - (11 * 6) / 2, ((SCREEN_HEIGHT / 2) - 4));
			// 	  // display.setTextSize(1);
			// 	  // display.println("Hola mundo");
			// 	  // display.display();

			// 	  // request->send(200, "text/plain", "hello world"); }
			//   });

	server.on("/trigger-shoot", [](AsyncWebServerRequest *request) { 
		shooter.TakeShoot();
		request->send(200, "text/plain", "ok");

	});
			//   {
			// 	  // digitalWrite(GPIO_FOCUS, HIGH);
			// 	  // delay(2);
			// 	  // digitalWrite(GPIO_SHUTTER, HIGH);
			// 	  // delay(2000);
			// 	  // digitalWrite(GPIO_SHUTTER, LOW);
			// 	  // digitalWrite(GPIO_FOCUS, LOW);

			// 	  // delay(100);
			// 	  // request->send(200, "text/plain", "ok");
			//   });

	server.begin();

	// GenerateWifiQR(WiFi.softAPSSID(), WIFI_PASSWORD);

	ui.PrintWifiQR(WiFi.softAPSSID(), WIFI_PASSWORD);

	// ui.SetStatus("Idle");
	// ui.SetInfo02(WiFi.softAPIP().toString());
	// ui.UpdateIU();
}

void loop()
{		
	if (ClientConnected)
		ui.UpdateUI();
		
	delay(300); // FrameRate 10 fps
}

// Callback para manejo de eventos de WiFi
void WiFiEvent(WiFiEvent_t event)
{
	Serial.println("Evento: " + event);

	switch (event)
	{
	case WIFI_EVENT_AP_STACONNECTED:
		Serial.println("¡Un cliente se ha conectado al AP!");

		// display.clearDisplay();
		// display.display();

		// ShowInfoBar();
		// ShowTaskBar();

		ui.SetStatus("Idle");
		ui.SetInfo02(WiFi.softAPIP().toString());

		ClientConnected = true;

		break;

	case WIFI_EVENT_AP_STADISCONNECTED:
		Serial.println("Un cliente se ha desconectado del AP.");
		ui.PrintWifiQR(WiFi.softAPSSID(), WIFI_PASSWORD);

		break;
	default:
		break;
	}
}