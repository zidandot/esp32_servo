#include <ESP32Servo.h>

#define SOUND_SENSOR 34
Servo Servo1;
int threshold = 2000;

void setup() {
  Serial.begin(115200);
  Servo1.attach(22);
  Servo1.write(0);
  Serial.println("Deteksi suara dimulai...")
}

void loop() {
  int soundValue = analogRead(SOUND_SENSOR);
  Serial.println(soundValue);

  if (soundValue > threshold) {
    Serial.println("Suara keras! Servo bergerak...");
    Servo1.write(90);
    delay(1000);
    Servo1.write(0);
  }

  delay(200);
}
