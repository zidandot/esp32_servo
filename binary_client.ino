// ESP32 WiFi Client - Responds to Binary Pulses
// Upload this code to the second ESP32

#include <WiFi.h>

const char* ssid = "GMRT_C20";
const char* password = "SubtimAlfan5";
const char* serverIP = "192.168.4.1";
const int serverPort = 80;

WiFiClient client;
int receivedPulseCount = 0;
int highCount = 0;
int lowCount = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("ON");
  Serial.println("OFF");
  
  // Connect to Server's Access Point
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\n=== ESP32 Responsive Client ===");
  Serial.print("Connected! Client IP: ");
  Serial.println(WiFi.localIP());
  
  // Connect to server
  if (client.connect(serverIP, serverPort)) {
    Serial.println("✓ Connected to server");
    Serial.println("Listening and responding to binary pulses...\n");
    
    // Send initial acknowledgment
    client.println("ACK:CLIENT_READY");
  } else {
    Serial.println("✗ Connection to server failed");
  }
}

void respondToPulse(String pulse) {
  pulse.trim();
  
  // Display received pulse
  Serial.print("Received #");
  Serial.print(receivedPulseCount);
  Serial.print(": ");
  
  String response = "";
  
  if (pulse == "1") {
    Serial.print("1 [HIGH] ████████");
    
    Serial.println("LED ON");
    highCount++;
    response = "ACK:HIGH_RECEIVED";
    
  } else if (pulse == "0") {
    Serial.print("0 [LOW]  ________");
    
    // Response to LOW pulse
    Serial.println("LED OFF");
    lowCount++;
    response = "ACK:LOW_RECEIVED";
    
  } else {
    Serial.print("Unknown: ");
    Serial.print(pulse);
    response = "ACK:UNKNOWN_DATA";
  }
  
  // Send acknowledgment back to server
  client.println(response);
  Serial.print(" → Sent: ");
  Serial.println(response);
  
  receivedPulseCount++;
  
  // Display statistics every 10 pulses
  if (receivedPulseCount % 10 == 0) {
    Serial.println("\n--- Statistics ---");
    Serial.print("Total: ");
    Serial.print(receivedPulseCount);
    Serial.print(" | HIGH: ");
    Serial.print(highCount);
    Serial.print(" | LOW: ");
    Serial.println(lowCount);
    Serial.println("------------------\n");
  }
}

void loop() {
  // Check for incoming binary data from server
  if (client.available()) {
    String data = client.readStringUntil('\n');
    respondToPulse(data);
  }
  
  // Reconnect if connection lost
  if (!client.connected()) {
    Serial.println("LED OFF");
    delay(1000);
    Serial.println("\n! Disconnected from server !");
    Serial.println("Reconnecting...");
    delay(1000);
    
    if (client.connect(serverIP, serverPort)) {
      Serial.println("✓ Reconnected to server");
      Serial.println("Listening and responding to binary pulses...\n");
      client.println("ACK:CLIENT_RECONNECTED");
    }
  }
  
  delay(10);
}
