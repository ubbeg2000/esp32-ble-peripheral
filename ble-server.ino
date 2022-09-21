/*********
  Rui Santos
  Complete instructions at https://RandomNerdTutorials.com/esp32-ble-server-client/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*********/

#include <BLEDevice.h>
#include <BLEServer.h>
//#include <BLEUtils.h>
//#include <BLE2902.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <stdio.h>

char bleServerName[] = "BME280_ESP32";
char WIFI_SSID[] = "DedeNET";
char WIFI_PASSWORD[] = "ElkhanKasep2022!";
char AUTH_SERVICE_UUID[] = "213850a2-0a13-472e-8018-65e644f39fd6";

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 1000;

int currentRssi = 0;

bool deviceConnected = false;

// Access Control Characteristic and Descriptor
BLECharacteristic accessControlCharacteristics("1266cc05-10fb-4452-b42c-963d1efa6b67",
    BLECharacteristic::PROPERTY_NOTIFY |
    BLECharacteristic::PROPERTY_READ   |
    BLECharacteristic::PROPERTY_WRITE  |
    BLECharacteristic::PROPERTY_NOTIFY |
    BLECharacteristic::PROPERTY_INDICATE
                                              );

BLEDescriptor accessControlDescriptor(BLEUUID((uint16_t)0x2902));

//Setup callbacks onConnect and onDisconnect
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      Serial.println("SOMEONE CONNECTED");
      deviceConnected = true;
    };
    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

class MyCallbackHandler: public BLECharacteristicCallbacks {
    //    void onRead(BLECharacteristic* pCharacteristic) {
    //      HTTPClient http;
    //
    //      String serverPath = "http://192.168.0.1/dat?rssi=24.37&id=1&c=";
    //
    //      // Your Domain name with URL path or IP address with path
    //      http.begin(serverPath.c_str());
    //
    //      // Send HTTP GET request
    //      int httpResponseCode = http.GET();
    //    }
    void onWrite(BLECharacteristic* pCharacteristic) {
      HTTPClient http;
      char buffer[100];

      //      String serverPath = "http://192.168.0.1/dat?rssi=" + currentRssi + "&id=1&c=" + pCharacteristic->getValue();
//      sprintf(buffer, "http://192.168.0.1/dat?rssi=%d&id=1&c=%s", currentRssi, pCharacteristic->getValue());

      // Your Domain name with URL path or IP address with path
//      http.begin(String(buffer).c_str());

      // Send HTTP GET request
//      int httpResponseCode = http.GET();
    }
};

static void my_gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t* param) {
  Serial.print("RSSI "); Serial.println(param->read_rssi_cmpl.rssi);
  currentRssi = param->read_rssi_cmpl.rssi;
}

void setup() {
  // Start serial communication
  Serial.begin(115200);
  
  // Connect to wifi
//  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
//  Serial.println("Connecting...");
//
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.print(".");
//  }
//
//  Serial.println("");
//  Serial.print("Connected as: ");
//  Serial.println(WiFi.localIP());

  // Create the BLE Device
  BLEDevice::init(bleServerName);
  BLEDevice::setCustomGapHandler(my_gap_event_handler);

  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *accessControlService = pServer->createService("213850a2-0a13-472e-8018-65e644f39fd6");

  accessControlCharacteristics.setCallbacks(new MyCallbackHandler());
  accessControlCharacteristics.addDescriptor(&accessControlDescriptor);
  accessControlService->addCharacteristic(&accessControlCharacteristics);

  // Starting BLE service
  accessControlService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(AUTH_SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
//  pServer->getAdvertising()->start();

  Serial.println("Waiting client...");
}

void loop() {
  //  if (deviceConnected) {
  //    if ((millis() - lastTime) > timerDelay) {
  //      // Notify device ID
  //      static char deviceID[3] = "69";
  //
  //      accessControlCharacteristics.setValue(deviceID);
  //      accessControlCharacteristics.notify();
  //
  //      lastTime = millis();
  //    }
  //  }
}
