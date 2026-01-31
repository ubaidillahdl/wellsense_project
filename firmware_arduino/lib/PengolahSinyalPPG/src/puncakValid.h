#ifndef PUNCAK_VALID_H
#define PUNCAK_VALID_H

#include <Arduino.h>

void puncakValid(uint8_t *idxPeaks, uint8_t nPeaks, uint8_t *idxValleys, uint8_t nValleys, uint8_t *outValidPeaks, uint8_t *outVBefore, uint8_t *outVAfter, uint8_t *outNumValid);

#endif