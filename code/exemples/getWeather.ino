// Librairies
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

// Wifi
const char *ssid = "yourSSID";
const char *password = "yourPassword";

// HTTPClient
const char* apiKey = "apiKeyFromOpenWeather";  // Replace with your API key
const char* city = "yourCity";        // Replace with your city
const char* weatherURL = "http://api.openweathermap.org/data/2.5/weather?q=yourCity&appid=apiKeyFromOpenWeather";

void getWeather();

void setup(){
  Serial.begin(115200);

  // WiFi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("*");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

long lastupdate = 0;

void loop() {
  unsigned long currentMillis = millis();

  if(currentMillis - lastupdate2 > 60000){ // Update every minute
    getWeather();
    lastupdate = currentMillis;
  }
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
      Serial.print("Météo : ");
      Serial.println(weatherCondition);
    }
    http.end();
  }
}