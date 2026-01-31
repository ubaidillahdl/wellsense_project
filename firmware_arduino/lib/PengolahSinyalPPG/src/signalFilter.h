#ifndef SIGNAL_FILTER_H
#define SIGNAL_FILTER_H

#include "Arduino.h"

// Daftar fungsi (Prototipe)
int32_t signalFilter(int32_t rawSample);
int32_t estimateDcComponent(int32_t *reg, int32_t sample);
int32_t lowPassFirFilter(int32_t inputAc);
int32_t multiply32(int32_t x, int32_t y);

#endif