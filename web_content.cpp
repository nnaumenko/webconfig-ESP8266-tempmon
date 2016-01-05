/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "web_content.h"

const char PROGMEM httpStatusCode200[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
const char PROGMEM httpStatusCode404[] = "HTTP/1.1 404 Not Found\r\n";
const char PROGMEM httpStatusCode405[] = "HTTP/1.1 405 Method Not Allowed\r\n";

const char PROGMEM webServerHeadBegin[] =
  "<!DOCTYPE html>\r\n"
  "<html>\r\n"
  "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">\r\n"
  "<head>\r\n"
  "<title>Module configuration</title>\r\n"
  "<style type=\"text/css\" media=\"all\">\r\n"
  "body {font-family:Arial;background-color:#D0FFD0;color:#000000;font-size:85%;}\r\n"
  "h1{text-align:center;font-size:120%;font-weight:bold;}\r\n"
  "h2{text-align:center;font-size:110%;font-weight:bold;}\r\n"
  "div{text-align:center;margin:auto;width:20em;padding:1em;background-color:#66FF66;border-style:solid;border-color:black;border-width:2px;}\r\n"
  "table{margin:auto;}\r\n"
  "td{text-align:left;}\r\n"
  "</style>\r\n";

const char PROGMEM webServerHeadBody[] =
  "</head>\r\n"
  "<body>\r\n"
  "<h1>Module configuration</h1>\r\n";

const char PROGMEM webServerBodyEnd[] =
  "</body>\r\n"
  "</html>\r\n";

const char PROGMEM webServerWifiFormBegin[] =
  "<div class=\"cfg_section\">\r\n"
  "<form method=\"GET\" action=\"/\">\r\n"
  "<h2>Connection settings:</h2><br>\r\n"
  "<table>\r\n";

const char PROGMEM webServerWifiFormInputBegin[] =
  "<tr><td>";

const char PROGMEM webServerWifiFormInputInput[] =
  "</td><td><input type=\"text\" name=\"";

const char PROGMEM webServerWifiFormInputValue[] =
  "\" value=\"";

const char PROGMEM webServerWifiFormInputEnd[] =
  "\"></td></tr>\r\n";

const char PROGMEM webServerWifiFormEnd[] =
  "<table><br>\r\n"
  "<input type=\"submit\" value=\"Save settings\">\r\n"
  "</form></div>\r\n";

const char PROGMEM webServerWifiFormParameterSsid[] = "ssid";
const char PROGMEM webServerWifiFormParameterPassword[] = "password";
const char PROGMEM webServerWifiFormParameterAuth[] = "auth";

const char PROGMEM webServerWifiFormNameSsid[] = "SSID";
const char PROGMEM webServerWifiFormNamePassword[] = "Password";
const char PROGMEM webServerWifiFormNameAuth[] = "Auth Token";
