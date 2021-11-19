/*
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
   Ported to Arduino ESP32 by Evandro Copercini
*/

#include "HexToByteArray.h"
#include "RuuviParse.h"
#include <WiFi.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <iostream>

HexToByteArray byteArrayConv;
RuuviParse parser;

const byte MaxByteArraySize = 16;
int scanTime = 5; //In seconds
BLEScan* pBLEScan;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      //Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
      if (advertisedDevice.haveManufacturerData()) {
        char *pHex = BLEUtils::buildHexData(nullptr, (uint8_t*)advertisedDevice.getManufacturerData().data(), advertisedDevice.getManufacturerData().length());
        if (strncmp(pHex, "9904", 4) == 0) {
          std::string res = "Name: " + advertisedDevice.getName() + ", Address: " + advertisedDevice.getAddress().toString() + "\n";
          std::cout << res;
          //Serial.printf("buildHexData: %s \n", pHex);
          byte byteArray[MaxByteArraySize] = {0};
          byteArrayConv.hexCharacterStringToBytes(byteArray, pHex);
          //byteArrayConv.dumpByteArray(byteArray, MaxByteArraySize);
          //Serial.printf("getManufacturer: %X \n", parser.getManufacturer(byteArray));
          Serial.printf("getFormat: %u \n", parser.getFormat(byteArray));
          Serial.printf("getTemperature: %g C \n", parser.getTemperature(byteArray));
          Serial.printf("getPressure: %u Pa \n", parser.getPressure(byteArray));
          Serial.printf("getHumidity: %g RH \n", parser.getHumidity(byteArray));
          //Serial.printf("getAcceleration X: %i mG \n", parser.getAccelerationX(byteArray));
          //Serial.printf("getAcceleration Y: %i mG \n", parser.getAccelerationY(byteArray));
          //Serial.printf("getAcceleration Z: %i mG \n", parser.getAccelerationZ(byteArray));
          Serial.printf("getBattery: %g mV \n", parser.getBattery(byteArray));
        } else {
          Serial.println("Not a Ruuvi device.");
        }
        Serial.println();
        free(pHex);
      }
    }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Scanning...");

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value

  Serial.println("DF3 test data:");
  char *test_df3 = "990403291A1ECE1EFC18F94202CA0B53";
  byte testByteArray[MaxByteArraySize] = {0};
  byteArrayConv.hexCharacterStringToBytes(testByteArray, test_df3);
  Serial.printf("getFormat: %u \n", parser.getFormat(testByteArray));
  Serial.printf("getTemperature: %g C \n", parser.getTemperature(testByteArray));
  Serial.printf("getPressure: %u Pa \n", parser.getPressure(testByteArray));
  Serial.printf("getHumidity: %g RH \n", parser.getHumidity(testByteArray));
  Serial.println("");

  Serial.println("DF5 test data:");
  char *test_df5 = "99040512FC5394C37C0004FFFC040CAC364200CDCBB8334C884F";
  testByteArray[MaxByteArraySize] = {0};
  byteArrayConv.hexCharacterStringToBytes(testByteArray, test_df5);
  Serial.printf("getFormat: %u \n", parser.getFormat(testByteArray));
  Serial.printf("getTemperature: %g C \n", parser.getTemperature(testByteArray));
  Serial.printf("getPressure: %u Pa \n", parser.getPressure(testByteArray));
  Serial.printf("getHumidity: %g RH \n", parser.getHumidity(testByteArray));
  Serial.println("");
}

void loop() {
  // put your main code here, to run repeatedly:
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.print("Devices found: ");
  Serial.println(foundDevices.getCount());
  Serial.println("Scan done!");
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  delay(2000);
}
