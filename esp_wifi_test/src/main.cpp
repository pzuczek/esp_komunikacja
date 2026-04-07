#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h> // Wbudowana biblioteka do tworzenia serwera WWW

// --- KONFIGURACJA WI-FI ---
const char* ssid = "1509B"; 
const char* password = "17485524";

// Tworzymy obiekt serwera nasłuchującego na standardowym porcie 80
WebServer server(80);

// Funkcja, która wykona się, gdy ktoś wejdzie na stronę główną ESP32
void handleRoot() {
  // Wysyłamy do przeglądarki kod 200 (OK) i prosty tekst
  String html = "<h1>Czesc!</h1><p>Twoje ESP32 dziala i jest w sieci Wi-Fi!</p>";
  server.send(200, "text/html", html);
}

void setup() {
  // Uruchamiamy port szeregowy, żeby widzieć komunikaty na komputerze
  Serial.begin(115200);
  Serial.println();
  Serial.print("Laczenie z siecia: ");
  Serial.println(ssid);

  // Zlecamy ESP32 połączenie z routerem
  WiFi.begin(ssid, password);

  // Czekamy, aż status zmieni się na POŁĄCZONO
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Sukces! Wypisujemy adres IP, który router przydzielił ESP32
  Serial.println("");
  Serial.println("Polaczono z Wi-Fi!");
  Serial.print("Adres IP Twojego ESP32 to: ");
  Serial.println(WiFi.localIP());

  // Mówimy serwerowi: "Jeśli ktoś wejdzie na stronę główną (/), uruchom funkcję handleRoot"
  server.on("/", handleRoot);
  
  // Uruchamiamy serwer
  server.begin();
  Serial.println("Serwer WWW uruchomiony.");
}

void loop() {
  // W głównej pętli ESP32 musi stale nasłuchiwać, czy ktoś nie próbuje wejść na stronę
  server.handleClient();
}