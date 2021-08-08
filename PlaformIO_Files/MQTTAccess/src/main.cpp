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

#include <Arduino.h>
#include <FS.h>
#include <EEPROM.h>
#include <DNSServer.h>
#include <Ticker.h>
#include <ESP8266mDNS.h>

/*
*****************************************************************************
PLEASE READ THIS FIRST

Add a file to the include directory of this project with the 
following name:

.passwd.h

The content of this file should be:

#pragma once

// WiFi credentials
#define WIFI_NETWORK "YOUR_WIFI_NETWORK_NAME"
#define WIFI_PASSWD "YOUR_WIFI_NETWORK_PASSWORD"

#define FALLBACK_WIFI_NETWORK "Dummy"  // or chose any other name you like
#define FALLBACK_WIFI_PASSWD "12345678" // or take another password

// mqtt server
#define MY_MQTT_HOST "your_mqtt_host e.g. 10.10.10.10"
#define MY_MQTT_PORT "1883"
#define MY_MQTT_USERNAME "your_mqtt_server_username"
#define MY_MQTT_PASSWORD "your_mqtt_server.password"

// OTA credentials
#define OTA_PASSWORD "MyPassW00rd" // must be the same in platformio.ini

Also check in the platformio.ini file the upload_port set.

In the current file this port is set for a serial connection to a

Linux Ubuntu PC for a Windows PC running Visual Studio Code with
PlatformIO you should use something like:
upload_port = COM3
Or any other comport number which is used for the serial connection

For further information see the README.md in the repository on GitHub

*****************************************************************************
*/

// * Include settings
#include "settingsMain.h"
#include ".passwd.h"
#include "myGlobal.h"
#include "myWiFi.h"
#include "myNTP.h"
#include "myTelnet.h"
#include "myMQTT.h"
#include "myOTA.h"

// * include EEProm functions
#include "MyEEprom.h"

#define MODULE_NAME "MQTTAccessModule"

// * Initiate led blinker library
Ticker ticker;

char topic[255];

// **********************************
// * Function definitions           *
// **********************************


// **********************************
// * Ticker (System LED Blinker)    *
// **********************************

// * Blink on-board Led
void tick() {
  // * Toggle state
  int state = digitalRead(LED_BUILTIN);    // * Get the current state of GPIO1 pin
  digitalWrite(LED_BUILTIN, !state);       // * Set pin to the opposite state
}

// **********************************
// * Setup MDNS discovery service   *
// **********************************

void setup_mdns() {
  Serial.println(F("Starting MDNS responder service"));

  bool mdns_result = MDNS.begin(HOSTNAME);
  if (mdns_result) {
      MDNS.addService("http", "tcp", 80);
  }
}

// **********************************
// * Setup Main                     *
// **********************************

void setup() {
  Serial.begin(BAUD_RATE); 

  Serial.printf("\n\r\n\r");
  Serial.print(MQTTACCESS_MODULE_SOFTWARE_VERSION);
  Serial.println("\r");
  Serial.printf("\n\r\n\rSerial port = %d b/s\n\r", Serial.baudRate());

  // * Configure EEPROM
  EEPROM.begin(512);
    
    // * Set led pin as output
  pinMode(LED_BUILTIN, OUTPUT);

  // * Start ticker with 0.5 because we start in AP mode and try to connect
  ticker.attach(0.5, tick);


  // to reset the WiFi credentials, uncomment next line:
  // WiFi.disconnect(true);

  setup_WiFi();

  ip_addr = WiFi.localIP();

  // * Keep LED on
  ticker.detach();
  digitalWrite(LED_BUILTIN, LOW);

  setupTelnet();

  // * Start ticker with 1.0 after connection
  ticker.attach(1.0, tick);

  // * Configure OTA
  setup_ota();

  // * Startup MDNS Service
  setup_mdns();

  myMQTT_setup();

  initNTP();
}

// **********************************
// * send test MQTT messages loop   *
// **********************************

unsigned long lastMQTTMessageSent = 0;

char testMQTTMess[255];

void sendTestMQTTMessages_loop(void) {
  unsigned long now = millis();

  if ((now - lastMQTTMessageSent) >= 1000) {
    getCurrentNTPTime();
    snprintf(testMQTTMess, sizeof(testMQTTMess), "%02d-%02d-%04d %02d:%02d:%02d", currentDay, currentMonth, currentYear, currentHour, currentMinutes, currentSecs);
    myMQTT_send_mqtt_message("mqttaccess/date_time", testMQTTMess);
    lastMQTTMessageSent = now;
  }
}


// **********************************
// * Loop                           *
// **********************************

bool firstTime = true;

unsigned long now;

bool MQTTFirstTime = true;

void loop()
{
  ota_loop();

  telnet.loop();

  ntpUpdateLoop();

  if (!myMQTT_connected()) {
    now = millis();

    if (((now - LAST_RECONNECT_ATTEMPT) > 30000) || MQTTFirstTime) {
      MQTTFirstTime = false;
      LAST_RECONNECT_ATTEMPT = now;
      Serial.println("mqtt client not connected\r");
      telnet.println("mqtt client not connected\r");

      if (myMQTT_reconnect())
      {
        MQTTFirstTime = true;
      }
    }
  } else {
    if (firstTime) {
      Serial.println("First time = true \r");
      firstTime = false;
      snprintf(logMess, sizeof(logMess), "p1 port module alive, hostname: %s", HOSTNAME);
      myMQTT_send_mqtt_message("Status", logMess);
      telnet.print(logMess);
      telnet.print("\n\r");
    }
  }
    
  myMQTT_loop();

  sendTestMQTTMessages_loop();
}
