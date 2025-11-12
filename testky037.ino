#define SOUND_ANALOG 34
void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // Configure ADC
  analogReadResolution(12); // ESP32 uses 12-bit ADC (0-4095)
  analogSetAttenuation(ADC_11db); // Full range 0-3.3V
  
  Serial.println("Deteksi suara dimulai...");
  Serial.print("Test reading: ");
  Serial.println(analogRead(SOUND_ANALOG));
}

void loop() {
  int soundValue = analogRead(SOUND_ANALOG);
  Serial.println(soundValue);
  delay(50);
}