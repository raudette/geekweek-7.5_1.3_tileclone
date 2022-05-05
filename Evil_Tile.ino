/*
EVIL_TILE

The Evil Tile leverages an ESP32 device to emulate the functions of a TILE BLE (Bluetooth Low Energy) tracker.
The code below attempts to replicate the profile of a TILE BLE tracker.
Although it is currently configured for use with a TILE reconfiguring the settings would allow you to emulate any BLE device
by modifying the appropriate Service UUIDs and Characteristic UUIDs and values.

GeekWeek 7.5 Team 1-3_iot_hunting
Version 1.0
Last Modified: 5/5/2022 @ 13:16 AST
*/

// Required Libraries
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// Some of the variables you will need and settings that can be modified.

long chaos; // used to store a random value that is generated.
long stray; // used to store a random value that is generated.
long silent; // used to store a random value that is generated.
uint16_t NAME_UUID = 0x2A00; 
uint16_t APP_UUID = 0x2A01;
uint16_t PER_UUID = 0x2A04;
uint16_t TILE_UUID = 0xFEED; // This UUID sets 0xFEED which is used by TILE inc. to identify TILE devices.
#define CHARACTERISTIC1_UUID  "9d410019-35d6-f4dd-ba60e7bd8dc491c00" // These UUIDs are used to identify services on TILE devices.
#define CHARACTERISTIC2_UUID  "9d410019-35d6-f4dd-ba60e7bd8dc491c00" // These UUIDs are used to identify services on TILE devices.

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0)); // This attempts to use Pin 0 to generate entropy.
  Serial.println("Starting Up Evil Tile");
  //Randomize the mac address.
  chaos = random(1,4); // Generates a random value betwen 1 and 4 to select a MAC Address.
  stray = random(1,9); // Generates a random value between 1 and 9 to select transmission power.

/* This block will randomize the MAC address, to modify the mac address just replace the 2 characters after the 0x for each octet so that if the mac address is: da:2a:d1:de:2d:b3 then it would be represented as 0xda,0x2a,0xd1,0xde,0x2d,0xb3.
   After transcribing the address into the correct format, add -0x02 to the end of it. This will subtract 2 from the base mac address, which will allow then ensure that the bluetooth MAC address matches the address you've added.
   The final address should look like this: {0xda,0x2a,0xd1,0xde,0x2d,0xb3-0x02}

   Reference: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/misc_system_api.html
 */
  if (chaos == 1){
      uint8_t new_mac[7] = {0xda,0x2a,0xd1,0xde,0x2d,0xb3-0x02}; //da:2a:d1:de:2d:b3
      esp_base_mac_addr_set(new_mac); // Configures the esp32 Base MAC address, this address is used to generate all MAC addresses on the ESP32.
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

/* Cycle through a different Transmission Power (TX_PWR).
   This block uses the randomness generated above and stored in stray to select a random transmission power for the ESP32.
   The intention is to make it more difficult to detect the device by means of signal strength analysis.

   It is also possible to configure transmission power for scanning and advertizing independently. 
   Reference: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/bluetooth/controller_vhci.html

*/
  if (stray == 1){
       esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT,ESP_PWR_LVL_N12); //This sets the default transmission power of the ESP32 from which all transmission powers are derived. 
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
/* Create 2 Services and set the appropriate characteristics.
   Bluetooth Low Energy Assigned Numbers - https://www.bluetooth.com/specifications/assigned-numbers/

*/

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
  /*
  This block generates a random number between 1 and 30 000, it will then depricate by 1 every cycle and sleep for 10 miliseconds, once the value is below 1
  the ESP32 is reset. This will cause setup to run again and issue a new MAC Address and a new transmission power setting.
  */
    silent = random(1,30000);    
    do{
      --silent;
      delay(10);
    }
    while (silent >= 1);
    ESP.restart();
    }


    /*
      References:
      Bluetooth LE:
      https://espressif-docs.readthedocs-hosted.com/projects/espressif-esp-faq/en/latest/software-framework/ble-bt.html#:~:text=What%20is%20the%20transmit%20power,with%20a%203%20dBm%20interval.

      Bluetooth Specification:
      https://www.bluetooth.com/specifications/

      Nordic Semiconductor Introduction to BLE:
      https://webinars.nordicsemi.com/introduction-to-bluetooth-low-6
    */
