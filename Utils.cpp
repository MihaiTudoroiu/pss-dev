#include "Utils.h"
#include <EEPROM.h>

std::string readFromMemory(const int address)
{
  // EEPROM.begin(512);

  short max_wifi_length = 64;

  // Read the string from the EEPROM
  std::string storedStr = "";
  int i = 0;
  char c = EEPROM.read(address + i);
  
  while (c != '\0' && i < max_wifi_length) 
  {
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