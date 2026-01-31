#ifndef CARI_PUNCAK_H
#define CARI_PUNCAK_H

#include <Arduino.h>

void cariPuncak(float *sinyalBuffer, uint8_t panjangBuffer,
                uint8_t minProminence, uint8_t minDistance,
                float *outPeakValues, uint8_t *outPeakLocs,
                uint8_t *outNumPeaks);
uint8_t kandidatPuncak(float *sinyalBuffer, uint8_t panjangBuffer,
                       uint8_t *indeks);

#endif