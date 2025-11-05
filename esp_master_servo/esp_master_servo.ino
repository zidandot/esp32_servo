#include <ESP32Servo.h>
#include <Wire.h>
#include <WiFi.h>
#include <WiFiAP.h>

const char* ssid = "esp32";
const char* pass = "12345678";

WiFiServer server(80);
Servo servo1, servo2;

#define servo1Pin 26 
#define servo2Pin 25
int servoAwal = 90;

void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid, pass);
  Serial.println(WiFi.softAPIP());
  server.begin();

  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);

  servo1.write(servoAwal);
  servo2.write(servoAwal);
}

void loop() {
  WiFiClient client = server.available();

  if(client) {
    Serial.println("Client connected!!");
    while(client.connected()) {
      if(client.available()) {
        String data = client.readString();
        Serial.println("Data dari client: " + data);
      }
    }
    client.stop();
    Serial.println("Client disconected!");
  }
  servo1.write(servoAwal+90);
  servo2.write(servoAwal-90);
  delay(500);
  servo1.write(servoAwal-90);
  servo2.write(servoAwal+90);
  delay(500);
}
