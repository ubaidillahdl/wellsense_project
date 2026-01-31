#ifndef CARI_LEMBAH_H
#define CARI_LEMBAH_H

#include <Arduino.h>

// Deklarasi fungsi (hanya judulnya saja)
void cariLembah(float *sinyalBuffer, uint8_t panjangBuffer,
                uint8_t minProminence, uint8_t minDistance,
                float *outPeakValues, uint8_t *outPeakLocs,
                uint8_t *outNumPeaks);
uint8_t kandidatLembah(float *sinyalBuffer, uint8_t panjangBuffer,
                       uint8_t *indeks);

#endif