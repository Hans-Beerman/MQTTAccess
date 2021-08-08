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

#include ".passwd.h"

// * Baud rate hardware serial
#define BAUD_RATE 115200 // baudrate for DSMR > V2.2

// * The hostname of our little creature
#define HOSTNAME "mqttaccess-module"

// * Wifi timeout in milliseconds
#define WIFI_TIMEOUT 30000
// * WiFiManager timeout in seconds
#define WIFI_MANAGER_TIMEOUT 120 // 2 minutes

// * MQTT network settings
#define MQTT_MAX_RECONNECT_TRIES 10

// * MQTT root topic
#define MQTT_ROOT_TOPIC "mqttaccess"

