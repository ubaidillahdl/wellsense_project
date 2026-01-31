#include "MAX30105.h"
#include <PengolahSinyalPPG.h>
#include <Wire.h>

MAX30105 particleSensor;
ChebyFilter chebyFilter;

#define BUFFER_SIZE 200
#define SHIFT_SIZE BUFFER_SIZE / 4

// --- KONFIGURASI BUFFER & PARAMETER ---
float sinyalBuffer[BUFFER_SIZE]; // Tempat menyimpan 100 data terakhir

// --- PENYIMPANAN HASIL DETEKSI ---
uint8_t outPeakLocs[5]; // Lokasi (indeks) puncak ditemukan
float outPeakValues[5]; // Nilai (amplitudo) puncak ditemukan
uint8_t outNumPeaks;    // Jumlah puncak yang berhasil dideteksi

uint8_t outValleyLocs[5]; // Lokasi (indeks) lembah ditemukan
float outValleyValues[5]; // Nilai (amplitudo) lembah ditemukan
uint8_t outNumValleys;    // Jumlah lembah yang berhasil dideteksi

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
      255; // Kecerahan LED (255): Tengah-tengah agar tidak cepat panas
  uint8_t sampleAverage =
      1; // Rata-rata sampel: 4 data digabung jadi 1 (mengurangi noise)
  uint8_t ledMode =
      2; // Mode LED: 2 = Red + IR (IR paling sensitif untuk detak jantung)
  uint8_t sampleRate = BUFFER_SIZE; // Frekuensi pengambilan data (200 Hz):
                                    // Cukup untuk sinyal PPG
  uint16_t pulseWidth = 411; // Lebar pulsa: 411 µs (mempengaruhi resolusi ADC)
  uint16_t adcRange =
      16384; // Range ADC: 16384 (resolusi 12-bit untuk detail sinyal)

  // Terapkan semua konfigurasi ke sensor
  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate,
                       pulseWidth, adcRange);

  // --- TAHAP 1: INISIALISASI BUFFER ---
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
  // --- TAHAP 2: LOOPING UTAMA (REAL-TIME) ---
  // Geser buffer: buang 50 data terlama di kiri
  memmove(&sinyalBuffer[0], &sinyalBuffer[SHIFT_SIZE],
          (BUFFER_SIZE - SHIFT_SIZE) * sizeof(float));

  // Isi data baru: tambahkan 50 data baru di kanan (indeks 150-199)
  for (byte i = (BUFFER_SIZE - SHIFT_SIZE); i < BUFFER_SIZE; i++) {
    while (particleSensor.available() == false)
      particleSensor.check();
    sinyalBuffer[i] =
        chebyProcess(&chebyFilter, (float)particleSensor.getRed());
    particleSensor.nextSample();
  }

  float minPromP = 0.7 * cariNilaiMax(sinyalBuffer, BUFFER_SIZE);
  float minPromL = 0.5 * cariNilaiMax(sinyalBuffer, BUFFER_SIZE);
  uint8_t minDist = 0.4 * BUFFER_SIZE;

  cariPuncak(sinyalBuffer, BUFFER_SIZE, minPromP, minDist, outPeakValues,
             outPeakLocs, &outNumPeaks);
  cariLembah(sinyalBuffer, BUFFER_SIZE, minPromL, minDist, outValleyValues,
             outValleyLocs, &outNumValleys);

  // Kirim Data Tengah: Kirim indeks 50-100 ke MATLAB agar marker pas di puncak
  // (Data tengah dipilih supaya library sudah punya info data sebelum &
  // sesudahnya)
  for (byte i = SHIFT_SIZE; i < BUFFER_SIZE / 2; i++) {
    float pVal = 0; // Default 0 jika bukan puncak
    float lVal = 0; // Default 0 jika bukan lembah

    // Cek apakah indeks saat ini adalah lokasi puncak/lembah hasil hitungan
    for (uint8_t j = 0; j < outNumPeaks; j++) {
      if (i == outPeakLocs[j])
        pVal = sinyalBuffer[i];
    }
    for (uint8_t j = 0; j < outNumValleys; j++) {
      if (i == outValleyLocs[j])
        lVal = sinyalBuffer[i];
    }

    // Format pengiriman untuk MATLAB Plotter
    Serial.print("RT:");
    Serial.print(sinyalBuffer[i]); // Kolom 1: Sinyal
    Serial.print("\t");
    Serial.print(pVal); // Kolom 2: Marker Puncak
    Serial.print("\t");
    Serial.println(lVal); // Kolom 3: Marker Lembah
  }
}