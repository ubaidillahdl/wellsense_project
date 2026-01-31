#ifndef CARI_LEMBAH_H
#define CARI_LEMBAH_H

#include <Arduino.h>

// Deklarasi fungsi (hanya judulnya saja)
void cariLembah(uint16_t *sinyalBuffer, uint8_t panjangBuffer,
                uint8_t minProminence, uint8_t minDistance,
                uint16_t *outPeakValues, uint8_t *outPeakLocs,
                uint8_t *outNumPeaks);
uint8_t kandidatLembah(uint16_t *sinyalBuffer, uint8_t panjangBuffer,
                       uint8_t *indeks);

#endif