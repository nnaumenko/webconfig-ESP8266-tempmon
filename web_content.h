/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#ifndef WEB_CONTENT_H
#define WEB_CONTENT_H

#include <Arduino.h>

extern const char PROGMEM httpStatusCode200[];
extern const char PROGMEM httpStatusCode404[];
extern const char PROGMEM httpStatusCode405[];

extern const char PROGMEM webServerHeadBegin[];
extern const char PROGMEM webServerHeadBody[];
extern const char PROGMEM webServerBodyEnd[];

extern const char PROGMEM webServerWifiFormBegin[];
extern const char PROGMEM webServerWifiFormInputBegin[];
extern const char PROGMEM webServerWifiFormInputInput[];
extern const char PROGMEM webServerWifiFormInputValue[];
extern const char PROGMEM webServerWifiFormInputEnd[];
extern const char PROGMEM webServerWifiFormEnd[];

extern const char PROGMEM webServerWifiFormParameterSsid[];
extern const char PROGMEM webServerWifiFormParameterPassword[];
extern const char PROGMEM webServerWifiFormParameterAuth[];

extern const char PROGMEM webServerWifiFormNameSsid[];
extern const char PROGMEM webServerWifiFormNamePassword[];
extern const char PROGMEM webServerWifiFormNameAuth[];


#endif
