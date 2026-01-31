#ifndef STATISTIK_SINYAL_H
#define STATISTIK_SINYAL_H

#include "Arduino.h"

// Daftar fungsi (Prototipe)
uint16_t hitungSkewness(uint16_t *buffer, uint8_t start, uint8_t end);
uint16_t hitungKurtosis(uint16_t *buffer, uint8_t start, uint8_t end);

#endif