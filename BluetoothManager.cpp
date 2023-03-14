#include "BluetoothManager.h"
#include <EEPROM.h>

BluetoothCallback::BluetoothCallback(Stream *streamObject) : m_streamRef(streamObject), BLECharacteristicCallbacks(){};
BluetoothCallback::~BluetoothCallback()
{
  delete m_streamRef;  
}

void BluetoothCallback::onWrite(BLECharacteristic *pCharacteristic) 
{
  std::string value = pCharacteristic->getValue();

  std::string WIFI_HOST;
  std::string WIFI_PASSWD;

  if (pCharacteristic->getUUID().toString() == WIFI_HOST_UUID) 
  {
    m_streamRef->print("Wifi Host: ");
    m_streamRef->println(value.c_str());

    WIFI_HOST = value;

    // Write the WIFI host string to the EEPROM
    for (int i = 0; i < value.length(); i++) 
    {
      EEPROM.write(address_wifi_host + i, value[i]);
    }

    EEPROM.write(address_wifi_host + value.length(), '\0');

    EEPROM.commit();
  } 
  else if (pCharacteristic->getUUID().toString() == WIFI_PASSWD_UUID) 
  {
    m_streamRef->print("Wifi Passwd: ");
    m_streamRef->println(value.c_str());

    WIFI_PASSWD = value;

    // Write the WIFI passwd string to the EEPROM
    for (int i = 0; i < value.length(); i++) 
    {
      EEPROM.write(address_wifi_passwd + i, value[i]);
    }

    EEPROM.write(address_wifi_passwd + value.length(), '\0');

    EEPROM.commit();
  }

  // Add the condition to stop the BLE server
  if (!WIFI_HOST.empty() && !WIFI_PASSWD.empty()) 
  {
    // Remove the BLE server
    m_streamRef->println("BLE server stopped.");
    BLEDevice::deinit();
  }
}

BluetoothManager::BluetoothManager(Stream *streamObject) : m_streamRef(streamObject)
{
  initBLEDevice();
}

BluetoothManager::~BluetoothManager()
{
  delete m_streamRef;
  delete m_pServer;
  delete m_pWifiHostChar;
  delete m_pWifiPasswdChar;
}

void BluetoothManager::initBLEDevice()
{
  BLEDevice::init(DEVICE_NAME);

  m_pServer = BLEDevice::createServer();

  BLEService* pService = m_pServer->createService("WiFi");

  m_pWifiHostChar = pService->createCharacteristic(
    BluetoothCallback::WIFI_HOST_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
  );

  m_pWifiHostChar->setValue("Host");

  m_pWifiHostChar->setCallbacks(new BluetoothCallback(m_streamRef));

  m_pWifiPasswdChar = pService->createCharacteristic(
    BluetoothCallback::WIFI_PASSWD_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
  );

  m_pWifiPasswdChar->setValue("Passwd");
  
  m_pWifiPasswdChar->setCallbacks(new BluetoothCallback(m_streamRef));

  pService->start();
  m_pServer->getAdvertising()->start();

  m_streamRef->println("Bluetooth server started");
}  