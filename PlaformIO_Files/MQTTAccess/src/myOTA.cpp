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

#include <ArduinoOTA.h>

#include "myOTA.h"
#include "settingsMain.h"

// **********************************
// * Setup OTA                      *
// **********************************

void setup_ota() {
  Serial.println(F("Arduino OTA activated."));

  // * Port defaults to 8266
  ArduinoOTA.setPort(8266);

  // * Set hostname for OTA
  ArduinoOTA.setHostname(HOSTNAME);
  ArduinoOTA.setPassword(OTA_PASSWORD);

  ArduinoOTA.onStart([]() {
    Serial.println(F("Arduino OTA: Start"));
  });

  ArduinoOTA.onEnd([]() {
    Serial.println(F("Arduino OTA: End (Running reboot)"));
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Arduino OTA Progress: %u%%\r", (progress / (total / 100)));
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Arduino OTA Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println(F("Arduino OTA: Auth Failed"));
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println(F("Arduino OTA: Begin Failed"));
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println(F("Arduino OTA: Connect Failed"));
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println(F("Arduino OTA: Receive Failed"));
    } else if (error == OTA_END_ERROR) {
      Serial.println(F("Arduino OTA: End Failed"));
    }
  });

  ArduinoOTA.begin();
  Serial.println(F("Arduino OTA finished"));
}

void ota_loop(void) {
  ArduinoOTA.handle();
}
