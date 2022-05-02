/*
EVIL_TILE
GeekWeek 7.5
Version 1.0
Last Modified: 5/2/2022 @ 13:54
*/
// Required Libraries
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// Some of the variables you will need and settings that can be modified.
long chaos;
long stray;
long silent;
uint16_t NAME_UUID = 0x2A00;
uint16_t APP_UUID = 0x2A01;
uint16_t PER_UUID = 0x2A04;
uint16_t TILE_UUID = 0xFEED;
#define CHARACTERISTIC1_UUID  "9d410019-35d6-f4dd-ba60e7bd8dc491c00"
#define CHARACTERISTIC2_UUID  "9d410019-35d6-f4dd-ba60e7bd8dc491c00"

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));
  Serial.println("Starting Up Evil Tile");
  //Randomize the mac address.
  chaos = random(1,4);
  stray = random(1,8);

/* This block will randomize the MAC address, to modify the mac address just replace the 2 characters after the 0x for each octet so that if the mac address is: da:2a:d1:de:2d:b3 then it would be represented as 0xda,0x2a,0xd1,0xde,0x2d,0xb3.
   After transcribing the address into the correct format, add -0x02 to the end of it. This will subtract 2 from the base mac address, which will allow then ensure that the bluetooth MAC address matches the address you've added.
   The final address should look like this: {0xda,0x2a,0xd1,0xde,0x2d,0xb3-0x02}
 */
  if (chaos == 1){
      uint8_t new_mac[7] = {0xda,0x2a,0xd1,0xde,0x2d,0xb3-0x02}; //da:2a:d1:de:2d:b3
      esp_base_mac_addr_set(new_mac);
      Serial.println("Broadcasting as da:2a:d1:de:2d:b3");
  }
  if (chaos == 2){
      uint8_t new_mac[7] = {0xf0,0x7e,0xb5,0xf1,0x10,0x04-0x02}; //f0:7e:b5:f1:10:04
      esp_base_mac_addr_set(new_mac);
      Serial.println("Broadcasting as f0:7e:b5:f1:10:04");
  }
  if (chaos == 3){
      uint8_t new_mac[7] = {0x1c,0x78,0x3e,0x6a,0xa4,0x38-0x02}; //1C-78-3E-6A-A4-38
      esp_base_mac_addr_set(new_mac);
      Serial.println("Broadcasting as 1c:78:3e:6a:a4:38");
  }
  else{
      uint8_t new_mac[7] = {0xda,0x2a,0xd1,0xde,0x2d,0xb3-0x02}; //C1-B0-B5-9B-A6-6E
      esp_base_mac_addr_set(new_mac);
      Serial.println("Broadcasting as da:2a:d1:de:2d:b3");
  }
// Cycle through a different Transmission Power (TX_PWR).
  if (stray == 1){
       esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT,ESP_PWR_LVL_N12);
       Serial.println("TX:-12 dbm");
    }
    if (stray == 2){
      esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT,ESP_PWR_LVL_N9);
      Serial.println("TX:-9 dbm");
    }
    if (stray == 3){
      esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT,ESP_PWR_LVL_N6);
      Serial.println("TX:-6 dbm");
    }
    if (stray == 4){
      esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT,ESP_PWR_LVL_N3);
      Serial.println("TX:-3 dbm");
    }
    if (stray == 6){
      esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT,ESP_PWR_LVL_N0);
      Serial.println("TX:0 dbm");
    }
    if (stray == 7){
      esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT,ESP_PWR_LVL_P3);
      Serial.println("TX:+3 dbm");
    }
    if (stray == 8){
      esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT,ESP_PWR_LVL_P6);
      Serial.println("TX:+6 dbm");
    }
     if (stray == 9){
      esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT,ESP_PWR_LVL_P9);
      Serial.println("TX:+9 dbm");
    }
    else{
      esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT,ESP_PWR_LVL_P3);
      Serial.println("TX:+3 dbm");
    }
// Create 2 Services and set the appropriate characteristics.
  BLEDevice::init("EVIL_TILE");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService1 = pServer->createService(NAME_UUID);
  BLECharacteristic *pCharacteristic1 = pService1->createCharacteristic(
                                        NAME_UUID, 
                                        BLECharacteristic::PROPERTY_READ
                                        );
  BLECharacteristic *pCharacteristic2 = pService1->createCharacteristic(
                                        APP_UUID, 
                                        BLECharacteristic::PROPERTY_READ);

  BLECharacteristic *pCharacteristic3 = pService1->createCharacteristic(
                                        PER_UUID, 
                                        BLECharacteristic::PROPERTY_READ);
    pService1->start();
 
  BLEService *pService2 = pServer->createService(TILE_UUID);
  BLECharacteristic *pCharacteristic4 = pService2->createCharacteristic(
                                         CHARACTERISTIC1_UUID,
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
  BLECharacteristic *pCharacteristic5 = pService2->createCharacteristic(
                                       CHARACTERISTIC2_UUID,
                                       BLECharacteristic::PROPERTY_NOTIFY
                                     );
  BLEDescriptor *pDescriptor1 = new BLEDescriptor((uint16_t)0x2902); 
  pService2->start();
  BLEDevice::startAdvertising();
  Serial.println("Advertising...");
}

void loop() {
    silent = random(1,30000);    
    do{
      --silent;
      delay(10);
    }
    while (silent >= 1);
    ESP.restart();
    }