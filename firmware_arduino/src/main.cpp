#include <Wire.h>
#include "MAX30105.h"
#include <PengolahSinyalPPG.h>

MAX30105 particleSensor;

// --- KONFIGURASI BUFFER & PARAMETER ---
uint16_t sinyalBuffer[150];   // Tempat menyimpan 100 data terakhir
uint8_t panjangBuffer = 150;  // Ukuran jendela pengamatan

// --- PENYIMPANAN HASIL DETEKSI ---
uint8_t outPeakLocs[15];     // Lokasi (indeks) puncak ditemukan
uint16_t outPeakValues[15];  // Nilai (amplitudo) puncak ditemukan
uint8_t outNumPeaks;         // Jumlah puncak yang berhasil dideteksi

uint8_t outValleyLocs[15];     // Lokasi (indeks) lembah ditemukan
uint16_t outValleyValues[15];  // Nilai (amplitudo) lembah ditemukan
uint8_t outNumValleys;         // Jumlah lembah yang berhasil dideteksi

// --- PENYIMPANAN HASIL VALIDASI ---
uint8_t outValidPeaks[10];  // Indeks puncak yang valid
uint8_t outVBefore[10];     // Indeks lembah sebelum puncak valid
uint8_t outVAfter[10];      // Indeks lembah sesudah puncak valid
uint8_t outNumValid;        // Jumlah puncak valid yang ditemukan

void setup() {
  Serial.begin(115200);

  // Inisialisasi Sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println(F("Sensor tidak ditemukan"));
    while (1)
      ;
  }

  // --- SETTING SENSOR (Sangat Mempengaruhi Kualitas Sinyal) ---
  byte ledBrightness = 255;  // Kecerahan LED (127): Tengah-tengah agar tidak cepat panas
  byte sampleAverage = 1;    // Rata-rata sampel: 4 data digabung jadi 1 (mengurangi noise)
  byte ledMode = 2;          // Mode LED: 2 = Red + IR (IR paling sensitif untuk detak jantung)
  byte sampleRate = 100;     // Frekuensi pengambilan data (100 Hz): Cukup untuk sinyal PPG
  int pulseWidth = 411;      // Lebar pulsa: 411 µs (mempengaruhi resolusi ADC)
  int adcRange = 16384;      // Range ADC: 4096 (resolusi 12-bit untuk detail sinyal)

  // Terapkan semua konfigurasi ke sensor
  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);

  // --- TAHAP 1: INISIALISASI BUFFER ---
  Serial.print("Ambil sampel awal");
  for (byte i = 0; i < panjangBuffer; i++) {
    while (particleSensor.available() == false) particleSensor.check();
    sinyalBuffer[i] = signalFilter(particleSensor.getRed());
    particleSensor.nextSample();
    if (i % 15 == 0) Serial.print(".");
  }
}

void loop() {
  // --- TAHAP 1: PENGUMPULAN DATA (BATCH) ---
  // Kita isi seluruh buffer dari indeks 0 sampai 99
  for (byte i = 0; i < panjangBuffer; i++) {
    while (particleSensor.available() == false) particleSensor.check();
    sinyalBuffer[i] = signalFilter(particleSensor.getRed());
    particleSensor.nextSample();
  }

  // --- TAHAP 2: PERHITUNGAN FITUR ---
  // Algoritma bekerja pada set data statis (tidak berubah selama dihitung)
  // Gunakan parameter yang pas: minDistance sekitar 30-50, minProminence sesuai kekuatan sinyal
  cariPuncak(sinyalBuffer, panjangBuffer, 70, 50, outPeakValues, outPeakLocs, &outNumPeaks);
  cariLembah(sinyalBuffer, panjangBuffer, 70, 50, outValleyValues, outValleyLocs, &outNumValleys);

  // Validasi hubungan Puncak-Lembah
  puncakValid(outPeakLocs, outNumPeaks, outValleyLocs, outNumValleys, outValidPeaks, outVBefore, outVAfter, &outNumValid);

  // --- TAHAP 3: PENGIRIMAN DATA KE MATLAB ---
  // Kirim seluruh 100 data yang baru saja diproses
  for (byte i = 0; i < panjangBuffer; i++) {
    float pVal = 0;   // Default 0 jika bukan puncak
    float lbVal = 0;  // Default 0 jika bukan lembah sebelum
    float laVal = 0;  // Default 0 jika bukan lembah sesudah

    // Cek marker di indeks i
    for (uint8_t j = 0; j < outNumValid; j++) {
      if (i == outValidPeaks[j]) pVal = sinyalBuffer[i];
      if (i == outVBefore[j]) lbVal = sinyalBuffer[i];
      if (i == outVAfter[j]) laVal = sinyalBuffer[i];
    }

    // Format RT:Sinyal [tab] Puncak [tab] VBefore [tab] VAfter
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