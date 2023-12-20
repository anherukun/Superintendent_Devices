#include "Secrets.h"

#include <ArduinoJson.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#include <WiFi.h>

class BluetoothService
{

private:
    const char *bleServiceUUID;

    BLEDevice bleDevice;
    BLEService *pService;

    void CreateDeviceInfoCharacteristic()
    {
        Serial.println("Creating new value characteristic");

        BLECharacteristic *pCharacteristic = pService->createCharacteristic(DEVICE_INFO_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ);
        
        StaticJsonDocument<150> doc;

        doc["Device_ID"] = WiFi.macAddress();
        doc["Device_Type"] = DEVICE_TYPE;
        doc["Device_MAC"] = WiFi.macAddress();
        doc["Firmware_Version"]["Major"] = FIRMWARE_MAJOR_VERSION;
        doc["Firmware_Version"]["Minor"] = FIRMWARE_MINOR_VERSION;
        doc["Firmware_Version"]["Build"] = FIRMWARE_BUILD_VERSION;

        String payload;
        serializeJson(doc, payload);

        pCharacteristic->setValue(payload.c_str());
    }

public:
    BluetoothService(const char *serviceUUID)
    {
        bleServiceUUID = serviceUUID;

        Serial.println("Starting BLE");
        delay(100);

        bleDevice.init(DEVICE_NAME);
        BLEServer *pServer = bleDevice.createServer();
        pService = pServer->createService(bleServiceUUID);

        CreateDeviceInfoCharacteristic();

        delay(100);
    }

    void CreateCharacteristic(const char *characteristic_uuid, const char *service_uuid, u_int32_t properties, BLECharacteristicCallbacks *pCallbacks)
    {
        Serial.println("Creating new characteristic...");

        BLECharacteristic *pCharacteristic = pService->createCharacteristic(characteristic_uuid, properties);
        pCharacteristic->setCallbacks(pCallbacks);

        // pService->start();

        // BLEAdvertising *pAdvertising = bleDevice.getAdvertising();
        // pAdvertising->addServiceUUID(bleServiceUUID);
        // // pAdvertising->addServiceUUID(service_uuid);
        // pAdvertising->setScanResponse(true);
        // pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
        // pAdvertising->setMinPreferred(0x12);

        // bleDevice.startAdvertising();

        // Serial.print("Characteristic ");
        // Serial.print(characteristic_uuid);
        // Serial.println(" defined! Now you can read/write in it.");
        // Serial.println("Characteristic defined! Now you can read it in your phone!");
    }

    void StartService()
    {
        pService->start();

        BLEAdvertising *pAdvertising = bleDevice.getAdvertising();
        pAdvertising->addServiceUUID(bleServiceUUID);
        // pAdvertising->addServiceUUID(service_uuid);
        pAdvertising->setScanResponse(true);
        pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
        pAdvertising->setMinPreferred(0x12);

        bleDevice.startAdvertising();
    }

    void Stop()
    {
        bleDevice.stopAdvertising();
    }
};