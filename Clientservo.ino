#include <WiFi.h>
#include <WiFiClient.h>
#include <Servo.h>

Servo servo2;

const char* ssid = "GMRT_C20";
const char* password = "SubtimAlfan5";
const char* host = "192.168.4.1"; // IP server default ESP32 AP

WiFiClient client;
int servoAngle = 0;
bool turnFlag = false;

void setup() {
  Serial.begin(115200);
  servo2.attach(12);
  servo2.write(0);

  Serial.println("Menghubungkan ke WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nTerhubung ke server!");
}

void loop() {
  if (client.connect(host, 80)) {
    // Cek apakah server memberi giliran
    client.print("PING\r");
    String response = client.readStringUntil('\r');
    if (response.indexOf("SERVER_DONE") >= 0) {
      Serial.println("Giliran Client!");
      turnFlag = true;
    }
    client.stop();
  }

  // Jika giliran client
  if (turnFlag) {
    for (servoAngle = 0; servoAngle <= 180; servoAngle += 10) {
      servo2.write(servoAngle);
      delay(100);
    }
    for (servoAngle = 180; servoAngle >= 0; servoAngle -= 10) {
      servo2.write(servoAngle);
      delay(100);
    }

    // Kirim sinyal ke server bahwa sudah selesai
    if (client.connect(host, 80)) {
      client.print("TURN_DONE\r");
      client.stop();
    }

    turnFlag = false;
    Serial.println("Giliran Server!");
  }

  delay(500);
}
