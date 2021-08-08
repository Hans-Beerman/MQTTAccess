/*
       Copyright 2021      Hans Beerman <hans.beerman@xs4all.nl>
                          
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

   Version: 0.0.0.1
   Date:    202108088
*/

#pragma once

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// software version
#define MQTTACCESS_MODULE_SOFTWARE_VERSION "MQTTAccess module Version 0.0.0.1  Copyright Hans Beerman [2020 - 2021]"
#define DATE_OFF_VERSION "Date: 2021-08-08\n\r"

// * IP address module
extern IPAddress ip_addr;

extern char logMess[255];

extern unsigned long LAST_RECONNECT_ATTEMPT;

