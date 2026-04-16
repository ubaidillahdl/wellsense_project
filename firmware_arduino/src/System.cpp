#include "Functions.h"
#include "Global.h"

// --- ISR (Interrupt Service Routine) ---
// Dipanggil otomatis setiap sensor MAX30105 punya data baru (400Hz)
void handleInterrupt() {
      dataReady = true;
}

// --- KONFIGURASI AWAL SISTEM ---
void initSistem() {
      // Komunikasi Serial ke PC
      Serial.begin(115200);

      // Komunikasi ke Modul SIM800C
      sim800.begin(9600);
      // sim800.println("ATE0");

      // Setup Pin Interrupt untuk Sensor
      pinMode(interruptPin, INPUT_PULLUP);

      pinMode(vibratorPin, OUTPUT);
}

void vibrate() {
      // Getaran pertama (pendek)
      digitalWrite(vibratorPin, HIGH);
      delay(80);
      digitalWrite(vibratorPin, LOW);

      delay(100);  // Jeda singkat

      // Getaran kedua (sedikit lebih panjang)
      digitalWrite(vibratorPin, HIGH);
      delay(150);
      digitalWrite(vibratorPin, LOW);
}
