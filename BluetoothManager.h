#ifndef BLUETOOTH_MANAGER
#define BLUETOOTH_MANAGER

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <Stream.h>

class BluetoothCallback: public BLECharacteristicCallbacks 
{
public:
  BluetoothCallback(Stream *streamObject);
  ~BluetoothCallback();

private:
  void onWrite(BLECharacteristic *pCharacteristic);

  Stream* m_streamRef;

  const int address_wifi_host = 0;
  const int address_wifi_passwd = 100;
  
public:

  static constexpr char* WIFI_HOST_UUID = "108bdd67-3b58-469b-b207-eb55d26357e4";
  static constexpr char* WIFI_PASSWD_UUID = "4eddb67b-5382-45bf-9cf3-8171d441851b";
};

class BluetoothManager
{
public:

  BluetoothManager(Stream *streamObject);
  ~BluetoothManager();

private:

  void initBLEDevice();

private:

  Stream* m_streamRef;
  BLEServer* m_pServer;
  BLECharacteristic* m_pWifiHostChar;
  BLECharacteristic* m_pWifiPasswdChar;

  const char* DEVICE_NAME = "Plant supervisor";
};

#endif // BLUETOOTH_MANAGER
