#include "DatabaseManager.h"
#include "addons/TokenHelper.h"

UUID DatabaseManager::uuid = UUID();

std::string DatabaseManager::login(const std::string& userEmail, 
                                  const std::string& userPasswd)
{
  Serial.println("Connecting to database...");
  
  // Assign the api key (required)
  m_config.api_key = c_FIREBASE_KEY;

  // Assign the user sign in credentials
  m_auth.user.email = userEmail;
  m_auth.user.password = userPasswd;

  Firebase.reconnectWiFi(true);
  m_fbdo.setResponseSize(4096);

  // Assign the callback function for the long running token generation task
  m_config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  // Assign the maximum retry of token generation
  m_config.max_token_generation_retry = 5;

  // Initialize the library with the Firebase authen and config
  Firebase.begin(&m_config, &m_auth);

  if(!m_config.signer.tokens.error.message.empty())
  {
    Serial.println("Failed to authenticate to database.");

    Serial.print("Database Error Message: ");

    Serial.println(m_config.signer.tokens.error.message.c_str());

    Serial.print("Database Error Code: ");

    Serial.println(m_config.signer.tokens.error.code);

    return "";
  }
  
  while ((m_auth.token.uid) == "") 
  {
    delay(1000);
  }

  // Print user UID
  const std::string USER_UID = m_auth.token.uid.c_str();

  return USER_UID;
}

void DatabaseManager::refreshSessionToken(const std::string& session)
{
  this->m_sessionsMap[session] = std::chrono::system_clock::now();
}

void DatabaseManager::registerSession(const std::string& session)
{
  this->m_sessionsMap.insert({session, std::chrono::system_clock::now()});
}

bool DatabaseManager::isValidSession(const std::string& session)
{
  // Define a duration of 1 day
  std::chrono::duration<int, std::ratio<86400>> oneDay(1);

   if (this->m_sessionsMap[session] > oneDay)
   {
    return false;     
   }

   return true;
}

void DatabaseManager::removeOldEntries() 
{
  // Get the current time
  Timestamp now = std::chrono::system_clock::now();

  // Define a duration of 1 day
  std::chrono::duration<int, std::ratio<86400>> oneDay(1);

  // Iterate through the map and remove old entries
  for (auto it = this->m_sessionsMap.begin(); it != this->m_sessionsMap.end();) 
  {
      if (now - it->second > oneDay) 
      {
          it = this->m_sessionsMap.erase(it);
      }
      else 
      {
          ++it;
      }
  }
}
