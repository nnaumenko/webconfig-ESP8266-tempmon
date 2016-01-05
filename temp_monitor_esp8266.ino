/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include <Math.h>
#include <EEPROM.h>

#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include "eeprom_config.h"
#include "webconfig.h"
#include "web_content.h"

/*
 * Config mode SSID and password prefix
 * Config SSID will be ssidConfigModePrefix + ESP8266.ChipId
 * Config Password will be ssidConfigModePrefix + ESP8266.FlashChipId
 */

const char PROGMEM ssidConfigModePrefix[] = "ESP8266-";
const char PROGMEM passwordConfigModePrefix[] = "ESP";

/*
 * Pin mapping
 */

const int PIN_SWITCH_PROG = 0;    //GPIO0 = prog switch (+yellow LED)
const int PIN_LED_OPERATE = 2;    //GPIO2 = operate (green LED)
const int PIN_SENSOR_DHT = 4;     //GPIO4 = AM2321 signal
const int PIN_SENSOR_ONEWIRE = 5; //GPIO5 = OneWire signal
const int PIN_SWITCH_CONFIG = 12; //GPIO12 = config switch
const int PIN_LED_FAULT = 13;     //GPIO13 = fault (red LED)
const int PIN_SENSOR_MQ2 = A0;    //ADC = MQ2 analog signal

/*
 * Update timings
 */

const unsigned long UPDATE_TIME_SENSORS = 2500;//ms
const unsigned long UPDATE_TIME_STATUS_LEDS = 250;//ms
const unsigned long UPDATE_TIME_STATUS_VPINS = 499;//ms
const unsigned long UPDATE_TIME_VALUE_VPINS = 3001;//ms

/*
 * Data and status values
 */

boolean isFaultDHT = false;
boolean isFaultOneWire = false;
boolean isFaultMQ2 = false;

float valueTemperatureDHT = 0.0;
float valueHumidityDHT = 0.0;
float valueTemperatureOneWire = 0.0;
float valueGasMQ2 = 0.0;

boolean statusProgLED = false;
boolean statusFaultLED = false;
boolean statusOperateLED = false;

/*
 * DS18B20 sensor
 */

OneWire oneWire(PIN_SENSOR_ONEWIRE);
DallasTemperature sensorsDS18B20(&oneWire); // Pass our oneWire reference to Dallas Temperature.

void updateSensorOneWire(void) {
  const int ONEWIRE_SENSOR_INDEX = 0;
  const float ONEWIRE_ALARM_TRESHOULD = -127.0;
  sensorsDS18B20.requestTemperatures();
  valueTemperatureOneWire = (float)sensorsDS18B20.getTempCByIndex(ONEWIRE_SENSOR_INDEX);
  isFaultOneWire = (valueTemperatureOneWire <= ONEWIRE_ALARM_TRESHOULD);
  if (isFaultOneWire) valueTemperatureOneWire = NAN;
}

/*
 * DHT temperture/humidity sensor
 */

//#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(PIN_SENSOR_DHT, DHTTYPE, 15);

void updateSensorDHT(void) {
  valueTemperatureDHT = (float)dht.readTemperature();
  valueHumidityDHT = (float)dht.readHumidity();
  isFaultDHT = (isnan(valueTemperatureDHT)) || (isnan(valueHumidityDHT));
}

/*
 * MQ2
 */

void updateSensorMQ2(void) {
  const int MQ2_ALARM_TRESHOULD = 2;
  valueGasMQ2 = analogRead(PIN_SENSOR_MQ2);
  isFaultMQ2 = (valueGasMQ2 < MQ2_ALARM_TRESHOULD);
  if (isFaultMQ2) valueGasMQ2 = NAN;
}

/*
 * Set status LEDs on front panel
 */

void updateStatusLEDs (boolean isConfigMode) {
  //Prog LED (set externally)
  statusProgLED = !digitalRead(PIN_SWITCH_PROG);
  //Operate LED
  boolean isConnect = false;
  if (!isConfigMode) isConnect = Blynk.connected();
  statusOperateLED = isConnect;
  //Fault LED
  statusFaultLED = isFaultDHT || isFaultOneWire || isFaultMQ2;
  if (!isConfigMode) statusFaultLED = statusFaultLED || (!Blynk.connected());
  //Set final LED statuses
  //Note: true = LED off, false = LED on
  //Prog led is set either by DIP-switch or by setup() function in Config Mode
  digitalWrite(PIN_LED_OPERATE, !statusOperateLED);
  digitalWrite(PIN_LED_FAULT, !statusFaultLED);
}

/*
 * Send virtual pins state to Blynk server
 * Setting all the virtual pins at once will result in too flood error
 * To prevent disconnecting from server, data are sent one at a time
 */

void updateValueVirtualPins(void) {
  static int currentPhase = 0;
  switch (currentPhase) {
    case 0:
      //V10 = DHT tempertaure
      Blynk.virtualWrite(V10, valueTemperatureDHT);
      break;
    case 1:
      //V11 = DHT humidity
      Blynk.virtualWrite(V11, valueHumidityDHT);
      break;
    case 2:
      //V12 = OneWire temperature
      Blynk.virtualWrite(V12, valueTemperatureOneWire);
      break;
    case 3:
      //V13 = MQ2 signal
      Blynk.virtualWrite(V13, valueGasMQ2);
      break;
  }
  currentPhase++;
  if (currentPhase > 3) currentPhase = 0;
}

void updateStatusVirtualPins(void) {
  static boolean lifeBit = false;
  const int BLYNK_WIDGET_LED_ON = 255;
  const int BLYNK_WIDGET_LED_OFF = 0;
  static int currentPhase = 0;
  switch (currentPhase) {
    case 0:
      //V4 = DHT fault
      Blynk.virtualWrite(V4, isFaultDHT ? BLYNK_WIDGET_LED_ON : BLYNK_WIDGET_LED_OFF);
      break;
    case 1:
      //V5 = OneWire fault
      Blynk.virtualWrite(V5, isFaultOneWire ? BLYNK_WIDGET_LED_ON : BLYNK_WIDGET_LED_OFF);
      break;
    case 2:
      //V6 = MQ2 fault
      Blynk.virtualWrite(V6, isFaultMQ2 ? BLYNK_WIDGET_LED_ON : BLYNK_WIDGET_LED_OFF);
      break;
    case 3:
      //V7 = lifebit
      lifeBit = !lifeBit;
      Blynk.virtualWrite(V7, lifeBit ? BLYNK_WIDGET_LED_ON : BLYNK_WIDGET_LED_OFF);
      break;
  }
  currentPhase++;
  if (currentPhase > 3) currentPhase = 0;
}

void printSensorDebugInfo(void) {
  Serial.print(F("["));
  Serial.print(millis());
  Serial.print(F("] sensor values:"));
  Serial.print(F(" DHT T = "));
  Serial.print(valueTemperatureDHT);
  Serial.print(F(" RH = "));
  Serial.print(valueHumidityDHT);
  Serial.print(F(" OneWire(0) T = "));
  Serial.print(valueTemperatureOneWire);
  Serial.print(F(" MQ2 AI = "));
  Serial.print(valueGasMQ2);
  Serial.println();
}

boolean checkTimedEvent (const unsigned long period, unsigned long * tempTimeValue) {
  if ((millis() - (*tempTimeValue)) < period) return false;
  *tempTimeValue = millis();
  return (true);
}

boolean isConfigMode = false;

void setup() {
  Serial.begin(9600);
  Serial.println(F("\nInit started."));

  pinMode(PIN_SWITCH_PROG, INPUT);
  pinMode(PIN_SWITCH_CONFIG, INPUT);
  pinMode(PIN_LED_OPERATE, OUTPUT);
  pinMode(PIN_LED_FAULT, OUTPUT);
  
  char chipId[16];
  const int RADIX_DECIMAL = 10;
  ltoa(ESP.getChipId(), chipId, RADIX_DECIMAL);
  char flashId[16];
  ltoa(ESP.getFlashChipId(), flashId, RADIX_DECIMAL);
  Serial.print(F("Chip ID: "));
  Serial.println(chipId);
  Serial.print(F("Flash chip ID:"));
  Serial.println(flashId);

  isConfigMode = !digitalRead(PIN_SWITCH_CONFIG);
  loadConfig();
  if (isConfigMode) {
    pinMode(PIN_SWITCH_PROG, OUTPUT);
    digitalWrite(PIN_SWITCH_PROG, false);
    Serial.println(F("Config Mode enabled."));
    const int TEXT_SIZE = 32;
    char ssid[TEXT_SIZE + 1] = {0};
    strncpy_P(ssid, ssidConfigModePrefix, sizeof(ssid));
    strncat(ssid, chipId, sizeof(ssid) - strlen(ssid));
    char password[TEXT_SIZE + 1] = {0};
    strncpy_P(password, passwordConfigModePrefix, sizeof(password));
    strncat(password, flashId, sizeof(password) - strlen(password));
    WiFi.softAP(ssid, password);
    Serial.println(F("Access point created: "));
    Serial.print(F("SSID: "));
    Serial.println(ssid);
    Serial.print(F("Password: "));
    Serial.println(password);
    Serial.print(F("IP address: "));
    Serial.println(WiFi.softAPIP());
    webServerBegin();
    Serial.println(F("Web server started."));
  }
  else {
    Serial.println(F("Config Mode not enabled."));
    Blynk.begin(authToken, wifiSsid, wifiPassword);
    Serial.println(F("Blynk init completed."));
  }
  dht.begin();
  sensorsDS18B20.begin();
  Serial.println(F("Init completed."));
}

void loop() {
  
  static unsigned long lastMillisSensors = 0;
  if (checkTimedEvent(UPDATE_TIME_SENSORS, &lastMillisSensors)) {
    updateSensorDHT();
    updateSensorOneWire();
    updateSensorMQ2();
    printSensorDebugInfo();
  }
  
  static unsigned long lastMillisStatusLEDs = 0;
  if (checkTimedEvent(UPDATE_TIME_STATUS_LEDS, &lastMillisStatusLEDs))
    updateStatusLEDs(isConfigMode);
    
  if (!isConfigMode) {
    static unsigned long lastMillisStatusVirtualPins = 0;
    if (checkTimedEvent(UPDATE_TIME_STATUS_VPINS, &lastMillisStatusVirtualPins))
      updateStatusVirtualPins();
    static unsigned long lastMillisValueVirtualPins = 0;
    if (checkTimedEvent(UPDATE_TIME_VALUE_VPINS, &lastMillisValueVirtualPins))
      updateValueVirtualPins();
    Blynk.run();
  }
  else {
    webServerRun();
  }
}


