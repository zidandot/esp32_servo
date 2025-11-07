#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <Servo.h>

Servo servo1;

const char* ssid = "GMRT_C20";
const char* password = "SubtimAlfan5";

WiFiServer server(80);
int servoAngle = 0;
bool turnFlag = true; // true = servo server jalan dulu

void setup() {
  Serial.begin(115200);
  servo1.attach(13);
  servo1.write(0);

  WiFi.softAP(ssid, password);
  Serial.println("WiFi Access Point started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();

    // Jika client meminta "TURN_DONE" maka giliran server lagi
    if (request.indexOf("TURN_DONE") >= 0) {
      turnFlag = true;
      Serial.println("Client selesai, giliran Server!");
    }

    if (turnFlag) {
      // Gerakkan servo server
      for (servoAngle = 0; servoAngle <= 180; servoAngle += 10) {
        servo1.write(servoAngle);
        delay(100);
      }
      for (servoAngle = 180; servoAngle >= 0; servoAngle -= 10) {
        servo1.write(servoAngle);
        delay(100);
      }
      turnFlag = false;

      // Kirim sinyal ke client untuk ganti giliran
      client.print("SERVER_DONE");
      Serial.println("Giliran Client!");
    }

    client.stop();
  }
}
