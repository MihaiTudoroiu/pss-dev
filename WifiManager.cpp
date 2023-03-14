#include "WifiManager.h"

std::string WifiManager::s_host="";
std::string WifiManager::s_password="";

WifiManager::WifiManager(const std::string& host,
            const std::string& password)
{
  WifiManager::s_host = host;
  WifiManager::s_password = password;
  
  initConnection();

  spawnWifiCheckDaemon();
}

void WifiManager::initConnection()
{
  WiFi.begin(WifiManager::s_host.c_str(), WifiManager::s_password.c_str());

  delay(1000);

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

void WifiManager::checkWiFi(void * parameter)
{
  while(true) 
  {
    if (WiFi.status() != WL_CONNECTED) 
    {
      Serial.println("WiFi connection lost, trying to reconnect...");

      WiFi.begin(WifiManager::s_host.c_str(), WifiManager::s_password.c_str());

      while (WiFi.status() != WL_CONNECTED) 
      {
        delay(1000);
        Serial.println("Connecting to WiFi...");
      }
      
      Serial.println("WiFi reconnected!");
    }

    delay(10000);
  }
}

void WifiManager::spawnWifiCheckDaemon()
{
  // TODO: remove the return statement once the developing process is done
  return;

  xTaskCreatePinnedToCore(
      WifiManager::checkWiFi, /* Function to implement the task */
      "wifiCheckTask", /* Name of the task */
      10000,  /* Stack size in words */
      NULL,  /* Task input parameter */
      0,  /* Priority of the task */
      &wifiCheckTask,  /* Task handle. */
      0); /* Core where the task should run */  
}