#include <ESP32Encoder.h>
ESP32Encoder encoder;

void setup() {
    encoder.attachHalfQuad(16, 17); // Replace with your chosen pins
    encoder.clearCount();
    encoder.setCount(0);
    Serial.begin(115200);
}

void loop() {
    Serial.println(encoder.getCount());
    delay(100);
}