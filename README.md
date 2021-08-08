**MQTTAccess module**

MQTTAcess, is een basis module, om vanaf een Wemos D1 mini bordje, berichten te sturen naar een MQTT server.

Voor het processorbordje, waar deze sourcecode op draait, wordt gebruik gemaakt van een Wemos D1 mini, die voorzien is van een ESP8266 processor, hiervoor kan bijvoorbeeld de robotdyn-d1-mini-pro-esp8266-4mb-cp2104 worden gebruikt

Zie: https://www.tinytronics.nl/shop/nl/arduino/wemos/robotdyn-d1-mini-pro-esp8266-4mb-cp2104

Eventuele overige hardware kan gemonteerd worden op een extra proefprintje, bijvoorbeeld: RobotDyn ProtoBoard Shield voor D1 Mini

Zie: https://www.tinytronics.nl/shop/nl/arduino/wemos/robotdyn-protoboard-shield-voor-d1-mini

**Software voor de MQTTAccess module**

Informatie kan naar een willekeurige MQTT server worden gestuurd. De details van deze server worden tijdens het compileren van de software gehaald uit de .passwd.h file in de include directory van het PlatformIO project. Deze file moet, voordat de software gecompileerd wordt, aan de include directory toegevoegd worden en moet het volgende bevatten:
 
_*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*_

_PLEASE READ THIS FIRST_

_Add a file to the include directory of this project with the_

_following name:_

_.passwd.h_

_The content of this file should be:_

_#pragma once_

_// WiFi credentials_

_#define WIFI\_NETWORK &quot;YOUR\_WIFI\_NETWORK\_NAME&quot;_

_#define WIFI\_PASSWD &quot;YOUR\_WIFI\_NETWORK\_PASSWORD&quot;_

_#define FALLBACK\_WIFI\_NETWORK &quot;Dummy&quot; // or chose any other name you like_

_#define FALLBACK\_WIFI\_PASSWD &quot;12345678&quot; // or take another password_

_// mqtt server_

_#define MY\_MQTT\_HOST &quot;your\_mqtt\_host e.g. 10.10.10.10&quot;_

_#define MY\_MQTT\_PORT &quot;1883&quot;_

_#define MY\_MQTT\_USERNAME &quot;your\_mqtt\_server\_username&quot;_

_#define MY\_MQTT\_PASSWORD &quot;your\_mqtt\_server.password&quot;_

_// OTA credentials_

_#define OTA\_PASSWORD &quot;MyPassW00rd&quot; // must be the same in platformio.ini_

_Also check in the platformio.ini file the upload\_port set._

_In the current file this port is set for a serial connection to a_

_Linux Ubuntu PC for a Windows PC running Visual Studio Code with_

_PlatformIO you should use something like:_

_upload\_port = COM3_

_Or any other comport number which is used for the serial connection_

_For further information see the README.md in the repository on GitHub_

_\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*_

**WiFi netwerk instellingen**

Standaard gebruikt de MQTTAccess module de WiFi credentials zoals deze geconfigureerd zijn in de .passw.h file. Omdat in main.cpp de volgende #define is gezet:

_#define USE\_DEFAULT\_WIFI\_SETTINGS true_

Dit is niet het geval als deze #define op false is gezet:

_#define USE\_DEFAULT\_WIFI\_SETTINGS false_

In dat geval zal de MQTT access module zich niet zomaar direct verbinden met een WiFi netwerk. Om het juiste WiFi netwerk in te stellen zal de MQTTAccess module een WiFi Access Point (AP) opztten, met de FALLBACK WiFi credentials, zoals die geconfigureerd is in de .passwd.h file. Ook wordt er een webserver (met IP address: 192.168.4.1) gestart.

Als je bijvoorbeeld een PC, een telefoon of een tablet verbindt met dit accesspoint, krijg je m.b.v. een webbrowser de mogelijkheid om een bepaald WiFi netwerk te kiezen en te configureren. Ook kan je dan de te gebruiken MQTT server configureren. Alle informatie wordt vervolgens opgeslagen in EEProm, zodat de module een volgende keer automatisch op kan starten.

**Overzicht van de repository**

Deze repository heeft de volgende subdirectories:

- **KiCad\_files:**

Indien beschikbaar het ontwerp van een print voor de betreffende module.

- **PlatformIO\_Files:**

De source code voor de MQTTAccess module. Voor het ontwikkelen van deze software wordt gebruik gemaakt van PlatformIO. PlatformIO is een extensie op Visual Studio Code.
