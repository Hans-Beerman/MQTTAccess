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

#include <ESP8266WiFi.h>
#include <WiFiManager.h>

#include "myWiFi.h"
#include "myGlobal.h"
#include "MyEEprom.h"
#include "settingsMain.h"

#define USE_DEFAULT_WIFI_SETTINGS true

// * To be filled with EEPROM data

char MQTT_HOST[64] = MY_MQTT_HOST;
char MQTT_PORT[6]  = MY_MQTT_PORT;
char MQTT_USER[32] = MY_MQTT_USERNAME;
char MQTT_PASS[32] = MY_MQTT_PASSWORD;

// * Gets called when WiFiManager enters configuration mode
void configModeCallback(WiFiManager *myWiFiManager) {
  Serial.println(F("Entered config mode"));
  Serial.println(WiFi.softAPIP());

  // * If you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

// * Callback for saving WIFI config
bool shouldSaveConfig = false;

// * Callback notifying us of the need to save config
void save_wifi_config_callback () {
  Serial.println(F("Should save config"));
  shouldSaveConfig = true;
}

// * Setup WiFi function
void setup_WiFi() {
  if (USE_DEFAULT_WIFI_SETTINGS) {
    // to reset the WiFi credentials, uncomment next line:
    // WiFi.disconnect(true);

    WiFi.begin(WIFI_NETWORK, WIFI_PASSWD);
    // Try up to del seconds to get a WiFi connection; and if that fails,setup AP
    // with a bit of a delay.
    //
    const int del = WIFI_TIMEOUT; // milliseconds.
    unsigned long start = millis();
    Serial.print("Connecting..");
    while ((WiFi.status() != WL_CONNECTED) && (millis() - start < del)) {
      delay(500);
      Serial.print(",");
    };
    Serial.print("\n\r");
  }

  if (!USE_DEFAULT_WIFI_SETTINGS || (WiFi.status() != WL_CONNECTED)) {
    // * WiFiManager local initialization. Once its business is done, there is no need to keep it around

    // * Get MQTT Server settings
    String settings_available = read_eeprom(134, 1);

    if (settings_available == "1") {
        read_eeprom(0, 64).toCharArray(MQTT_HOST, 64);   // * 0-63
        read_eeprom(64, 6).toCharArray(MQTT_PORT, 6);    // * 64-69
        read_eeprom(70, 32).toCharArray(MQTT_USER, 32);  // * 70-101
        read_eeprom(102, 32).toCharArray(MQTT_PASS, 32); // * 102-133
    }

    WiFiManagerParameter CUSTOM_MQTT_HOST("host", "MQTT hostname", MQTT_HOST, 64);
    WiFiManagerParameter CUSTOM_MQTT_PORT("port", "MQTT port",     MQTT_PORT, 6);
    WiFiManagerParameter CUSTOM_MQTT_USER("user", "MQTT user",     MQTT_USER, 32);
    WiFiManagerParameter CUSTOM_MQTT_PASS("pass", "MQTT pass",     MQTT_PASS, 32);

    WiFiManager wifiManager;
    // * Reset settings - uncomment for testing
    // wifiManager.resetSettings();


    // * Set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
    wifiManager.setAPCallback(configModeCallback);

    // * Set timeout
    wifiManager.setConfigPortalTimeout(WIFI_MANAGER_TIMEOUT);

    // * Set save config callback
    wifiManager.setSaveConfigCallback(save_wifi_config_callback);

        // * Add all your parameters here
    wifiManager.addParameter(&CUSTOM_MQTT_HOST);
    wifiManager.addParameter(&CUSTOM_MQTT_PORT);
    wifiManager.addParameter(&CUSTOM_MQTT_USER);
    wifiManager.addParameter(&CUSTOM_MQTT_PASS);

    wifiManager.setTimeout(WIFI_MANAGER_TIMEOUT);

    // * Fetches SSID and pass and tries to connect
    // * Reset when no connection after 10 seconds
    if (!wifiManager.autoConnect(FALLBACK_WIFI_NETWORK, FALLBACK_WIFI_PASSWD)) {
      Serial.printf("No connection after %d seconds (ssid=%s). Rebooting.\n\r", WIFI_TIMEOUT, WiFi.SSID().c_str());
      Serial.print("Rebooting...\n\r");
      delay(1000);
    // * Reset and try again, or maybe put it to deep sleep
      ESP.reset();
    }
    // WiFi connection established
    // * Save the custom parameters to FS
    // * Read updated parameters
    snprintf(MQTT_HOST, sizeof(MQTT_HOST), "%s", CUSTOM_MQTT_HOST.getValue());
    snprintf(MQTT_PORT, sizeof(MQTT_PORT), "%s", CUSTOM_MQTT_PORT.getValue());
    snprintf(MQTT_USER, sizeof(MQTT_USER), "%s", CUSTOM_MQTT_USER.getValue());
    snprintf(MQTT_PASS, sizeof(MQTT_PASS), "%s", CUSTOM_MQTT_PASS.getValue());

    // * Save the custom parameters to FS
    if (shouldSaveConfig) {
      Serial.println(F("Saving WiFiManager config"));

      write_eeprom(0, 64, MQTT_HOST);   // * 0-63
      write_eeprom(64, 6, MQTT_PORT);   // * 64-69
      write_eeprom(70, 32, MQTT_USER);  // * 70-101
      write_eeprom(102, 32, MQTT_PASS); // * 102-133
      write_eeprom(134, 1, "1");        // * 134 --> always "1"
      EEPROM.commit();
    }
  }

  // * If you get here you have connected to the WiFi
  Serial.println(F("Connected to WIFI..."));
  if(WiFi.SSID().c_str()) {
    Serial.printf("SSID = <%s>\n\r", WiFi.SSID().c_str());
  }

  ip_addr = WiFi.localIP();

  // * If you get here you have connected to the WiFi
  snprintf(logMess, sizeof(logMess), "IP address = %s\n\r", ip_addr.toString().c_str());
  Serial.print(logMess);
}
