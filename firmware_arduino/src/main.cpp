#include "MAX30105.h"
#include <PengolahSinyalPPG.h>
#include <Wire.h>

MAX30105 particleSensor;

#define BUFFER_SIZE 100
#define SHIFT_SIZE 25

// --- BUFFER UTAMA ---
uint16_t sinyalBuffer[BUFFER_SIZE];
bool isPeak[BUFFER_SIZE];
bool isTroughBefore[BUFFER_SIZE];
bool isTroughAfter[BUFFER_SIZE];

// --- STORAGE HASIL FUNGSI PUNCAK ---
uint8_t outValidPeaks[10];
uint8_t outVBefore[10];
uint8_t outVAfter[10];
uint8_t outNumValid;

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
  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate,
                       pulseWidth, adcRange);

  // Fill buffer awal agar tidak kosong saat loop pertama
  Serial.print("Ambil sampel awal");
  for (byte i = 0; i < BUFFER_SIZE; i++) {
    while (particleSensor.available() == false)
      particleSensor.check();
    sinyalBuffer[i] = signalFilter(particleSensor.getRed());
    particleSensor.nextSample();
    if (i % 15 == 0)
      Serial.print(".");
  }
}

void loop() {
  // 1. GESER BUFFER (Membuang 25 data tertua, menyisakan ruang di akhir)
  memmove(&sinyalBuffer[0], &sinyalBuffer[SHIFT_SIZE], 75 * sizeof(uint16_t));
  memmove(&isPeak[0], &isPeak[SHIFT_SIZE], 75 * sizeof(bool));
  memmove(&isTroughBefore[0], &isTroughBefore[SHIFT_SIZE], 75 * sizeof(bool));
  memmove(&isTroughAfter[0], &isTroughAfter[SHIFT_SIZE], 75 * sizeof(bool));

  // 2. ISI DATA BARU (Ke indeks 75 - 99)
  for (byte i = (BUFFER_SIZE - SHIFT_SIZE); i < BUFFER_SIZE; i++) {
    while (particleSensor.available() == false)
      particleSensor.check();
    sinyalBuffer[i] = signalFilter(particleSensor.getRed());

    // Reset status data baru (wajib agar marker lama tidak terbawa)
    isPeak[i] = false;
    isTroughBefore[i] = false;
    isTroughAfter[i] = false;

    particleSensor.nextSample();
  }

  // 3. DETEKSI (Mencari puncak & lembah di seluruh 100 data)
  detektorSiklus(sinyalBuffer, BUFFER_SIZE, outValidPeaks, outVBefore,
                 outVAfter, &outNumValid);

  // 4. UPDATE STATUS (Sinkronisasi hasil deteksi ke array boolean)
  // Tandai berdasarkan hasil hitungan terbaru
  for (uint8_t j = 0; j < outNumValid; j++) {
    isPeak[outValidPeaks[j]] = true;
    isTroughBefore[outVBefore[j]] = true;
    isTroughAfter[outVAfter[j]] = true;
  }

  // 5. KIRIM DATA MATANG (Indeks 0-24)
  // Data dikirim setelah 'menginap' di buffer agar divalidasi oleh data
  // sesudahnya
  for (byte i = 0; i < SHIFT_SIZE; i++) {
    uint16_t currentSignal = sinyalBuffer[i];
    uint16_t pVal = 0;
    uint16_t lbVal = 0;
    uint16_t laVal = 0;

    if (isPeak[i])
      pVal = currentSignal;
    if (isTroughBefore[i])
      lbVal = currentSignal;
    if (isTroughAfter[i])
      laVal = currentSignal;

    // Format: Sinyal | Puncak | L-Sblm | L-Ssdh
    Serial.print("RT:");
    Serial.print(currentSignal);
    Serial.print("\t");
    Serial.print(pVal);
    Serial.print("\t");
    Serial.print(lbVal);
    Serial.print("\t");
    Serial.println(laVal);
  }

  // 6. KIRIM SIKLUS UTUH (Grafik Bawah - Burst/Semburan)
  // Kita cek apakah ada siklus yang baru saja selesai (VAfter ada di area data
  // baru 75-99)
  for (uint8_t j = 0; j < outNumValid; j++) {
    uint8_t idxVBefore = outVBefore[j];
    uint8_t idxPeak = outValidPeaks[j];
    uint8_t idxVAfter = outVAfter[j];

    if (outVAfter[j] >= 75) {

      // 1. Hitung Jarak (Waktu)
      uint8_t jarakNaik = idxPeak - idxVBefore; // Durasi Sistolik
      uint8_t jarakTurun = idxVAfter - idxPeak; // Durasi Diastolik
      uint8_t lebarTotal = idxVAfter - idxVBefore;

      uint8_t start = outVBefore[j];
      uint8_t end = outVAfter[j];

      float nilaiSkew = hitungSkewness(sinyalBuffer, start, end);
      float nilaiKurt = hitungKurtosis(sinyalBuffer, start, end);

      if (abs(nilaiSkew) > 0.1 && nilaiKurt > 1.5) {
        // --- PERKETAT DI SINI ---
        // 1. Rasio: Jarak turun harus minimal 1.5x lebih panjang dari jarak
        // naik
        // 2. Amplitudo: (Opsional) Cek selisih nilai agar tidak memproses garis
        // datar
        bool rasioOke = (jarakTurun >= (1.5 * jarakNaik));
        bool lebarOke =
            (lebarTotal > 50 && lebarTotal < 95); // Detak normal 60-120 BPM

        if (rasioOke && lebarOke) {
          // Kirim instruksi ke MATLAB untuk reset Plot 2 (Opsional jika MATLAB
          // mendukung)
          Serial.println("START_PLOT_2");

          // Kirim seluruh titik dalam siklus tersebut sekaligus
          for (uint8_t k = start; k <= end; k++) {
            uint16_t sig = sinyalBuffer[k];
            uint16_t p = 0;
            uint16_t lb = 0;
            uint16_t la = 0;

            if (k == outValidPeaks[j])
              p = sig;
            if (k == outVBefore[j])
              lb = sig;
            if (k == outVAfter[j])
              la = sig;

            Serial.print("CYC:");
            Serial.print(k - start); // X: Indeks lokal dimulai dari 0
            Serial.print("\t");
            Serial.print(sig); // Y: Sinyal
            Serial.print("\t");
            Serial.print(p); // Marker Puncak
            Serial.print("\t");
            Serial.print(lb); // Marker L-Sebelum
            Serial.print("\t");
            Serial.println(la); // Marker L-Sesudah
            Serial.print("\t");
            Serial.print(nilaiSkew);
            Serial.print("\t");
            Serial.println(nilaiKurt);
          }
          Serial.println("END_PLOT_2");
        }
      }
    }
  }
}