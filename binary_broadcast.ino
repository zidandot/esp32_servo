// ESP32 WiFi Server - Binary Pulse Broadcaster
// Upload this code to the first ESP32

#include <WiFi.h>

const char* ssid = "GMRT_C20";
const char* password = "SubtimAlfan5";

WiFiServer server(80);

// Periodic data transmission
unsigned long lastSendTime = 0;
const unsigned long sendInterval = 1000; // Send every 1 second
int pulseCounter = 0;

void setup() {
  Serial.begin(115200);
  
  // Create Access Point
  WiFi.softAP(ssid, password);
  Serial.println("=== ESP32 Binary Pulse Server ===");
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  
  server.begin();
  Serial.println("Server started - Broadcasting binary pulses");
  Serial.println("Waiting for client connection...\n");
}

void loop() {
  WiFiClient client = server.available();
  
  if (client) {
    Serial.println(">>> Client connected <<<\n");
    
    while (client.connected()) {
      // Periodic binary pulse transmission
      if (millis() - lastSendTime >= sendInterval) {
        // Alternate between 1 and 0
        int binaryPulse = pulseCounter % 2;
        
        // Send to client
        client.println(String(binaryPulse));
        
        // Display on Serial Monitor with visual indicator
        Serial.print("Pulse #");
        Serial.print(pulseCounter);
        Serial.print(": ");
        
        if (binaryPulse == 1) {
          Serial.print("1 [HIGH] ");
          Serial.println("████████"); // Visual bar
        } else {
          Serial.print("0 [LOW]  ");
          Serial.println("________"); // Visual bar
        }
        
        pulseCounter++;
        lastSendTime = millis();
      }
      
      delay(10);
    }
    
    client.stop();
    Serial.println("\n>>> Client disconnected <<<");
    Serial.println("Waiting for client connection...\n");
  }
  
  delay(10);
}
