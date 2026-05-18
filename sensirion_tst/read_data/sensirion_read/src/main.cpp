#include <Arduino.h>
#include <SensirionI2cSfx6xxx.h>
#include <Wire.h>

#ifdef NO_ERROR
#undef NO_ERROR
#endif
#define NO_ERROR 0

SensirionI2cSfx6xxx sensor;

static char errorMessage[64];
static int16_t error;

void setup() {
    Serial.begin(115200);
    while (!Serial) { delay(100); }
    
    Wire.begin();
    Wire.setClock(100000); 
    
    // Inicjalizacja z domyślnym adresem 0x24
    sensor.begin(Wire, 0x24); 

    Serial.println("Rozpoczęcie konfiguracji SFM6000D (Miernik)...");

    // 1. Zatrzymanie z długim odczekaniem (czyści stan układu np. po miękkim restarcie ESP)
    sensor.stopContinuousMeasurement();
    delay(200);

    // 2. Start pomiaru ciągłego (kalibracja dla powietrza)
    error = sensor.startAirContinuousMeasurement();
    if (error != NO_ERROR) {
        Serial.print("Blad startAir: ");
        errorToString(error, errorMessage, sizeof errorMessage);
        Serial.println(errorMessage);
        return;
    }
    
    Serial.println("Uruchomiono pomiar ciagly. Czekam na stabilizacje sensora...");
    delay(500); // Miernik potrzebuje ułamka sekundy na rozruch wewnętrznej grzałki i przetwornika
    
    Serial.println("Setup zakonczony. Rozpoczynam odczyty.");
}

void loop() {
    float aFlow = 0.0;
    
    // Odczyt samego przepływu
    error = sensor.readFlow(aFlow);
    
    if (error != NO_ERROR) {
        Serial.print("Error readFlow(): ");
        errorToString(error, errorMessage, sizeof errorMessage);
        Serial.println(errorMessage);
        
        // Zabezpieczenie: próba zresetowania stanu linii I2C w razie zawieszenia komunikacji
        Wire.endTransmission(); 
    } else {
        Serial.print("Aktualny przeplyw: ");
        Serial.print(aFlow);
        Serial.println(" slm"); // slm = Standard Liters per Minute
    }

    // Odczyt co 500 ms (2 razy na sekundę) - optymalne dla stabilności wyników
    delay(500); 
}