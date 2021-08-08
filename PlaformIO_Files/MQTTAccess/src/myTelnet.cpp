
#include <ESPTelnet.h>

#include "myTelnet.h"
#include "myGlobal.h"
#include "myNTP.h"

// * Initiate telnet server
ESPTelnet telnet;

// Callback functions for telnet server
void onTelnetConnect(String ip) {
  Serial.print("- Telnet: ");
  Serial.print(telnet.getIP());
  Serial.println(" connected");
  telnet.println("\n\rWelcome " + telnet.getIP() + "\r");
  telnet.println("(Use ^] + q  to disconnect.)\r");
  telnet.println("\r");
  telnet.print(MQTTACCESS_MODULE_SOFTWARE_VERSION);
  telnet.println("\r");
  telnet.print(DATE_OFF_VERSION);
  telnet.println("\r");
  telnet.println("\r");
  telnet.println(ntpDateTimeStr);
  telnet.println("\r");
}

void onTelnetDisconnect(String ip) {
  Serial.print("- Telnet: ");
  Serial.print(ip);
  Serial.println(" disconnected");
}
void onTelnetReconnect(String ip) {
  Serial.print("- Telnet: ");
  Serial.print(ip);
  Serial.println(" reconnected");
}

void onTelnetConnectionAttempt(String ip) {
  Serial.print("- Telnet: ");
  Serial.print(telnet.getLastAttemptIP());
  Serial.println(" tried to connected");
}

void setupTelnet() {
  if (telnet.begin()) {
    telnet.onConnect(onTelnetConnect);
    telnet.onConnectionAttempt(onTelnetConnectionAttempt);
    telnet.onReconnect(onTelnetReconnect);
    telnet.onDisconnect(onTelnetDisconnect);
    Serial.println("- Telnet is running. Connect to Telnet using:");
    Serial.println("telnet " + ip_addr.toString());
  } else {
    // could not create server
    Serial.println("- Failed to create Telnet server. Is device connected to WiFi?");
    delay(10000);
    ESP.reset();
    delay(1000); 
  }
}

