#ifndef PENGOLAH_SINYAL_PPG_H
#define PENGOLAH_SINYAL_PPG_H

#include "cariLembah.h"
#include "cariPuncak.h"
#include "dcRemoval.h"
#include "detektorSiklus.h"
#include "puncakValid.h"
#include "signalFilter.h"
#include "statistikSinyal.h"

float cariNilaiMax(uint16_t *sinyalBuffer, uint8_t panjangBuffer);

#endif