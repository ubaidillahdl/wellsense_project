#ifndef PUNCAK_H
#define PUNCAK_H

#include <Arduino.h>

void detektorSiklus(uint16_t *sinyalBuffer, uint8_t panjangBuffer,

                    uint8_t *outValidPeaks, uint8_t *outVBefore,
                    uint8_t *outVAfter, uint8_t *outNumValid);

#endif