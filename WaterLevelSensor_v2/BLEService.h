// #include "Secrets.h"

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

void StartBluetoothService(const char *uuid, u_int32_t properties, BLECharacteristicCallbacks *pCallbacks)
{
    //Serial.println(F("Starting BLE"));
    delay(100);

    BLEDevice::init("XIAO_C3");
    BLEServer *pServer = BLEDevice::createServer();
    BLEService *pService = pServer->createService("BA996CEB-71A4-4EF1-8AC5-CE55B792FD31");

    //Serial.println(F("Creating new characteristic..."));

    BLECharacteristic *pCharacteristic = pService->createCharacteristic(uuid, BLECharacteristic::PROPERTY_WRITE);
    pCharacteristic->setCallbacks(pCallbacks);

    pService->start();

    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(uuid);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);

    BLEDevice::startAdvertising();

    //Serial.println(F("Characteristic defined! Now you can read it in your phone!"));
    delay(100);
}