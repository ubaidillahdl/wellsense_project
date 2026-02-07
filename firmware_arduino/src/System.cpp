#include "Functions.h"
#include "Global.h"

// --- ISR (Interrupt Service Routine) ---
// Dipanggil otomatis setiap sensor MAX30105 punya data baru (400Hz)
void handleInterrupt() {
      dataReady = true;
}

// --- UTILITAS MEMORI ---
// Mengecek sisa RAM (Sangat penting karena kita pakai buffer besar)
int sisaRAM() {
      extern int __heap_start, *__brkval;
      int v;
      // Menghitung selisih antara stack pointer dan heap
      return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}

// --- KONFIGURASI AWAL SISTEM ---
void initSistem() {
      // Komunikasi Serial ke PC
      Serial.begin(115200);

      // Komunikasi ke Modul SIM800C
      sim800.begin(9600);
      sim800.println("ATE0");  // Disable Echo: Agar SIM800 tidak mengirim balik perintah kita

      // Setup Pin Interrupt untuk Sensor
      pinMode(interruptPin, INPUT_PULLUP);
}
