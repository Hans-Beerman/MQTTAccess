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

#include <WiFiUdp.h>
#include <NTP.h> // install NTP by Stefan Staub
//
// information about NTP.h, see: https://platformio.org/lib/show/5438/NTP
//

// * for NTP

#include "myNTP.h"
#include "myGlobal.h"
#include "myTelnet.h"

#define NTP_UPDATE_WINDOW (1000 * 60 * 60) // in ms = 1 hour

WiFiUDP wifiUDP;

NTP ntp(wifiUDP);

char ntpDateTimeStr[64];
char newDateTimeStr[64];
unsigned long NTPUpdatedTime = 0;
int currentHour;
int currentMinutes;
int currentSecs;
int currentDay;
int currentMonth;
int currentYear;

void initNTP() {
  ntp.ruleDST("CEST", Last, Sun, Mar, 2, 120); // last sunday in march 2:00, timezone +120min (+1 GMT + 1h summertime offset)
  ntp.ruleSTD("CET", Last, Sun, Oct, 3, 60); // last sunday in october 3:00, timezone +60min (+1 GMT)
  ntp.begin(false); // start NTP nonblocking
  ntp.update();

  currentHour = ntp.hours();
  currentMinutes = ntp.minutes();
  currentSecs = ntp.seconds();
  currentDay = ntp.day();
  currentMonth = ntp.month();
  currentYear = ntp.year();

  snprintf(ntpDateTimeStr, sizeof(ntpDateTimeStr), "Last boot: %4d-%02d-%02d %02d:%02d:%02d", currentYear, currentMonth, currentDay, currentHour, currentMinutes, currentSecs);

  ntp.updateInterval(NTP_UPDATE_WINDOW);
  ntp.update();
}

void getCurrentNTPTime() {
  currentHour = ntp.hours();
  currentMinutes = ntp.minutes();
  currentSecs = ntp.seconds();
  currentDay = ntp.day();
  currentMonth = ntp.month();
  currentYear = ntp.year();

  snprintf(newDateTimeStr, sizeof(newDateTimeStr), "%4d-%02d-%02d %02d:%02d:%02d", currentYear, currentMonth, currentDay, currentHour, currentMinutes, currentSecs);
}

void ntpUpdateLoop(void) {
  ntp.update();
}