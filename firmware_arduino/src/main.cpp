#include "MAX30105.h"
#include <PengolahSinyalPPG.h>
#include <Wire.h>


MAX30105 particleSensor;
ChebyFilter chebyFilter;

#define BUFFER_SIZE 200
// #define SHIFT_SIZE 50

// --- KONFIGURASI BUFFER & PARAMETER ---
float sinyalBuffer[BUFFER_SIZE]; // Tempat menyimpan 100 data terakhir

// --- STORAGE HASIL FUNGSI PUNCAK ---
uint8_t outValidPeaks[5];
uint8_t outVBefore[5];
uint8_t outVAfter[5];
uint8_t outNumValid;

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
  uint8_t ledBrightness =
      255; // Kecerahan LED (127): Tengah-tengah agar tidak cepat panas
  uint8_t sampleAverage =
      1; // Rata-rata sampel: 4 data digabung jadi 1 (mengurangi noise)
  uint8_t ledMode =
      2; // Mode LED: 2 = Red + IR (IR paling sensitif untuk detak jantung)
  uint8_t sampleRate =
      200; // Frekuensi pengambilan data (100 Hz): Cukup untuk sinyal PPG
  uint16_t pulseWidth = 411; // Lebar pulsa: 411 µs (mempengaruhi resolusi ADC)
  uint16_t adcRange =
      16384; // Range ADC: 4096 (resolusi 12-bit untuk detail sinyal)

  // Terapkan semua konfigurasi ke sensor
  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate,
                       pulseWidth, adcRange);

  // --- TAHAP 1: INISIALISASI BUFFER ---
  Serial.print("Ambil sampel awal");
  for (byte i = 0; i < BUFFER_SIZE; i++) {
    while (particleSensor.available() == false)
      particleSensor.check();
    sinyalBuffer[i] =
        chebyProcess(&chebyFilter, (float)particleSensor.getRed());
    particleSensor.nextSample();
    if (i % 15 == 0)
      Serial.print(".");
  }
}

void loop() {
  // --- TAHAP 1: PENGUMPULAN DATA (BATCH) ---
  // Kita isi seluruh buffer dari indeks 0 sampai 99
  for (byte i = 0; i < BUFFER_SIZE; i++) {
    while (particleSensor.available() == false)
      particleSensor.check();
    sinyalBuffer[i] =
        chebyProcess(&chebyFilter, (float)particleSensor.getRed());
    particleSensor.nextSample();
  }

  // 3. DETEKSI (Mencari puncak & lembah di seluruh 100 data)
  detektorSiklus(sinyalBuffer, BUFFER_SIZE, outValidPeaks, outVBefore,
                 outVAfter, &outNumValid);

  // Kirim Data Tengah: Kirim indeks 50-100 ke MATLAB agar marker pas di puncak
  // (Data tengah dipilih supaya library sudah punya info data sebelum &
  // sesudahnya)
  for (byte i = 0; i < BUFFER_SIZE; i++) {
    float pVal = 0;  // Default 0 jika bukan puncak
    float lbVal = 0; // Default 0 jika bukan lembah
    float laVal = 0; // Default 0 jika bukan lembah

    // Cek marker di indeks i
    for (uint8_t j = 0; j < outNumValid; j++) {
      if (i == outValidPeaks[j])
        pVal = sinyalBuffer[i];
      if (i == outVBefore[j])
        lbVal = sinyalBuffer[i];
      if (i == outVAfter[j])
        laVal = sinyalBuffer[i];
    }

    // Format: Sinyal | Puncak | L-Sblm | L-Ssdh
    Serial.print("RT:");
    Serial.print(sinyalBuffer[i]);
    Serial.print("\t");
    Serial.print(pVal);
    Serial.print("\t");
    Serial.print(lbVal);
    Serial.print("\t");
    Serial.println(laVal);
  }
}