/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "eeprom_config.h"

#include <EEPROM.h>

char authToken[CONFIG_TEXT_LENGTH + 1] = {0};
char wifiSsid[CONFIG_TEXT_LENGTH + 1] = {0};
char wifiPassword[CONFIG_TEXT_LENGTH + 1] = {0};

const int EEPROM_ADDRESS_AUTH_TOKEN = 0;
const int EEPROM_ADDRESS_WIFI_SSID = CONFIG_TEXT_LENGTH;
const int EEPROM_ADDRESS_WIFI_PASSWORD = CONFIG_TEXT_LENGTH * 2;

const int SIZE_OF_EEPROM_DATA = CONFIG_TEXT_LENGTH * 3; //3x char array,

void saveTextToEEPROM (int address, const char * text, size_t length) {
  for (int i = 0; i < length; i++) {
    EEPROM.write(address + i, text[i]);
  }
}

void loadTextFromEEPROM (int address, char * text, size_t length) {
  for (int i = 0; i < length; i++) {
    text[i] = EEPROM.read(address + i);
  }
}

void saveConfig(void) {
  EEPROM.begin(SIZE_OF_EEPROM_DATA);
  saveTextToEEPROM(EEPROM_ADDRESS_AUTH_TOKEN, authToken, CONFIG_TEXT_LENGTH);
  saveTextToEEPROM(EEPROM_ADDRESS_WIFI_SSID, wifiSsid, CONFIG_TEXT_LENGTH);
  saveTextToEEPROM(EEPROM_ADDRESS_WIFI_PASSWORD, wifiPassword, CONFIG_TEXT_LENGTH);
  EEPROM.end();
  Serial.print(F("["));
  Serial.print(millis());
  Serial.println(F("] Config saved to EEPROM."));
}

void loadConfig(void) {
  EEPROM.begin(SIZE_OF_EEPROM_DATA);
  loadTextFromEEPROM(EEPROM_ADDRESS_AUTH_TOKEN, authToken, CONFIG_TEXT_LENGTH);
  loadTextFromEEPROM(EEPROM_ADDRESS_WIFI_SSID, wifiSsid, CONFIG_TEXT_LENGTH);
  loadTextFromEEPROM(EEPROM_ADDRESS_WIFI_PASSWORD, wifiPassword, CONFIG_TEXT_LENGTH);
  EEPROM.end();
  Serial.print(F("["));
  Serial.print(millis());
  Serial.println(F("] Config loaded from EEPROM."));
  //  Serial.print(F("Auth token: "));
  //  Serial.println(authToken);
  //  Serial.print(F("WIFI network:"));
  //  Serial.println(wifiNetwork);
  //  Serial.print(F("WIFI password:"));
  //  Serial.println(wifiPassword);
}
