#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <EEPROM.h>
#include <WiFi.h>

int address_wifi_host = 0;
int address_wifi_passwd = 100;

std::string WIFI_HOST = "Halo"; // Halo
std::string WIFI_PASSWD = "82532437"; // 82532437

short max_wifi_length = 64;

const char* DEVICE_NAME = "Plant supervisor";
const char* WIFI_HOST_UUID = "108bdd67-3b58-469b-b207-eb55d26357e4";
const char* WIFI_PASSWD_UUID = "4eddb67b-5382-45bf-9cf3-8171d441851b";

BLEServer* pServer = NULL;
BLECharacteristic* pWifiHostChar = NULL;
BLECharacteristic* pWifiPasswdChar = NULL;

class MyCallbacks: public BLECharacteristicCallbacks 
{

  void onWrite(BLECharacteristic *pCharacteristic) 
  {
    std::string value = pCharacteristic->getValue();

    if (pCharacteristic->getUUID().toString() == WIFI_HOST_UUID) 
    {
      Serial.print("Wifi Host: ");
      Serial.println(value.c_str());

      WIFI_HOST = value;

      // Write the WIFI host string to the EEPROM
      for (int i = 0; i < WIFI_HOST.length(); i++) 
      {
        EEPROM.write(address_wifi_host + i, WIFI_HOST[i]);
      }

      EEPROM.write(address_wifi_host + WIFI_HOST.length(), '\0');

      EEPROM.commit();
    } 
    else if (pCharacteristic->getUUID().toString() == WIFI_PASSWD_UUID) 
    {
      Serial.print("Wifi Passwd: ");
      Serial.println(value.c_str());

      WIFI_PASSWD = value;

      // Write the WIFI passwd string to the EEPROM
      for (int i = 0; i < WIFI_PASSWD.length(); i++) 
      {
        EEPROM.write(address_wifi_passwd + i, WIFI_PASSWD[i]);
      }

      EEPROM.write(address_wifi_passwd + WIFI_PASSWD.length(), '\0');

      EEPROM.commit();
    }

    // Add the condition to stop the BLE server
    if (!WIFI_HOST.empty() && !WIFI_PASSWD.empty()) 
    {
      // Remove the BLE server
      Serial.println("BLE server stopped.");
      BLEDevice::deinit();
    }
  }
};

void initBLEDevice()
{
  BLEDevice::init(DEVICE_NAME);

  pServer = BLEDevice::createServer();

  BLEService* pService = pServer->createService("WiFi");

  pWifiHostChar = pService->createCharacteristic(
    WIFI_HOST_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
  );

  pWifiHostChar->setValue("Host");

  pWifiHostChar->setCallbacks(new MyCallbacks());

  pWifiPasswdChar = pService->createCharacteristic(
    WIFI_PASSWD_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
  );

  pWifiPasswdChar->setValue("Passwd");
  
  pWifiPasswdChar->setCallbacks(new MyCallbacks());

  pService->start();
  pServer->getAdvertising()->start();

  Serial.println("Bluetooth server started");
}

std::string readFromMemory(const int address)
{
  // Read the string from the EEPROM
  std::string storedStr = "";
  int i = 0;
  char c = EEPROM.read(address + i);
  while (c != '\0' && i < max_wifi_length) {
    storedStr += c;
    i++;
    c = EEPROM.read(address + i);
  }

  if (i == max_wifi_length)
  {
    return "";
  }

  return storedStr;
}

void connectToWiFi()
{
  WiFi.begin(WIFI_HOST.c_str(), WIFI_PASSWD.c_str());

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.print(".");
  }

  WiFi.hostname("Plant supervisor");

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  // EEPROM.begin(512);

  // Read the WIFI host from the EEPROM
  // WIFI_HOST = readFromMemory(address_wifi_host);
  Serial.print("Stored WIFI host: ");
  Serial.println(WIFI_HOST.c_str());

  // Read the WIFI passwd string from the EEPROM
  // WIFI_PASSWD = readFromMemory(address_wifi_passwd);
  Serial.print("Stored WIFI passwd: ");
  Serial.println(WIFI_PASSWD.c_str());

  if(WIFI_HOST.empty() || WIFI_PASSWD.empty())
  {
    Serial.println("Bluetooth initialized...");
    // initBLEDevice();
  }

  connectToWiFi();
}

void loop() 
{
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected. Attempting to reconnect...");
    WiFi.begin(WIFI_HOST.c_str(), WIFI_PASSWD.c_str());
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  delay(10000);
}
