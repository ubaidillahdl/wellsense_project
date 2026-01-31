#include "MAX30105.h"
#include <PengolahSinyalPPG.h>
#include <Wire.h>

MAX30105 particleSensor;
DCRemover hapusDC;

// --- KONFIGURASI BUFFER & PARAMETER ---
uint16_t sinyalBuffer[100];  // Tempat menyimpan 100 data terakhir
uint8_t panjangBuffer = 100; // Ukuran jendela pengamatan
uint8_t minProminence = 90; // Ambang batas sensitivitas deteksi puncak (semakin
                            // tinggi semakin selektif)

// --- PENYIMPANAN HASIL DETEKSI ---
uint8_t outPeakLocs[15];    // Lokasi (indeks) puncak ditemukan
uint16_t outPeakValues[15]; // Nilai (amplitudo) puncak ditemukan
uint8_t outNumPeaks;        // Jumlah puncak yang berhasil dideteksi

uint8_t outValleyLocs[15];    // Lokasi (indeks) lembah ditemukan
uint16_t outValleyValues[15]; // Nilai (amplitudo) lembah ditemukan
uint8_t outNumValleys;        // Jumlah lembah yang berhasil dideteksi

void setup() {
  Serial.begin(115200);

  // Inisialisasi Sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println(F("Sensor tidak ditemukan"));
    while (1)
      ;
  }

  // --- SETTING SENSOR (Sangat Mempengaruhi Kualitas Sinyal) ---
  byte ledBrightness =
      255; // Kecerahan LED (127): Tengah-tengah agar tidak cepat panas
  byte sampleAverage =
      1; // Rata-rata sampel: 4 data digabung jadi 1 (mengurangi noise)
  byte ledMode =
      2; // Mode LED: 2 = Red + IR (IR paling sensitif untuk detak jantung)
  byte sampleRate =
      100; // Frekuensi pengambilan data (100 Hz): Cukup untuk sinyal PPG
  int pulseWidth = 411; // Lebar pulsa: 411 µs (mempengaruhi resolusi ADC)
  int adcRange = 16384; // Range ADC: 4096 (resolusi 12-bit untuk detail sinyal)

  // Terapkan semua konfigurasi ke sensor
  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate,
                       pulseWidth, adcRange);

  // --- TAHAP 1: INISIALISASI BUFFER ---
  Serial.print("Ambil sampel awal");
  for (byte i = 0; i < panjangBuffer; i++) {
    while (particleSensor.available() == false)
      particleSensor.check();
    sinyalBuffer[i] = signalFilter(particleSensor.getRed());
    particleSensor.nextSample();
    if (i % 15 == 0)
      Serial.print(".");
  }
}

void loop() {
  // --- TAHAP 2: LOOPING UTAMA (REAL-TIME) ---
  // Geser buffer: buang 25 data terlama di kiri
  memmove(&sinyalBuffer[0], &sinyalBuffer[25],
          (panjangBuffer - 25) * sizeof(float));

  // Isi data baru: tambahkan 25 data baru di kanan (indeks 75-99)
  for (byte i = 75; i < panjangBuffer; i++) {
    while (particleSensor.available() == false)
      particleSensor.check();
    sinyalBuffer[i] = signalFilter(particleSensor.getRed());
    particleSensor.nextSample();
  }

  // Hitung Fitur: Cari posisi puncak & lembah di seluruh buffer 100 data
  cariPuncak(sinyalBuffer, panjangBuffer, 30, 50, outPeakValues, outPeakLocs,
             &outNumPeaks);
  cariLembah(sinyalBuffer, panjangBuffer, 30, 50, outValleyValues,
             outValleyLocs, &outNumValleys);

  // Kirim Data Tengah: Kirim indeks 25-50 ke MATLAB agar marker pas di puncak
  // (Data tengah dipilih supaya library sudah punya info data sebelum &
  // sesudahnya)
  for (byte i = 25; i < 50; i++) {
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