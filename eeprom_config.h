/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#ifndef EEPROM_CONFIG_H
#define EEPROM_CONFIG_H

#include <Arduino.h>

#define CONFIG_TEXT_LENGTH 32 //NOT including trailing 0 character

extern char authToken[CONFIG_TEXT_LENGTH + 1];
extern char wifiSsid[CONFIG_TEXT_LENGTH + 1];
extern char wifiPassword[CONFIG_TEXT_LENGTH + 1];

void saveConfig(void);
void loadConfig(void);

#endif
