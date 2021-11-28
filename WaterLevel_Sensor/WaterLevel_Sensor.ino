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
#define SERVER_IP_PROD "192.168.31.13:80"
#define DEVICETYPE "water_level_sensor"
#define DEVICENAME "Sensor_Nivel_de_Agua"
#define DATATYPE "float"
#define UNITVALUE "%"
#define FIRMWAREVERSION "1.1.0"
#ifndef STASSID
#define STASSID "UBNT-HOME"
#define STAPSK  "Brazil_2014"
#endif

#define _pintrig 0
#define _pinecho 1

char DEVICEID[] = "11AD6066-5088-47AE-B6B4-909AAB296958";

int profundidad = 126;

void setup() {

  Serial.begin(9600);

  Serial.println();
  Serial.println();
  Serial.println();

  pinMode(_pinecho, INPUT);
  pinMode(_pintrig, OUTPUT);

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

  SendConnection("http://" SERVER_IP "/api/IOTDevice/Add");
  SendConnection("http://" SERVER_IP_PROD "/api/IOTDevice/Add");
}

void loop() {
  //SendValue("http://" SERVER_IP "/api/Data/Add", GetActualCapacity(GetDistance()));
  SendValue("http://" SERVER_IP_PROD "/api/Data/Add", GetActualCapacity(GetDistance()));

  SendValue("http://" SERVER_IP "/api/Data/Add", GetDistance());
  //SendValue("http://" SERVER_IP_PROD "/api/Data/Add", GetDistance());

  UpdateConnectionStatus("http://" SERVER_IP "/api/IOTDevice/Update/" + String(DEVICEID));
  UpdateConnectionStatus("http://" SERVER_IP_PROD "/api/IOTDevice/Update/" + String(DEVICEID));

  delay(60000);
  //delay(10000);
}

void SendValue(String server, float value) {
  // wait for WiFi connection
  if ((WiFi.status() == WL_CONNECTED)) {

    WiFiClient client;
    HTTPClient http;
    String data = "{\"IDDevice\":\""+ String(DEVICEID) +"\",\"value\":\"" + String(value, 2) +"\",\"DataType\": \"float\"}";

    Serial.print("[HTTP] begin...\n");
    // configure traged server and url
    http.begin(client, server); //HTTP
    http.addHeader("Content-Type", "application/json");

    Serial.print("[HTTP] POST...\n");
    // float value = getRandom();
    Serial.println(WiFi.localIP().toString() +">>: " + data);
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

void UpdateConnectionStatus(String server) {
  if ((WiFi.status() == WL_CONNECTED)) {
    WiFiClient client;
    HTTPClient http;
    String data = "{\"IPAddress\":\""+ WiFi.localIP().toString() +"\", \"DeviceName\":\""+ DEVICENAME +"\", \"DeviceStatus\":\"Online\"}";

    Serial.print("[HTTP] begin...\n");
    // configure traged server and url
    http.begin(client, server); //HTTP
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

void SendConnection(String server) {
  if ((WiFi.status() == WL_CONNECTED)) {
    WiFiClient client;
    HTTPClient http;
    String data = "{\"IDDevice\":\""+ String(DEVICEID) +"\", \"MacAddress\":\""+ WiFi.macAddress() +"\", \"IPAddress\":\""+ WiFi.localIP().toString() +"\", \"DeviceType\":\""+ DEVICETYPE +"\", \"DeviceName\":\""+ DEVICENAME +"\", \"DataType\":\""+ DATATYPE +"\", \"MeasurementUnit\":\""+ UNITVALUE +"\", \"FirmwareVersion\":\""+ FIRMWAREVERSION +"\", \"DeviceStatus\":\"Online\"}";

    Serial.print("[HTTP] begin...\n");
    // configure traged server and url
    http.begin(client, server); //HTTP
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

float GetDistance() { // Centimeters
  // Clears the trigPin condition
  digitalWrite(_pintrig, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(_pintrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(_pintrig, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  long duration = pulseIn(_pinecho, HIGH);
  // Calculating the distance
  return duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
}

float GetActualCapacity(float distance) {
  if (distance > profundidad)
  {
    return 0;
  } else if (distance < 0) {
    return 100;
  } else {
    return ((distance - profundidad) * 100 / profundidad) * (-1);
  }    
}

float getRandom() {
  return random(0, 100);
}
