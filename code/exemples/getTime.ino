// Librairies
#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

// Wifi
const char *ssid = "yourSSID";
const char *password = "yourPassword";

// WifiUdp
WiFiUDP ntpUDP;

// NTPClient
NTPClient ntpClient(ntpUDP, "0.be.pool.ntp.org", 7200); // 7200 for +2h
int currentHour = -1;
int currentMinute = -1;

void getTime();

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

  if(currentMillis - lastupdate2 > 30000){ // Update every 30 sec
    getTime();
    lastupdate = currentMillis;
  }

void getTime(){
  ntpClient.update();
  Serial.print(ntpClient.getHours());
  Serial.print(":");
  Serial.println(ntpClient.getMinutes());
}