#ifndef WEB_SERVER_MANAGER
#define WEB_SERVER_MANAGER

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

#include "DatabaseManager.h"

class WebServerManager
{
public: 
  WebServerManager(uint16_t port);
  ~WebServerManager();

private:
  void startServer();
  
  // POST /login
  void handleLoginRequest();

  // GET /module
  void handleGetModuleData();

  // POST /water
  void handleStartWaterPump();

  // POST /water
  void handleStopWaterPump();

private:
  AsyncWebServer* m_server;

  DatabaseManager* m_dbManager;
};

#endif // WEB_SERVER_MANAGER