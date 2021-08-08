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

#include "myMQTT.h"
#include "myGlobal.h"
#include "myTelnet.h"
#include "settingsMain.h"


// * Initiate WIFI client
WiFiClient espClient = WiFiClient();

PubSubClient myMQTT_client = PubSubClient(espClient);

void myMQTT_setup(void) {
  // * Setup MQTT
  Serial.printf("MQTT connecting to: %s:%s\n", MQTT_HOST, MQTT_PORT);
  myMQTT_client.setServer(MQTT_HOST, atoi(MQTT_PORT));
}

bool myMQTT_connected(void) {
  return myMQTT_client.connected();
}

// * Send a message to a broker topic
void myMQTT_send_mqtt_message(const char *topic, char *payload) {
  Serial.printf("MQTT Outgoing on %s: ", topic);
  Serial.println(payload);

  bool result = myMQTT_client.publish(topic, payload, false);

  if (!result) {
    Serial.printf("MQTT publish to topic %s failed\n", topic);
  }
}

// * Reconnect to MQTT server and subscribe to in and out topics
bool myMQTT_reconnect() {
  // * Loop until we're reconnected
  int MQTT_RECONNECT_RETRIES = 0;

  while (!myMQTT_client.connected() && MQTT_RECONNECT_RETRIES < MQTT_MAX_RECONNECT_TRIES) {
    MQTT_RECONNECT_RETRIES++;
    Serial.printf("MQTT connection attempt %d / %d ...\n", MQTT_RECONNECT_RETRIES, MQTT_MAX_RECONNECT_TRIES);
    snprintf(logMess, sizeof(logMess), "MQTT connection attempt %d / %d ...\n\r", MQTT_RECONNECT_RETRIES, MQTT_MAX_RECONNECT_TRIES);
    telnet.println(logMess);


    // * Attempt to connect
    if (myMQTT_client.connect(HOSTNAME, MQTT_USER, MQTT_PASS)) {
      Serial.println(F("MQTT connected!"));
      telnet.println(F("MQTT connected!\r"));

      // * Once connected, publish an announcement...
      snprintf(logMess, sizeof(logMess), "p1 port module alive, hostname: %s", HOSTNAME);
      myMQTT_client.publish("Status", logMess);

      Serial.printf("MQTT root topic: %s\n", MQTT_ROOT_TOPIC);
      snprintf(logMess, sizeof(logMess), "MQTT root topic: %s\n\r", MQTT_ROOT_TOPIC);
      telnet.println(logMess);

    } else {
      Serial.print(F("MQTT Connection failed: rc="));
      Serial.println(myMQTT_client.state());
      Serial.println(F(" Retrying in 2 seconds"));
      Serial.println("");
      snprintf(logMess, sizeof(logMess), "MQTT Connection failed: rc=%d\r", myMQTT_client.state());
      telnet.println(logMess);
      telnet.println(F(" Retrying in 2 seconds\r"));
      telnet.println("\r");
      delay(2000);
    }
  }

  if (MQTT_RECONNECT_RETRIES >= MQTT_MAX_RECONNECT_TRIES) {
    Serial.printf("*** MQTT connection failed, giving up after %d tries ...\n", MQTT_RECONNECT_RETRIES);
    Serial.println(F(" Retrying in 10 seconds"));
    snprintf(logMess, sizeof(logMess), "*** MQTT connection failed, giving up after %d tries ...\n", MQTT_RECONNECT_RETRIES);
    telnet.println(logMess);
    telnet.println(F(" Retrying in 10 seconds\r"));
    return false;
  }

  return true;
}

void myMQTT_loop(void) {
  myMQTT_client.loop();
}
