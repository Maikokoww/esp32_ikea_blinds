// Librairies
#include <WiFi.h>
#include <ESP32Encoder.h>
#include <FS_MX1508.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <Preferences.h>

// Wifi
const char *ssid = "yourSSID";
const char *password = "youtPassword";
NetworkServer server(80);

// ESP32Encoder
ESP32Encoder encoder;
#define pinEncoderA 16
#define pinEncoderB 17
#define BASE 200
long lastupdate1 = 0;

// FS_MX1508
#define pinINA1 20
#define pinINB1 21
MX1508 motor1(pinINA1, pinINB1);

// WifiUdp
WiFiUDP ntpUDP;

// NTPClient
NTPClient ntpClient(ntpUDP, "0.be.pool.ntp.org", 7200);
int currentHour = -1;
int currentMinute = -1;
long lastupdate2 = 0;

// HTTPClient
const char* apiKey = "yourApiKeyFromOpenWeatherMap";  // Replace with your API key
const char* city = "yourCity";        // Replace with your city
const char* weatherURL = "http://api.openweathermap.org/data/2.5/weather?q=yourCity&appid=yourApiKeyFromOpenWeatherMap";
long lastupdate3 = 0;

// Preferences
Preferences preferences;

// Set functions
int getMotorPosition();
void store(int motorPosition, int closedPercentage);
void getWeather();
int retrieveEEPROM();
void saveEEPROM(int motorPosition);

void setup(){
  Serial.begin(115200);
  preferences.begin("EEPROMData", false);

  // EEPROM
  int eepromPosition = retrieveEEPROM();
  Serial.print("Position de l'eeprom : ");
  Serial.println(eepromPosition);

  // Encoder 
  encoder.attachHalfQuad(pinEncoderA, pinEncoderB); // Replace with your chosen pins
  encoder.clearCount();
  encoder.setCount(eepromPosition);

  // WiFi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("█");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  unsigned long currentMillis = millis();
 
  if(currentMillis - lastupdate1 > 2500){ // Checks every 2.5 seconds
    getMotorPosition();
    lastupdate1 = currentMillis;
  }

  if(currentMillis - lastupdate2 > 30000) // Checks every 30 seconds
  {
    ntpClient.update();
    int currentHour = ntpClient.getHours();
    int currentMinute = ntpClient.getMinutes();
    Serial.print("Heure actuelle : ");
    Serial.print(currentHour);
    Serial.print(":");
    Serial.println(currentMinute);
    lastupdate2 = currentMillis;
  }

  if(currentMillis - lastupdate3 > 900000) // Checks every 15 minutes
  {
    getWeather();
    lastupdate3 = currentMillis;
  }

  NetworkClient client = server.accept();  // listen for incoming clients

  if (client) {                     // if you get a client,
    Serial.println("New Client.");  // print a message out the serial port
    String currentLine = "";        // make a String to hold incoming data from the client
    while (client.connected()) {    // loop while the client's connected
      if (client.available()) {     // if there's bytes to read from the client,
        char c = client.read();     // read a byte, then
        Serial.write(c);            // print it out the serial monitor
        if (c == '\n') {            // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("<body>");
            client.print("<a href=\"/store100\">Click here to set blinds to 100%<br></a>");
            client.print("<a href=\"/store75\">Click here to set blinds to 75%<br></a>");
            client.print("<a href=\"/store50\">Click here to set blinds to 50%<br></a>");
            client.print("<a href=\"/store25\">Click here to set blinds to 25%<br></a>");
            client.print("<a href=\"/store0\">Click here to set blinds to 0%<br></a>");
            client.print("<style>*{margin:0;padding:0}body{display:flex;position:absolute;"
             "flex-direction:column;gap:10px;width:100vw;background-color:#696969}"
             "h1{width:100%;justify-content:center;display:flex;font-size:large}"
             "a{background-color:#777;padding:15px;text-decoration:none;"
             "text-align:center;color:#FFF;border-radius:15px}</style>");
            client.print("</body>");
            
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {  // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        if (currentLine.endsWith("GET /store100")) {
          store(getMotorPosition(), 100);
          Serial.println("Blinds position : 100");
        }
        if (currentLine.endsWith("GET /store75")) {
          store(getMotorPosition(), 75);
          Serial.println("Blinds position : 75");
        }
        if (currentLine.endsWith("GET /store50")) {
          store(getMotorPosition(), 50);
          Serial.println("Blinds position : 50");
        }
        if (currentLine.endsWith("GET /store25")) {
          store(getMotorPosition(), 25);
          Serial.println("Blinds position : 25");
        }
        if (currentLine.endsWith("GET /store0")) {
          store(getMotorPosition(), 0);
          Serial.println("Blinds position : 0");
        }
        if (currentHour != -1){
          if (currentHour == 6 && currentMinute == 30){
            store(getMotorPosition(), 33);
          }
          if (currentHour == 10 && currentMinute == 0){
            store(getMotorPosition(), 66);
          }
          if (currentHour == 12 && currentMinute == 0){
            store(getMotorPosition(), 100);
          }
          if (currentHour == 20 && currentMinute == 30){
            store(getMotorPosition(), 66);
          }
          if (currentHour == 22 && currentMinute == 0){
            store(getMotorPosition(), 0);
          }
        }
      } 
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}

int getMotorPosition(){
  int motorPosition = encoder.getCount(); // Set the EEPROM value to the motorPosition integer
  Serial.print("Current motor position : ");
  Serial.println(motorPosition/BASE);
  if (retrieveEEPROM() != motorPosition){
    saveEEPROM(motorPosition);
  }
  return(motorPosition/BASE);
}

void store(int motorPosition, int closedPercentage) {
  while (motorPosition != closedPercentage){
    if (motorPosition < closedPercentage){
      motor1.motorGoP(100); // Move forward
      motorPosition = getMotorPosition(); // Update position
      Serial.print("Requested position : ");
      Serial.println(closedPercentage);
    }
    else {
      motor1.motorGoP(-100); // Move backward
      motorPosition = getMotorPosition(); // Update position
      Serial.print("Requested position : ");
      Serial.println(closedPercentage);
    }
  }
  motor1.motorGo(0);
  Serial.print("Motor Stopped");
}

void getWeather(){
  if (WiFi.status() == WL_CONNECTED) {
  HTTPClient http;
  http.begin(weatherURL);
  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    String payload = http.getString();
    JSONVar weatherData = JSON.parse(payload);

  if (JSON.typeof(weatherData) == "undefined") {
      Serial.println("Failed to parse weather data.");
    }

    String weatherCondition = weatherData["weather"][0]["main"];
    Serial.print("Weather : ");
    Serial.println(weatherCondition);

      // Automate blinds based on weather conditions
    if (weatherCondition == "Rain" || weatherCondition == "Thunderstorm") {
      Serial.println("Closing blinds because of rainy weather.");
      store(getMotorPosition(), 0);
      }
    }
    http.end();
  }
}

int retrieveEEPROM(){
  int EEPROMDataMoteur = preferences.getInt("motorPosition"); // Get the value of the EEPROM
  return(EEPROMDataMoteur);
}

void saveEEPROM(int motorPosition){
  preferences.putInt("motorPosition", motorPosition); // Store an integer value to EEPROM
  Serial.println("Motor Position saved to EEPROM.");
}
