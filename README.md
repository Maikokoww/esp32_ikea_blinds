# esp32_ikea_blinds
Control your blinds using an ESP32
## About
**This project is made by a passionate student**

This project is about making a DIY automatic blinds controller using an esp32 and a motor for IKEA FRIDANS.
Be careful, the code is shaky, modify it to make it work  for your blinds's position.

## Libraries
- WiFi.h
- [ESP32Encoder.h](https://github.com/madhephaestus/ESP32Encoder)
- [FS_MX1508.h](https://github.com/fanfanlatulipe26/FS_MX1508)
- WiFiUdp.h
- [NTPClient.h](https://github.com/arduino-libraries/NTPClient)
- HTTPClient.h
- [Arduino_JSON.h](https://github.com/arduino-libraries/Arduino_JSON)
- [Preferences.h](https://github.com/hpsaturn/easy-preferences)


## Codes
**There is some part you need to edit to make it work for you : **
```C++
// Wifi
const char *ssid = "yourSSID";
const char *password = "yourPassword";

// ESP32Encoder
#define pinEncoderA 16 // Set your encoder pins
#define pinEncoderB 17
#define BASE 200 // Adjust it to make your encoderPosition=100 when your blinds are closed (ex. 200000/BASE = 100%)

// FS_MX1508
#define pinINA1 20 // Set your motor controler pins
#define pinINB1 21

// NTPClient
NTPClient ntpClient(ntpUDP, "0.be.pool.ntp.org", 7200); // Ajust it for your country timestamp (ex. 7200 = +2UTC)

// HTTPClient
const char* apiKey = "yourApiKeyOpenWeatherMap";  // Replace with your API key
const char* city = "yourCity";        // Replace with your city
const char* weatherURL = "http://api.openweathermap.org/data/2.5/weather?q=yourCity&appid=yourApiKeyOpenWeatherMap";

```
## Build
**The assembly should look like this :**

![assembly](/build/assembly.jpg)

**The connection for this build is :**

![connection](/build/connection.jpg)
