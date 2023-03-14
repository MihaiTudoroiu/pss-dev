#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <string>

#include <WiFi.h>

class WifiManager
{
public:
  WifiManager(const std::string& host,
              const std::string& password);

private:

  void initConnection();

  static void checkWiFi(void * parameter);

  void spawnWifiCheckDaemon();

public:

    static std::string s_host;
    static std::string s_password;

private:

    TaskHandle_t wifiCheckTask;
};

#endif // WIFI_MANAGER_H