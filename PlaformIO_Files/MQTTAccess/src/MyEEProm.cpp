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

#include "MyEEprom.h"

int maxNrOfBytes = 512;

void begin_EEProm(int aMaxNrOfBytes) {
  maxNrOfBytes = aMaxNrOfBytes;
  EEPROM.begin(maxNrOfBytes); // Maximum number of bytes available to be used as EEProm, this is 512 for ESP32
}

// save bytes written to EEProm
void save_EEProm(void) {
  EEPROM.commit();
}

String read_eeprom(int offset, int len) {
  String res = "";

  if ((offset < 0) || (offset > maxNrOfBytes - 1)) {
    return res;
  }
  Serial.print(F("read_eeprom()"));

  for (int i = 0; i < len; ++i) {
    if ((offset + i > maxNrOfBytes - 1)) {
      return res;
    }

    res += char(EEPROM.read(i + offset));
  }
  return res;
}

void write_eeprom(int offset, int len, String value) {
  if ((offset < 0) || (offset > maxNrOfBytes - 1)) {
    return;
  }
  Serial.println(F("write_eeprom()"));
  for (int i = 0; i < len; ++i) {
    if ((offset + i > maxNrOfBytes - 1)) {
      return;
    }
    if ((unsigned)i < value.length()) {
      EEPROM.write(i + offset, value[i]);
    } else {
      EEPROM.write(i + offset, 0);
    }
  }
}
