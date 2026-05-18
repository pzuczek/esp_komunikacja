#include <Arduino.h>
#include <Wire.h>

void setup() {
  Serial.begin(115200);
  
  // Uruchamiamy magistralę I2C na domyślnych pinach ESP32 (SDA=21, SCL=22)
  Wire.begin(21, 22);
  
  // NOWOŚĆ: Ustawiamy limit czasu na 100ms. ESP32 przestanie się zawieszać!
  Wire.setTimeOut(100); 
  
  Serial.println("\nRozpoczynam skanowanie magistrali I2C...");
}

void loop() {
  byte error, address;
  int nDevices = 0;

  Serial.println("Skanowanie...");

  // Pętla sprawdzająca wszystkie 127 adresów I2C
  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("Znaleziono urzadzenie I2C pod adresem 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
      nDevices++;
    }
    else if (error == 4) {
      Serial.print("Nieznany blad pod adresem 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }    
  }
  
  if (nDevices == 0) {
    Serial.println("Nie znaleziono zadnych urzadzen I2C.\n");
  } else {
    Serial.println("Skanowanie zakonczone.\n");
  }
  
  delay(5000); // Czekamy 5 sekund przed kolejnym skanem
}