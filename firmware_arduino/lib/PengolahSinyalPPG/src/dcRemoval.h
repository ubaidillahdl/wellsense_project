#ifndef DC_REMOVER_H
#define DC_REMOVER_H

#include "Arduino.h"

// Struktur untuk menyimpan status (state) filter
typedef struct {
  float a;      // Konstanta filter (Persamaan 17)
  float w_prev; // Menyimpan nilai w(t-1)
} DCRemover;

// Fungsi inisialisasi
void dcRemoverInit(DCRemover *filter, float alpha);
float dcRemoverProcess(DCRemover *filter, float x_t);
void dcRemoverBuffer(int16_t *buffer, uint8_t bufferLength);
void applyCompactIIR(int16_t *data, uint8_t length, float b);

#endif