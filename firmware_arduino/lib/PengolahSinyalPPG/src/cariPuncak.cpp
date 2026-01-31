#include "cariPuncak.h"

void cariPuncak(uint16_t *sinyalBuffer, uint8_t panjangBuffer,
                uint8_t minProminence, uint8_t minDistance,
                uint16_t *outPeakValues, uint8_t *outPeakLocs,
                uint8_t *outNumPeaks) {
  // --- LANGKAH 0: Hitung Rata-rata (Baseline) untuk Zero Crossing ---
  uint32_t totalSum = 0;
  for (uint8_t i = 0; i < panjangBuffer; i++)
    totalSum += sinyalBuffer[i];
  uint32_t mean = totalSum / (float)panjangBuffer;

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
    *outNumPeaks = 0; // Kembalikan 0 puncak
    return;           // Berhenti di sini, hemat CPU!
  }

  // Langkah 1: Cari semua titik lokal maksimum (titik yang lebih tinggi dari
  // tetangga kiri-kanan)
  uint8_t candidateLocs[15];
  uint8_t nCandidates =
      kandidatPuncak(sinyalBuffer, panjangBuffer, candidateLocs);

  uint8_t validCount = 0;
  int16_t lastPeakLoc =
      -100; // Penanda lokasi puncak sebelumnya untuk filter jarak

  // Hitung ambang batas (Threshold) berdasarkan persentase terhadap nilai
  // tertinggi sinyal uint16_t globalMax = nilaiMax(sinyalBuffer,
  // panjangBuffer);
  float minPromThreshold = minProminence;
  uint8_t minDistanceThreshold = minDistance; // Ambang jarak antar puncak

  for (uint8_t i = 0; i < nCandidates; i++) {
    uint8_t idx = candidateLocs[i];
    float peakVal = sinyalBuffer[idx];

    // --- TAMBAHAN ZERO CROSSING CHECK ---
    // Puncak hanya dianggap valid jika nilainya di atas rata-rata (Fase
    // Positif)
    if (peakVal <= mean)
      continue;

    // Filter 1: Jika jarak ke puncak sebelumnya terlalu dekat, skip (mencegah
    // double count di 1 bukit)
    if (idx - lastPeakLoc < minDistanceThreshold)
      continue;

    // Filter 2: Cari titik terendah (lembah) di sisi KIRI puncak
    float leftMin = peakVal;
    for (int8_t j = idx - 1; j >= 0; j--) {
      if (sinyalBuffer[j] > peakVal)
        break; // Berhenti jika ada titik yang lebih tinggi dari puncak
      if (sinyalBuffer[j] < leftMin)
        leftMin = sinyalBuffer[j];
      if (sinyalBuffer[j] < mean)
        break; // Berhenti jika sudah menyeberang nol (Zero Crossing)
    }

    // Filter 3: Cari titik terendah (lembah) di sisi KANAN puncak
    float rightMin = peakVal;
    for (int8_t j = idx + 1; j < panjangBuffer; j++) {
      if (sinyalBuffer[j] > peakVal)
        break;
      if (sinyalBuffer[j] < rightMin)
        rightMin = sinyalBuffer[j];
      if (sinyalBuffer[j] < mean)
        break; // Berhenti jika sudah menyeberang nol (Zero Crossing)
    }

    // Hitung Prominence: Seberapa menonjol puncak terhadap lembah pengapitnya
    float higherLembah = (leftMin > rightMin) ? leftMin : rightMin;
    float prominence = peakVal - higherLembah;

    // Jika tonjolan cukup tinggi (bukan noise kecil), simpan sebagai puncak
    // valid
    if (prominence >= minPromThreshold) {
      outPeakValues[validCount] = peakVal;
      outPeakLocs[validCount] = idx;

      lastPeakLoc = idx;
      validCount++;
    }
  }
  *outNumPeaks = validCount; // Kembalikan jumlah total puncak yang ditemukan
}

uint8_t kandidatPuncak(uint16_t *sinyalBuffer, uint8_t panjangBuffer,
                       uint8_t *indeks) {
  uint8_t count = 0;
  uint8_t gap = 1; // Jarak perbandingan kiri-kanan

  // Loop dimulai dari 'gap' dan berakhir di 'panjangBuffer - gap'
  // supaya tidak mengakses indeks di luar array
  for (uint8_t i = gap; i < panjangBuffer - gap; i++) {
    // Syarat: Titik i harus lebih tinggi dari titik i-3 DAN i+3
    if (sinyalBuffer[i] >= sinyalBuffer[i - gap] &&
        sinyalBuffer[i] >= sinyalBuffer[i + gap]) {
      indeks[count] = i;
      count++;

      if (count >= 15)
        break;

      // OPTIONAL: Loncatkan 'i' setelah menemukan puncak
      // i += gap;
    }
  }
  return count;
}
