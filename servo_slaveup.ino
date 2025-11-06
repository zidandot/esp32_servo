// ESP32 Slave that receives pulses from the server and moves the servos accordingly

#include <ESP32Servo.h>
#include <Wire.h>
#include <WiFi.h>
#include <WiFiAP.h>

const char* ssid = "GMRT_C20";
const char* password = "SubtimAlfan5";
const char* serverIP = "192.168.4.1";
const int serverPort = 80;

WiFiClient client;
int receivedPulseCount = 0;
int highCount = 0;
int moveCount = 0;

Servo servo1, servo2; // Can be added with another servo

#define servo1Pin 26 
#define servo2Pin 25
int direction1 = 90; // Starts from the origin 90
int direction2 = 90; // Starts from the origin 90

void setup() {
  Serial.begin(115200);
  Serial.println("ON");
  Serial.println("OFF");

  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);

  servo1.write(direction1);
  servo2.write(direction2);
  
  // Connect to Access Point
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.print("Connected! Client IP: ");
  Serial.println(WiFi.localIP());
  
  // Connect to server
  if (client.connect(serverIP, serverPort)) {
    Serial.println("✓ Connected to server");
    Serial.println("Listening and responding to binary pulses...\n");
    
    // Send initial acknowledgment
    client.println("CLIENT_READY");
  } else {
    Serial.println("✗ Connection to server failed");
  }
}

void respondToPulse(String pulse) {
  pulse.trim(); // Removes potential trailing whitespace
  
  // Display received pulse
  Serial.print("Received #");
  Serial.print(receivedPulseCount);
  Serial.print(": ");

  // Reading the received pulse
  if (pulse == "1") {
    Serial.print("1 [HIGH] ████████");
    
    Serial.println("HIGH_RECEIVED");
    highCount++;
    
  } else if (pulse == "0") {
    Serial.print("0 [LOW]  ________");
    
    // Response to LOW pulse
    Serial.println("LOW_RECEIVED");
  }
  
  receivedPulseCount++;
  
  // If the client receives a certain amount of 1's, the servos will move
  if(highCount == 10) {
    Serial.println("Moving servos...");
    int moveInCycle = moveCount % 4;
    
    if(moveInCycle == 0 || moveInCycle == 1) { 
      // First two moves: positive direction
      direction1 = direction1 + 45;
      direction2 = direction2 - 45;
      servo1.write(direction1);
      servo2.write(direction2);
      Serial.print("Move #");
      Serial.print(moveCount + 1);

    } else if(moveInCycle == 2 || moveInCycle == 3) {
      // Next two moves: negative direction
      direction1 = direction1 - 45;
      direction2 = direction2 + 45;
      servo1.write(direction1);
      servo2.write(direction2);
      Serial.print("Move #");
      Serial.print(moveCount + 1);
    }
  delay(1000);
  moveCount++;
  highCount = 0;
  }
}

void loop() {
  // Check for incoming pulse
  if (client.available()) {
    String data = client.readStringUntil('\n');
    respondToPulse(data);
  }
  
  // Reconnect if connection lost
  if (!client.connected()) {
    Serial.println("OFF");
    delay(1000);
    Serial.println("Reconnecting...");
    delay(1000);
    
    if (client.connect(serverIP, serverPort)) {
      Serial.println("Reconnected to server!");
    }
  }
  
  delay(50);
}