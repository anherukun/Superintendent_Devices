// Eliminar del directorio C:\Users\angel\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.14\tools\sdk\esp32c3\include
// la carpeta qrcode
// #include "qrcode.h"
// #include "UIManager.h"
// #include "CameraProgram.h"
// #include "ShooterManager.h"
#include "Camtools/CameraPRGMModes.h"
#include "Camtools/CameraProgram.h"
#include "Camtools/ShooterManager.h"
#include "WebServerRequests.h"
#include "GPIO_Definitions.h"

#include <WiFi.h>
#include <WiFiClient.h>
#include <AsyncTCP.h>
#include <ESPmDNS.h>

// #define WIFI_QR "WIFI:S:MiRedWiFi;T:WPA;P:contraseña123;H:false;;"
#define WIFI_PASSWORD "19961999"

AsyncWebServer server(80);

UIManager ui = UIManager();

ShooterManager shooter(ui);
CameraProgram camera(shooter);

bool ClientConnected = false;

void setup()
{
	Serial.begin(115200);

	WiFi.softAP("EZ_" + WiFi.macAddress().substring(12, 17), WIFI_PASSWORD, 1, 0, 1);
	WiFi.onEvent(WiFiEvent);

	// ui = UIManager(GPIO_SDA, GPIO_SCL);
	ui.Setup(GPIO_SDA, GPIO_SCL);
	// shooter = ShooterManager();

	delay(500);

	server.on("/", [](AsyncWebServerRequest *request)
			  { request->send(200, "text/plain", "ok"); });
	//   {

	// 	  // display.setCursor((SCREEN_WIDTH / 2) - (11 * 6) / 2, ((SCREEN_HEIGHT / 2) - 4));
	// 	  // display.setTextSize(1);
	// 	  // display.println("Hola mundo");
	// 	  // display.display();

	// 	  // request->send(200, "text/plain", "hello world"); }
	//   });

	server.on("/trigger-shoot", [](AsyncWebServerRequest *request)
			  {
				  shooter.TakeShoot();
				  request->send(200, "text/plain", "ok"); });

	// 192.168.4.1/pgrm/load?mode=<PRGM_MODE>&exp_t=<SECONDS>&burst_c=<COUNT>&reburst_c=<COUNT>&burst_tb=<SECONDS>
	// 192.168.4.1/pgrm/load?mode=<0>&exp_t=<0>&burst_c=<0>&reburst_c=<0>&burst_tb=<0>
	// 192.168.4.1/pgrm/load?mode=<1>&exp_t=<60>&burst_c=<0>&reburst_c=<0>&burst_tb=<0>
	// 192.168.4.1/pgrm/load?mode=<2>&exp_t=<0>&burst_c=<10>&reburst_c=<0>&burst_tb=<0>
	// 192.168.4.1/pgrm/load?mode=<3>&exp_t=<60>&burst_c=<10>&reburst_c=<0>&burst_tb=<0>
	// 192.168.4.1/pgrm/load?mode=<4>&exp_t=<15>&burst_c=<10>&reburst_c=<3>&burst_tb=<60>
	server.on("/pgrm/load", [](AsyncWebServerRequest *request)
			  {
				  byte pgrm_mode = -1;
				  int exp_time = 0;
				  byte burst_c = 0;
				  byte reburst_c = 0;
				  int burst_time_b = 0;
				  bool focus = false;

				  if (request->hasParam("mode") == false)
				  {
					  request->send(400, "text/plain", "Error");
				  }

				  pgrm_mode = (byte)request->getParam("mode")->value().toInt();
				  exp_time = request->getParam("exp_t")->value().toInt();
				  burst_c = request->getParam("burst_c")->value().toInt();
				  reburst_c = request->getParam("reburst_c")->value().toInt();
				  burst_time_b = request->getParam("burst_tb")->value().toInt();
				  focus = request->getParam(focus)->value().toInt();

				  camera.LoadProgram(pgrm_mode, exp_time, burst_c, reburst_c, burst_time_b);

				  ui.SetStatus("PGRM Loaded");
				  ui.SetExpoTime(camera.GetExpositionTimePretty());
				  ui.SetBurstLimit(String(camera.GetBurstCount()));
				  ui.SetBurstTimeBetween(camera.GetBurstTimeBetweenPretty());
				  ui.SetReburstLimit(String(camera.GetReburstCount()));

				  request->send(200, "text/plain", "PGRM Loaded"); });

	server.begin();

	ui.PrintWifiQR(WiFi.softAPSSID(), WIFI_PASSWORD);
}

void loop()
{
	switch (camera.GetPROGRAM())
	{
	case PRGM_MODE_SINGLE_SHOOT:
		ui.SetMode("Single S.");
		break;

	case PRGM_MODE_SINGLE_SHOOT_BULB:
		ui.SetMode("Bulb S.");
		break;

	case PRGM_MODE_BURST_SHOOT:
		ui.SetMode("Burst S.");
		break;

	case PRGM_MODE_BURST_SHOOT_BULB:
		ui.SetMode("Bulb B.");
		break;

	case PRGM_MODE_REBURST_SHOOT:
		ui.SetMode("Reburst");
		break;

	default:
		break;
	}
	if (ClientConnected)
		ui.UpdateUI(camera.GetPROGRAM());

	delay(300); // FrameRate 5 fps
}

// Callback para manejo de eventos de WiFi
void WiFiEvent(WiFiEvent_t event)
{
	Serial.println("Evento: " + event);

	switch (event)
	{
	case WIFI_EVENT_AP_STACONNECTED:
		Serial.println("¡Un cliente se ha conectado al AP!");

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