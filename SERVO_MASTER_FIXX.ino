// ESP32 Master that broadcasts pulses to the client and moves the servos accordingly

#include <ESP32Servo.h>
#include <Wire.h>
#include <WiFi.h>
#include <WiFiAP.h>

const char* ssid = "GMRT_C20";
const char* password = "SubtimAlfan5";

WiFiServer server(80);
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

// Periodic data transmission
unsigned long lastSendTime = 0;
const unsigned long sendInterval = 1000; // Send every 1 second
int pulse;
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
}

void moveServo(int highCount) {
  if (pulse == 1) { 
    highCount++; 
  }

  // If the server broadcasts a certain amount of 1's, the servos will move
  if(highCount == 4) {
    Serial.println("\nMoving servos...");
    int moveInCycle = moveCount % 2;
    
    if(moveInCycle == 0) { 
      // First move: positive direction
      direction1 = direction1 + 30;
      direction2 = direction2 - 45;
      direction3 = direction3 + 45;
      direction4 = direction4 - 90;
      direction5 = direction5 + 90;

      servoBase.write(direction1);
      servoShoulder.write(direction2);
      servoElbow.write(direction3);
      servoWrist.write(direction4);
      servoGripper.write(direction5);
      Serial.print("\nMove #");
      Serial.print(moveCount + 1);

    } else if(moveInCycle == 1) {
      // Next move: negative direction
      direction1 = direction1 - 30;
      direction2 = direction2 + 45;
      direction3 = direction3 - 45;
      direction4 = direction4 + 90;
      direction5 = direction5 - 90;

      servoBase.write(direction1);
      servoShoulder.write(direction2);
      servoElbow.write(direction3);
      servoWrist.write(direction4);
      servoGripper.write(direction5);
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
        pulse = pulseCounter % 2;
        
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