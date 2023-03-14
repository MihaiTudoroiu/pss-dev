#include "WebServerManager.h"

#include <ArduinoJson.h>
#include <AsyncJson.h>

WebServerManager::WebServerManager(uint16_t port)
{
  m_server = new AsyncWebServer(port);

  m_dbManager = new DatabaseManager();

  startServer();
}

WebServerManager::~WebServerManager()
{
  delete m_server;
}

void WebServerManager::handleLoginRequest()
{
  AsyncCallbackJsonWebHandler* handler = new AsyncCallbackJsonWebHandler("/login", [this](AsyncWebServerRequest *request, JsonVariant &json) 
  {
    Serial.println("enter async callback json");

    JsonObject jsonObj = json.as<JsonObject>();
    
    const std::string email = jsonObj["email"];
    const std::string password = jsonObj["password"];

    // Validate input data
    if(email.empty() || password.empty())
    {
      request->send(400, "text/plain", "Bad request params.");

      return;
    }

    const std::string user_UID = m_dbManager->login(email, password);

    if(user_UID.empty())
    {
      request->send(401, "text/plain", "Unauthorized");

      return;
    }

    const std::string userSessionKey = DatabaseManager::generateSessionId();

    m_dbManager->registerSession(userSessionKey);

    AsyncJsonResponse * response = new AsyncJsonResponse();
    // response->addHeader("Server","ESP Async Web Server");
    JsonObject root = response->getRoot();
    
    root["session_key"] = userSessionKey;
    response->setLength();
    request->send(response);
  });

  m_server->addHandler(handler);
}

void WebServerManager::startServer()
{
  handleLoginRequest();
  
  m_server->begin(); 		
}