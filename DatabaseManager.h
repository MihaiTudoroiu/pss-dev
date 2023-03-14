#ifndef DATABASE_MANAGER
#define DATABASE_MANAGER

#include <Arduino.h>
#include <Firebase_ESP_Client.h>
#include <unordered_map>
#include <chrono>
#include "UUID.h"

using Timestamp = std::chrono::system_clock::time_point;

class DatabaseManager
{
public:

  static UUID uuid;

  static std::string generateSessionId() 
  {
    uuid.generate();

    return uuid.toCharArray();
  }

public:

  std::string login(const std::string& userEmail, const std::string& userPasswd);

  void refreshSessionToken(const std::string& session);

  void registerSession(const std::string& session);

  bool isValidSession(const std::string& session);

  void removeOldEntries();

private:

  FirebaseData m_fbdo;
  FirebaseAuth m_auth;
  FirebaseConfig m_config;

  std::string c_FIREBASE_KEY = "AIzaSyDjZ5u5KemOQA9vYf-PQRUsAngVw1pKD9U";

  std::unordered_map<std::string, Timestamp> m_sessionsMap;
};

#endif // DATABASE_MANAGER