#include "Secrets.h"

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

class BluetoothService
{

private:
    BLEDevice bleDevice;
    BLEService *pService;

public:
    BluetoothService()
    {
        Serial.println("Starting BLE");
        delay(100);

        bleDevice.init(DEVICE_NAME);
        BLEServer *pServer = bleDevice.createServer();
        pService = pServer->createService(BLE_SERVICE_UUID);
        
        delay(100);
    }

    void CreateCharacteristic(const char *uuid, u_int32_t properties, BLECharacteristicCallbacks *pCallbacks)
    {
        Serial.println("Creating new characteristic...");

        BLECharacteristic *pCharacteristic = pService->createCharacteristic(uuid, BLECharacteristic::PROPERTY_WRITE);
        pCharacteristic -> setCallbacks(pCallbacks);

        pService->start();
        
        BLEAdvertising *pAdvertising = bleDevice.getAdvertising();
        pAdvertising->addServiceUUID(uuid);
        pAdvertising->setScanResponse(true);
        pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
        pAdvertising->setMinPreferred(0x12);

        bleDevice.startAdvertising();

        //BLEDevice::startAdvertising();

        Serial.println("Characteristic defined! Now you can read it in your phone!");
    }

    void Stop() {
        bleDevice.stopAdvertising();
    }
};