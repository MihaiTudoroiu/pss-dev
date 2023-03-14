#include "WebServerManager.h"
#include "DatabaseManager.h"
#include "WifiManager.h"
#include "BluetoothManager.h"
#include "Utils.h"

// All global variables must reside in this area

WebServerManager* wbServer = nullptr;

////////////////////////////////////////////////

void setup() {
  Serial.begin(115200);

  // Those must be read from bluetooth connection
  std::string WIFI_HOST = "iPhone-Mihai"; // Halo // TP-Link_B070 // iPhone-Mihai // ASUS
  std::string WIFI_PASSWD = "123455aa"; // 82532437 // 27477885 // 123455aa

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
    //BluetoothManager bleManager;
  }

  WifiManager wifi(WIFI_HOST, WIFI_PASSWD);

  wbServer = new WebServerManager(80); 	
}

void loop() 
{
  delay(1);
}
