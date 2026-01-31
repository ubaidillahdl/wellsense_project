#include "MAX30105.h"
#include <PengolahSinyalPPG.h>
#include <Wire.h>


MAX30105 particleSensor;
ChebyFilter chebyFilter;

#define BUFFER_SIZE 200

// --- KONFIGURASI BUFFER & PARAMETER ---
float sinyalBuffer[BUFFER_SIZE]; // Tempat menyimpan 100 data terakhir

const uint32_t Ts = 1 / BUFFER_SIZE; // 200 Hz
uint32_t lastT = 0;
uint32_t sampleIdx = 0;
bool segmentReady = false;

void setup() {
  chebyInit(&chebyFilter);
  Serial.begin(115200);

  // Inisialisasi Sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println(F("Sensor tidak ditemukan"));
    while (1)
      ;
  }

  // --- SETTING SENSOR (Sangat Mempengaruhi Kualitas Sinyal) ---
  particleSensor.setup(255, 1, 2, 200, 411, 16384);
}

void loop() {
  particleSensor.check();

  // 1. BLOK SAMPLING (Prioritas Utama)
  if (micros() - lastT >= Ts) {
    lastT += Ts; // Menjaga ritme tetap konsisten

    if (particleSensor.available()) {
      float rawPPG = particleSensor.getRed();
      float filteredPPG = chebyProcess(&chebyFilter, rawPPG);

      // Simpan ke buffer
      if (sampleIdx < BUFFER_SIZE) {
        sinyalBuffer[sampleIdx] = filteredPPG;
        sampleIdx++;
      }

      particleSensor.nextSample();
    }
  }

  // 2. BLOK PENGIRIMAN (Dilakukan "cicil" agar tidak memblokir micros)
  if (segmentReady) {
    // Jangan pakai for-loop 200 kali sekaligus!
    // Itu akan memakan waktu mS yang lama dan merusak jadwal micros() di atas.

    // Solusi: Kirim data satu per satu setiap kali loop() berjalan
    // (Time-Slicing)
    static uint8_t sendIdx = 0;

    Serial.println(sinyalBuffer[sendIdx]);
    sendIdx++;

    if (sendIdx >= BUFFER_SIZE) {
      sendIdx = 0;
      segmentReady = false; // Selesai kirim satu blok
    }
  }

  // 3. LOGIKA PEMOTONGAN (Kapan segmentReady aktif)
  // Kamu bisa aktifkan segmentReady berdasarkan jumlah sampel (200)
  // atau berdasarkan State Machine (V1-P-V2)
  if (!segmentReady && sampleIdx >= BUFFER_SIZE) {
    segmentReady = true;
    sampleIdx = 0;
  }
}