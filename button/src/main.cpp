#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

// --- KONFIGURACJA PINÓW ---
#define BUTTON_PIN 4

// --- KONFIGURACJA SIECI I MQTT ---
const char* ssid = "DWR-921-7367C7"; // Z
const char* password = "aRCuewvq"; // ZMIEŃ NA SWOJE HASŁO!

// --- KONFIGURACJA MQTT ---
const char* mqtt_server = "192.168.0.50"; // Publiczny serwer testowy
const char* topic_publish = "moj/test"; // ZMIEŃ NA SWÓJ UNIKALNY TEMAT!

WiFiClient espClient;
PubSubClient client(espClient);

// --- ZMIENNE PRZERWANIA (VOLATILE) ---
volatile bool buttonPressed = false;
volatile unsigned long lastInterruptT ime = 0;
const unsigned long debounceDelay = 50; 

// --- FUNKCJA PRZERWANIA (ISR) ---
void IRAM_ATTR handleButtonPress() {
  unsigned long interruptTime = millis();
  if (interruptTime - lastInterruptTime > debounceDelay) {
    buttonPressed = true; // Zgłaszamy głównej pętli, że było kliknięcie
  
    lastInterruptTime = interruptTime;
  }
}

// --- FUNKCJA ŁĄCZĄCA Z MQTT ---
void reconnect() {
  while (!client.connected()) {
    Serial.print("Laczenie z serwerem MQTT...");
    String clientId = "ESP32Client-Button-" + String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("Polaczono!");
    } else {
      Serial.print("Blad, rc=");
      Serial.print(client.state());
      Serial.println(" - Sprobuj ponownie za 5 sekund.");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  // 1. Konfiguracja przycisku i przerwania
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleButtonPress, FALLING);

  // 2. Łączenie z Wi-Fi
  Serial.print("Laczenie z Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nPolaczono z Wi-Fi!");

  // 3. Konfiguracja MQTT
  client.setServer(mqtt_server, 1883);
  
  Serial.println("System gotowy. Wcisnij przycisk na PINIE 4, aby wyslac wiadomosc!");
}

void loop() {
  // Pilnujemy połączenia MQTT
  if (!client.connected()) {
    reconnect();
  }
  
  // Niezbędne do utrzymania komunikacji w tle
  client.loop();

  // Sprawdzamy flagę podniesioną przez przerwanie sprzętowe
  if (buttonPressed) {
    Serial.println("Wykryto klikniecie! Publikuje wiadomosc na MQTT...");
    
    // Wysyłamy wiadomość na serwer
    client.publish(topic_publish, "UWAGA: Fizyczny przycisk zostal wcisniety!");
    
    // Opuszczamy flagę, czekając na kolejne kliknięcie
    buttonPressed = false; 
  }
}