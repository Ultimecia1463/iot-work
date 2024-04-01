#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
  
#define led 12

BLEServer* pServer;
BLECharacteristic* pCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
char cmd;

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  }

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
  }
};
int brightness = 0;
int fadeAmount = 20;

void setup() {
  Serial.begin(115200);
  
  // Initialize the BLE device with a unique name
  BLEDevice::init("ESP32-CAM");

  // Create a BLE server and set its callbacks
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create a BLE service with a custom UUID
  BLEService *pService = pServer->createService(BLEUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b"));

  // Create a BLE characteristic with read, write, and notify properties
  pCharacteristic = pService->createCharacteristic(
      BLEUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8"),
      BLECharacteristic::PROPERTY_READ |
      BLECharacteristic::PROPERTY_WRITE |
      BLECharacteristic::PROPERTY_NOTIFY
  );

  // Add a descriptor to the characteristic
  pCharacteristic->addDescriptor(new BLE2902());

  // Start the BLE service
  pService->start();

  // Configure BLE advertising
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->addServiceUUID(pService->getUUID());
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);

  // Start advertising
  BLEDevice::startAdvertising();

  // Set GPIO pin 2 as an output
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW); // Initially set it to LOW
  Serial.println("Waiting for a client connection...");
}

void loop() {
  if (deviceConnected) {
    // Read the value of the characteristic
    std::string value = pCharacteristic->getValue();
    if (!value.empty()) {
      cmd = value[0];
      if (cmd == '1') {
        digitalWrite(led, HIGH);
        Serial.println("off"); // Set GPIO2 HIGH
      } else if (cmd == '0') {
        digitalWrite(led, LOW);
        Serial.println("on"); // Set GPIO2 LOW
      }else if (cmd == '2') {
        digitalWrite(led, LOW);
        delay(200);
        digitalWrite(led, HIGH);
        delay(200);
      }
    }
  }

  if (!deviceConnected && oldDeviceConnected) {
    delay(500); // Give the Bluetooth stack the chance to get things ready
    pServer->startAdvertising(); // Restart advertising
    Serial.println("Waiting for a client connection...");
    oldDeviceConnected = false;
  }

  if (deviceConnected && !oldDeviceConnected) {
    // Do stuff here when a device connects
    oldDeviceConnected = true;
  }
  delay(100);
}
