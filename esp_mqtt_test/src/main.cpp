#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h> // Biblioteka do MQTT

// --- KONFIGURACJA SIECI ---
const char* ssid = "1509B"; // Z
const char* password = "17485524"; // ZMIEŃ NA SWOJE HASŁO!

// --- KONFIGURACJA MQTT ---
const char* mqtt_server = "broker.hivemq.com"; // Publiczny serwer testowy
const char* topic_publish = "esp_mqtt_test_1/sub"; // ZMIEŃ NA SWÓJ UNIKALNY TEMAT!

WiFiClient espClient;
PubSubClient client(espClient);

// Zmienna do wysyłania wiadomości co 5 sekund (bez blokowania programu funkcja delay)
unsigned long lastMsg = 0;
int value = 0;

void setup() {
  Serial.begin(115200);
  
  // 1. Łączenie z Wi-Fi
  Serial.print("Laczenie z Wi-Fi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nPolaczono z Wi-Fi!");

  // 2. Konfiguracja serwera MQTT
  client.setServer(mqtt_server, 1883);
}

// Funkcja odpowiedzialna za utrzymanie połączenia z serwerem MQTT
void reconnect() {
  while (!client.connected()) {
    Serial.print("Proba polaczenia z MQTT...");
    // Tworzymy losowe ID klienta
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      Serial.println("Polaczono!");
    } else {
      Serial.print("Blad, rc=");
      Serial.print(client.state());
      Serial.println(" Sprobuj ponownie za 5 sekund");
      delay(5000);
    }
  }
}

void loop() {
  // Sprawdzamy czy ESP jest połączone z MQTT. Jeśli nie - łączymy.
  if (!client.connected()) {
    reconnect();
  }
  // Ta funkcja musi być wywoływana, aby MQTT działo w tle
  client.loop();

  // Wysyłamy wiadomość co 5 sekund (5000 milisekund)
  unsigned long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    ++value;
    
    // Budujemy naszą wiadomość
    String msg = "Wiadomosc z ESP32 nr: " + String(value);
    
    Serial.print("Wysylam: ");
    Serial.println(msg);
    
    // Publikujemy!
    client.publish(topic_publish, msg.c_str());
  }
}