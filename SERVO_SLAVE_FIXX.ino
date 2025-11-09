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

Servo servoBase, servoShoulder, servoElbow, servoWrist, servoGripper; // Can be added with another servo

#define servoBasePin 26 
#define servoShoulderPin 25
#define servoElbowPin 33
#define servoWristPin 32
#define servoGripperPin 35

int direction1 = 90; // Starts from the origin 90
int direction2 = 90;
int direction3 = 90;
int direction4 = 90;
int direction5 = 90;

void setup() {
  Serial.begin(115200);
  Serial.println("ON");
  Serial.println("OFF");

  servoBase.attach(servoBasePin);
  servoShoulder.attach(servoShoulderPin);
  servoElbow.attach(servoElbowPin);
  servoWrist.attach(servoWristPin);
  servoGripper.attach(servoGripperPin);

  servoBase.write(direction1);
  servoShoulder.write(direction2);
  servoElbow.write(direction3);
  servoWrist.write(direction4);
  servoGripper.write(direction5);
  
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
  Serial.print("\nReceived #");
  Serial.print(receivedPulseCount);
  Serial.print(": ");

  // Reading the received pulse
    if (pulse == "1") {
    Serial.print("1 [HIGH] ████████");
    highCount++;
    
  } else if (pulse == "0") {
    Serial.print("0 [LOW]  ________");    
  }
  
  receivedPulseCount++;
  
  // If the client receives a certain amount of 1's, the servos will move
  if(highCount == 4) {
    Serial.println("\nMoving servos...");
    int moveInCycle = moveCount % 4;
    
    if(moveInCycle == 0 || moveInCycle == 1) { 
      // First two moves: positive direction
      direction1 = direction1 + 90;
      direction2 = direction2 - 90;
      direction3 = direction3 + 90;
      direction4 = direction4 + 90;
      direction5 = direction5 + 90;

      servoBase.write(direction1);
      servoShoulder.write(direction2);
      servoElbow.write(direction3);
      servoWrist.write(direction4);
      servoGripper.write(direction5);
      Serial.print("Move #");
      Serial.print(moveCount + 1);

    } else if(moveInCycle == 2 || moveInCycle == 3) {
      // Next two moves: negative direction
      direction1 = direction1 - 90;
      direction2 = direction2 + 90;
      direction3 = direction3 + 90;
      direction4 = direction4 + 90;
      direction5 = direction5 + 90;

      servoBase.write(direction1);
      servoShoulder.write(direction2);
      servoElbow.write(direction3);
      servoWrist.write(direction4);
      servoGripper.write(direction5);
      Serial.print("\nMove #");
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