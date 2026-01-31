#ifndef PENGOLAH_SINYAL_PPG_H
#define PENGOLAH_SINYAL_PPG_H

#include "cariLembah.h"
#include "cariPuncak.h"
#include "chebyFilter.h"
#include "dcRemoval.h"
#include "detektorSiklus.h"
#include "ppgTracker.h"
#include "puncakValid.h"
#include "signalFilter.h"

float cariNilaiMax(float *sinyalBuffer, uint8_t panjangBuffer);

#endif