// ESP32 Master that broadcasts pulses to the client and moves the servos accordingly

#include <ESP32Servo.h>
#include <Wire.h>
#include <WiFi.h>
#include <WiFiAP.h>

const char* ssid = "GMRT_C20";
const char* password = "SubtimAlfan5";

WiFiServer server(80);
Servo servo1;
// Servo servo2; // Can be added with another servo

#define servo1Pin 22 
// #define servo2Pin 25
int direction1 = 90; // Starts from the origin 90
// int direction2 = 90; // Starts from the origin 90

// Periodic data transmission
unsigned long lastSendTime = 0;
const unsigned long sendInterval = 1000; // Send every 1 second
int pulseCounter = 0;
int highCount = 0;
int moveCount = 0;

void setup() {
  Serial.begin(115200);
  
  // Create Access Point
  WiFi.softAP(ssid, password);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  
  server.begin();
  Serial.println("Server started - Broadcasting binary pulses");
  Serial.println("Waiting for client connection...\n");
  
  servo1.attach(servo1Pin);
  // servo2.attach(servo2Pin);

  servo1.write(direction1);
  // servo2.write(direction2);
}

void moveServo(int pulse) {
  if (pulse == 1) { 
    highCount++; 
  }

  // If the server broadcasts a certain amount of 1's, the servos will move
  if(highCount == 4) {
    Serial.println("\nMoving servos...");
    int moveInCycle = moveCount % 2;
    
    if(moveInCycle == 0) { 
      // First move: positive direction
      direction1 = direction1 + 90;
      // direction2 = direction2 - 90;
      servo1.write(direction1);
      // servo2.write(direction2);
      Serial.print("\nMove #");
      Serial.print(moveCount + 1);

    } else if(moveInCycle == 1) {
      // Next move: negative direction
      direction1 = direction1 - 90;
      // direction2 = direction2 + 90;
      servo1.write(direction1);
      // servo2.write(direction2);
      Serial.print("\nMove #");
      Serial.print(moveCount + 1);
    }
  delay(500);
  moveCount++;
  highCount = 0;
  }
}

void loop() {
  WiFiClient client = server.available();
  
  if (client) {
    Serial.println(">>> Client connected <<<\n");
    
    while (client.connected()) {
      // Periodic binary pulse transmission
      if (millis() - lastSendTime >= sendInterval) {
        // Alternate between 1 and 0
        int pulse = pulseCounter % 2;
        
        // Send to client
        client.println(String(pulse));
        
        // Display on Serial Monitor with visual indicator
        Serial.print("Pulse #");
        Serial.print(pulseCounter);
        Serial.print(": ");
        
        if (pulse == 1) {
          Serial.print("1 [HIGH] ");
          Serial.println("████████"); // Visual bar
        } else {
          Serial.print("0 [LOW]  ");
          Serial.println("________"); // Visual bar
        }
        pulseCounter++;
        lastSendTime = millis();
        moveServo(pulse);
      }
      delay(100);
    }
    client.stop();
    Serial.println("\n>>> Client disconnected <<<");
    Serial.println("Waiting for client connection...\n");
  }
  delay(50);
}