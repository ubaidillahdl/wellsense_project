#ifndef CHEBY_FILTER_H
#define CHEBY_FILTER_H

#include "dcRemoval.h"
#include <Arduino.h>

// Struktur untuk menyimpan history (state) setiap section (SOS)
typedef struct {
  float w1;
  float w2;
} ChebySosState;

// Struktur utama filter
typedef struct {
  ChebySosState sections[4];
  DCRemover dc;
} ChebyFilter;

// Fungsi inisialisasi filter
void chebyInit(ChebyFilter *f);

// Fungsi pemrosesan sinyal
float chebyProcess(ChebyFilter *f, float input);

#endif