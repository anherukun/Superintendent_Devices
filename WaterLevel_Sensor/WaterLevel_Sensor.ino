#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

/* this can be run with an emulated server on host:
        cd esp8266-core-root-dir
        cd tests/host
        make ../../libraries/ESP8266WebServer/examples/PostServer/PostServer
        bin/PostServer/PostServer
   then put your PC's IP address in SERVER_IP below, port 9080 (instead of default 80):
*/
//#define SERVER_IP "10.0.1.7:9080" // PC address with emulation on host
#define SERVER_IP "192.168.31.65:25520"
#define DEVICETYPE "water_level_sensor"
#define DEVICENAME "Sensor Nivel de Agua"
#ifndef STASSID
#define STASSID "UBNT-HOME"
#define STAPSK  "Brazil_2014"
#endif

char DEVICEID[] = "11AD6066-5088-47AE-B6B4-909AAB296958";
// DEVICEID = "11AD6066-5088-47AE-B6B4-909AAB296958";

void setup() {

  Serial.begin(9600);

  Serial.println();
  Serial.println();
  Serial.println();

  WiFi.hostname(DEVICENAME);
  WiFi.begin(STASSID, STAPSK);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  delay(500);

  SendConnection();
}

void loop() {
  // wait for WiFi connection
  if ((WiFi.status() == WL_CONNECTED)) {

    WiFiClient client;
    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    // configure traged server and url
    http.begin(client, "http://" SERVER_IP "/api/Data/Add"); //HTTP
    http.addHeader("Content-Type", "application/json");

    Serial.print("[HTTP] POST...\n");
    float value = getRandom();
    Serial.println("{\"IDDevice\":\""+ String(DEVICEID) +"\",\"value\":\"" + String(value, 2) +"\",\"DataType\": \"float\"}");
    // start connection and send HTTP header and body
    int httpCode = http.POST("{\"IDDevice\":\""+ String(DEVICEID) +"\",\"value\":\"" + String(value, 2) +"\",\"DataType\": \"float\"}");

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);

      // file found at server
      if (httpCode == 201) {
        const String& payload = http.getString();
        Serial.println("received payload:\n<<");
        Serial.println(payload);
        Serial.println(">>");
      }
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }

  delay(60000);
}

void SendConnection() {
  if ((WiFi.status() == WL_CONNECTED)) {
    WiFiClient client;
    HTTPClient http;
    String data = "{\"IDDevice\":\""+ String(DEVICEID) +"\", \"MacAddress\":\""+ WiFi.macAddress() +"\", \"IPAddress\":\""+ WiFi.localIP().toString() +"\", \"DeviceType\":\""+ DEVICETYPE +"\", \"DeviceName\":\""+ DEVICENAME +"\", \"DataType\":\"float\", \"MeasurementUnit\":\"%\", \"FirmwareVersion\":\"1.0.1\", \"DeviceStatus\":\"Online\"}";

    Serial.print("[HTTP] begin...\n");
    // configure traged server and url
    http.begin(client, "http://" SERVER_IP "/api/IOTDevice/Add"); //HTTP
    http.addHeader("Content-Type", "application/json");

    Serial.print("[HTTP] POST...\n");
    Serial.println(data);
    // start connection and send HTTP header and body
    int httpCode = http.POST(data);

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);

      // file found at server
      if (httpCode == 201) {
        const String& payload = http.getString();
        Serial.println("received payload:\n<<");
        Serial.println(payload);
        Serial.println(">>");
      }
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }
}

float getRandom() {
  return random(0, 100);
}
