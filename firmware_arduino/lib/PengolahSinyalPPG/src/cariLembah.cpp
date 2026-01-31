#include "cariLembah.h"

void cariLembah(uint16_t *sinyalBuffer, uint8_t panjangBuffer,
                uint8_t minProminence, uint8_t minDistance,
                uint16_t *outValleyValues, uint8_t *outValleyLocs,
                uint8_t *outNumValleys) {
  // --- LANGKAH 0: Hitung Rata-rata (Baseline) untuk Zero Crossing ---
  uint32_t totalSum = 0;
  for (uint8_t i = 0; i < panjangBuffer; i++)
    totalSum += sinyalBuffer[i];
  uint32_t mean = totalSum / panjangBuffer;

  uint8_t zcCount = 0;
  for (uint8_t i = 1; i < panjangBuffer; i++) {
    if ((sinyalBuffer[i - 1] < mean && sinyalBuffer[i] >= mean) ||
        (sinyalBuffer[i - 1] > mean && sinyalBuffer[i] <= mean))
      zcCount++;
  }

  // --- LANGKAH 2: PENJAGA (Early Exit) ---
  // Jika perpotongan nol terlalu banyak (> 10) atau terlalu sedikit (< 2)
  // maka sinyal dianggap sampah (noise/flat).
  if (zcCount < 2 || zcCount > 10) {
    *outNumValleys = 0; // Kembalikan 0 puncak
    return;             // Berhenti di sini, hemat CPU!
  }

  // Langkah 1: Cari kandidat lembah
  uint8_t candidateLocs[15];
  uint8_t nCandidates =
      kandidatLembah(sinyalBuffer, panjangBuffer, candidateLocs);

  uint8_t validCount = 0;
  int16_t lastPeakLoc =
      -100; // Inisialisasi dengan nilai jauh agar puncak pertama lolos

  float minPromThreshold = minProminence;
  uint8_t minDistanceThreshold = minDistance;

  for (uint8_t i = 0; i < nCandidates; i++) {
    uint8_t idx = candidateLocs[i];
    float valleyVal = sinyalBuffer[idx];

    // --- TAMBAHAN ZERO CROSSING CHECK ---
    // Lembah hanya dianggap valid jika nilainya di BAWAH rata-rata (Fase
    // Negatif)
    if (valleyVal >= mean)
      continue;

    // --- 1. LOGIKA MIN DISTANCE ---
    // Jika jarak antara kandidat saat ini (idx) dengan puncak sebelumnya
    // terlalu dekat, abaikan.
    if (idx - lastPeakLoc < minDistanceThreshold)
      continue;

    // --- Cari lembah KIRI ---
    float leftMax = valleyVal;
    for (int8_t j = idx - 1; j >= 0; j--) {
      if (sinyalBuffer[j] < valleyVal)
        break;
      if (sinyalBuffer[j] > leftMax)
        leftMax = sinyalBuffer[j];
      if (sinyalBuffer[j] > mean)
        break; // Berhenti jika sudah menyeberang nol (Zero Crossing)
    }

    // --- Cari lembah KANAN ---
    float rightMax = valleyVal;
    for (int8_t j = idx + 1; j < panjangBuffer; j++) {
      if (sinyalBuffer[j] > rightMax)
        rightMax = sinyalBuffer[j];
      if (sinyalBuffer[j] < valleyVal)
        break;
      if (sinyalBuffer[j] > mean)
        break; // Berhenti jika sudah menyeberang nol (Zero Crossing)
    }

    // --- Logika Prominence: valleyVal - max(leftMax, rightMax) ---
    float lowerPeak = (leftMax < rightMax) ? leftMax : rightMax;
    float prominence = lowerPeak - valleyVal;

    if (prominence >= minPromThreshold) {
      outValleyValues[validCount] = valleyVal;
      outValleyLocs[validCount] = idx;

      lastPeakLoc = idx;
      validCount++;
    }
  }
  *outNumValleys = validCount; // Beritahu .ino berapa yang valid
}

uint8_t kandidatLembah(uint16_t *sinyalBuffer, uint8_t panjangBuffer,
                       uint8_t *indeks) {
  uint8_t count = 0;
  uint8_t gap =
      3; // Jarak perbandingan untuk memastikan dasar lembah yang solid

  // Mulai dari indeks 'gap' agar saat akses i - gap tidak negatif (error)
  for (uint8_t i = gap; i < panjangBuffer - gap; i++) {
    // Syarat: Titik i harus lebih rendah dari titik i-3 DAN i+3
    if (sinyalBuffer[i] < sinyalBuffer[i - gap] &&
        sinyalBuffer[i] < sinyalBuffer[i + gap]) {
      indeks[count] = i; // Simpan indeks kandidat lembah
      count++;

      if (count >= 15)
        break; // Safety limit

      // Opsional: Loncat i jika ingin menghindari deteksi titik yang berdekatan
      // i += gap;
    }
  }
  return count;
}