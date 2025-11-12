// ESP32 Master that broadcasts pulses to the client and moves the servos accordingly

#include <ESP32Servo.h>
#include <Wire.h>
Servo servoBase, servoShoulder, servoElbow, servoWrist, servoGripper; // Can be added with another servo

#define SOUND_SENSOR 34
#define servoBasePin 13
#define servoShoulderPin 12
#define servoElbowPin 14
#define servoWristPin 26
#define servoGripperPin 27

int direction1 = 90; // Starts from the origin 90
int direction2 = 90;
int direction3 = 90;
int direction4 = 90;
int direction5 = 90; 

int threshold = 2000; 
int pulse;
int highCount = 0;
int moveCount = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Deteksi suara dimulai...");
  
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

  analogReadResolution(12); // ESP32 uses 12-bit ADC (0-4095)
  analogSetAttenuation(ADC_11db); // Full range 0-3.3V
}

void moveServo() {
  int soundValue = analogRead(SOUND_SENSOR);
  Serial.println(soundValue);
  delay(50);
  pulse = (soundValue > threshold) ? 1 : 0;

  if (pulse == 1) { 
    highCount++; 
    delay(100);
  }

  if (highCount == 4) {
    Serial.println("\nMoving servos...");
    int moveInCycle = moveCount % 2;
    
    if (moveInCycle == 0 && soundValue > threshold) { 
      direction1 = direction1 + 30;
      direction2 = direction2 - 45;
      direction3 = direction3 + 45;
      direction4 = direction4 - 90;
      direction5 = direction5 + 90;
    } else if (moveInCycle == 1) {
      direction1 = direction1 - 30;
      direction2 = direction2 + 45;
      direction3 = direction3 - 45;
      direction4 = direction4 + 90;
      direction5 = direction5 - 90;
    }

    direction1 = constrain(direction1, 0, 180);
    direction2 = constrain(direction2, 0, 180);
    direction3 = constrain(direction3, 0, 180);
    direction4 = constrain(direction4, 0, 180);
    direction5 = constrain(direction5, 0, 180);

    servoBase.write(direction1);
    servoShoulder.write(direction2);
    servoElbow.write(direction3);
    servoWrist.write(direction4);
    servoGripper.write(direction5);
    Serial.print("\nMove #");
    Serial.println(moveCount + 1);

    moveCount++;
    highCount = 0;
  }
}

void loop() {
  moveServo();
  delay(1000);
}
