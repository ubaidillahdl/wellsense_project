#include "puncakValid.h"

void puncakValid(uint8_t *idxPeaks, uint8_t nPeaks, uint8_t *idxValleys,
                 uint8_t nValleys, uint8_t *outValidPeaks, uint8_t *outVBefore,
                 uint8_t *outVAfter, uint8_t *outNumValid) {
  uint8_t count = 0;

  for (uint8_t i = 0; i < nPeaks; i++) {
    uint8_t currentPeak = idxPeaks[i];
    int16_t foundPrevValley = -1;
    int16_t foundNextValley = -1;
    // uint8_t minDist = 5;
    // uint8_t maxDist = 100;

    // 1. Cari lembah terdekat SEBELUM puncak (Cek ke arah kiri)
    for (int8_t j = nValleys - 1; j >= 0; j--) {
      uint8_t vLoc = idxValleys[j];
      if (vLoc < currentPeak) {
        // uint8_t dist = currentPeak - vLoc;
        // if (dist >= minDist && dist <= maxDist)
        // {
        foundPrevValley = vLoc;
        // }
        break; // Stop pencarian setelah menemukan lembah pertama di kiri
      }
    }

    // 2. Cari lembah terdekat SESUDAH puncak (Cek ke arah kanan)
    for (uint8_t j = 0; j < nValleys; j++) {
      uint8_t vLoc = idxValleys[j];
      if (vLoc > currentPeak) {
        // uint8_t dist = vLoc - currentPeak;
        // if (dist >= minDist && dist <= maxDist)
        // {
        foundNextValley = vLoc;
        // }
        break; // Stop pencarian setelah menemukan lembah pertama di kanan
      }
    }

    // 3. Validasi Siklus: Harus punya dua lembah (Kiri & Kanan)
    if (foundPrevValley != -1 && foundNextValley != -1) {
      // Kunci Integritas: Urutan indeks harus V_sebelum < Puncak < V_sesudah
      if (foundPrevValley < currentPeak && currentPeak < foundNextValley) {
        outValidPeaks[count] = currentPeak;
        outVBefore[count] = (uint16_t)foundPrevValley;
        outVAfter[count] = (uint16_t)foundNextValley;
        count++;
      }
    }

    // Safety Break: Jangan melebihi ukuran array output
    if (count >= 10)
      break;
  }

  // Kirim total puncak valid kembali ke loop utama
  *outNumValid = count;
}