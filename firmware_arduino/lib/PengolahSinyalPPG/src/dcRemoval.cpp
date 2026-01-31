#include "dcRemoval.h"

void dcRemoverInit(DCRemover *filter, float alpha) {
  filter->a = alpha;
  filter->w_prev = 0.0f;
}

float dcRemoverProcess(DCRemover *filter, float x_t) {
  // Persamaan (17): w(t) = x(t) + a * w(t-1)
  float w_t = x_t + (filter->a * filter->w_prev);

  // Persamaan (18): y(t) = w(t) - w(t-1)
  // float y_t = w_t - filter->w_prev;
  float y_t = filter->w_prev - w_t; // balik

  // Update state untuk sampel berikutnya
  filter->w_prev = w_t;

  return y_t;
}

void dcRemoverBuffer(int16_t *buffer, uint8_t bufferLength) {
  int32_t mean = 0;
  for (uint8_t i = 0; i < bufferLength; i++)
    mean += buffer[i];
  mean = mean / bufferLength;

  for (int i = 0; i < bufferLength; i++)
    buffer[i] = buffer[i] - mean;
}

// Parameter b (1-d). Semakin kecil b, semakin kuat filternya.
// Misal b = 0.05 untuk filter yang halus.
void applyCompactIIR(int16_t *data, uint8_t length, float b) {
  // Karena stateless per buffer, kita gunakan data pertama sebagai titik awal
  float y = (float)data[0];

  for (uint8_t n = 1; n < length; n++) {
    // Rumus dari artikel: y += b * (x - y)
    y += b * ((float)data[n] - y);
    data[n] = (int16_t)y;
  }
}