#include <Arduino.h>
#include <Wire.h>

#include "Functions.h"
#include "Global.h"

// --- Inisialisasi Objek ---
MAX30105 particleSensor;
SoftwareSerial sim800(rxSim, txSim);

// --- Variabel Global & State ---
DataSensor wadah;
DesimasiState desimRed, desimIR;
LpfState filterRed, filterIR;
HasilVitals dataVitals;

bool dataReady = false;
uint8_t bufferIdx = 0;
int32_t rawRed, filteredRed, rawIR, filteredIR;
unsigned long waktuMulai = 0;
unsigned long waktuMulaiSesi = 0;
bool sedangIstirahat = false;
bool butuhRetryCepat = false;

void setup() {
      initSistem();     // Serial, I2C, Pin mode
      initSensorMAX();  // Konfigurasi register MAX30105
}

void loop() {
      // --- 0. CEK KEBERADAAN TANGAN (Proximity) ---
      if (!adaTangan()) {
            if (!sedangIstirahat) {
                  bufferIdx = 0;
                  sedangIstirahat = true;
                  detachInterrupt(digitalPinToInterrupt(interruptPin));
                  particleSensor.setPulseAmplitudeRed(0);
            }
            prosesStandby();  // Idle mode
            return;
      }

      // --- 1. JEDA / COOLDOWN (Manajemen Sesi) ---
      if (sedangIstirahat) {
            // Retry cepat jika error, atau 3 detik jika sesi baru
            unsigned long durasiTunggu = butuhRetryCepat ? 200 : 3000;
            if (millis() - waktuMulai >= durasiTunggu) {
                  bangunSesi();  // Re-aktifkan sensor & interrupt
            } else {
                  return;  // Masih dalam masa tunggu
            }
      }

      // --- 2. SAMPLING DATA (Interupsi 400Hz) ---
      if (dataReady) {
            prosesSampling();  // Low Pass Filter & Downsampling ke 50Hz
      }

      // --- 3. TRANSMISI DATA ---
      if (bufferIdx >= PANJANG_BUFFER) {
            prosesKirimData();  // Upload ke Server via SIM800C
      }
}