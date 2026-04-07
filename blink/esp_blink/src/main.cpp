#include <Arduino.h>

// Wbudowana dioda na większości płytek NodeMCU to GPIO 2.
// Jeśli nie mruga, spróbuj 13.
#define LED_PIN 2

void setup() {
  pinMode(LED_PIN, OUTPUT); // Ustaw pin diody jako wyjście
}

void loop() {
  digitalWrite(LED_PIN, HIGH); // Włącz diodę
  delay(1000);                  // Czekaj 1 sekundę
  digitalWrite(LED_PIN, LOW);  // Wyłącz diodę
  delay(1000);                  // Czekaj 1 sekundę
}