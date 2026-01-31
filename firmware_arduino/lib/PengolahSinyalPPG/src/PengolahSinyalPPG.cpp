#include "PengolahSinyalPPG.h"

// Fungsi mencari nilai tertinggi di buffer untuk penentuan threshold dinamis
float cariNilaiMax(uint16_t *sinyalBuffer, uint8_t panjangBuffer) {
  float nilaiMax = sinyalBuffer[0];
  for (uint8_t i = 1; i < panjangBuffer; i++) {
    if (sinyalBuffer[i] > nilaiMax) {
      nilaiMax = sinyalBuffer[i]; // Update jika ditemukan yang lebih besar
    }
  }
  return nilaiMax;
}