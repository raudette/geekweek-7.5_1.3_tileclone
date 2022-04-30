//V0.1

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;
long chaos;
long silent;
uint16_t SERVICE_UUID = 0xFEED; // Service UUID for Tile 
#define CHARACTERISTIC_UUID "9d410019-35d6-f4dd-ba60e7bd8dc491c00" //Characteristic UUID for Tile


class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

void setup() {
  Serial.begin(115200);
 randomSeed(analogRead(0));
  Serial.println("Starting Up Evil Tile");
  //Randomize the mac address.
  chaos = random(1,4);
  Serial.println(chaos);

  //IMPORTANT - SET THE SPOOF MACS IN THIS SECTION
  //                                                //This -0x02 is important, as the BLE MAC is +2 of the value you put in here
  //uint8_t new_mac[7] = {0x00,0x00,0x00,0x00,0x00,0x02-0x02}; //Replace this with your MAC address.
  if (chaos == 1){
      uint8_t new_mac[7] = {0x00,0x00,0x00,0x00,0x00,0x02-0x02};
      esp_base_mac_addr_set(new_mac);
      Serial.println("Broadcasting as 00:00:00:00:00:00");
  }
  if (chaos == 2){
      uint8_t new_mac[7] = {0x00,0x00,0x00,0x00,0x00,0x02-0x02}; //f0:7e:b5:f1:10:04
      esp_base_mac_addr_set(new_mac);
      Serial.println("Broadcasting as 00:00:00:00:00:00");
  }
  if (chaos == 3){
      uint8_t new_mac[7] = {0x00,0x00,0x00,0x00,0x00,0x02-0x02}; //1C-78-3E-6A-A4-38
      esp_base_mac_addr_set(new_mac);
      Serial.println("Broadcasting as 00:00:00:00:00:00");
  }
  else{
      uint8_t new_mac[7] = {0x00,0x00,0x00,0x00,0x00,0x02-0x02}; //C1-B0-B5-9B-A6-6E
      esp_base_mac_addr_set(new_mac);
      Serial.println("Broadcasting as 00:00:00:00:00:00");
  }
  //END OF SPOOF MAC SECTION

  // Create the BLE Device
  BLEDevice::init("Evil TILE");
  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY
                    );

  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
  // Create a BLE Descriptor
  pCharacteristic->addDescriptor(new BLE2902());

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  Serial.println("Waiting for connection...");
}

void loop() {
  silent = random(1,18000);
    //Serial.println(silent);
    //delay(silent);
    if (chaos == 1){
       esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT,ESP_PWR_LVL_N12);
       Serial.println("TX:-12 dbm");
    }
    if (chaos == 2){
      esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT,ESP_PWR_LVL_N9);
      Serial.println("TX:-9 dbm");
    }
    if (chaos == 3){
      esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT,ESP_PWR_LVL_N6);
      Serial.println("TX:-6 dbm");
    }
    if (chaos == 4){
      esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT,ESP_PWR_LVL_N3);
      Serial.println("TX:-3 dbm");
    }
    if (chaos == 6){
      esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT,ESP_PWR_LVL_N0);
      Serial.println("TX:0 dbm");
    }
    if (chaos == 7){
      esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT,ESP_PWR_LVL_P3);
      Serial.println("TX:+3 dbm");
    }
    if (chaos == 8){
      esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT,ESP_PWR_LVL_P6);
      Serial.println("TX:+6 dbm");
    }
     if (chaos == 9){
      esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT,ESP_PWR_LVL_P9);
      Serial.println("TX:+9 dbm");
    }
    else{
      esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT,ESP_PWR_LVL_P3);

    }
    // notify changed value
    if(silent <= 474){
      ESP.restart();

    }

        if (deviceConnected) {
        pCharacteristic->setValue((uint8_t*)&value, 4);
        pCharacteristic->notify();
        value++;
        delay(3); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
    }
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("Advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        oldDeviceConnected = deviceConnected;
    }
}