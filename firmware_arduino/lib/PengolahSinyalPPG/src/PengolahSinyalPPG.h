#ifndef PENGOLAH_SINYAL_PPG_H
#define PENGOLAH_SINYAL_PPG_H

#include "cariLembah.h"
#include "cariPuncak.h"
#include "dcRemoval.h"
#include "signalFilter.h"

float cariNilaiMax(float *sinyalBuffer, uint8_t panjangBuffer);

#endif