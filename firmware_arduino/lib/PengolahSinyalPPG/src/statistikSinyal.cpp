#include "statistikSinyal.h"

uint16_t hitungSkewness(uint16_t *buffer, uint8_t start, uint8_t end) {
  int n = end - start + 1;
  if (n < 3)
    return 0;

  uint32_t sum = 0;
  for (int i = start; i <= end; i++)
    sum += (uint32_t)buffer[i];
  uint32_t mean = sum / n;

  uint32_t m2 = 0; // Ragam
  uint32_t m3 = 0; // Kemiringan
  for (int i = start; i <= end; i++) {
    int32_t diff = (int32_t)buffer[i] - mean;
    uint32_t diffSq = (uint32_t)diff * diff;
    m2 += diffSq;
    m3 += diffSq * diff;
  }

  float variance = m2 / n;
  float stdDev = sqrt(variance);

  if (stdDev < 0.001)
    return 0; // Hindari pembagian nol atau sinyal datar

  return (m3 / n) / (stdDev * stdDev * stdDev);
}

uint16_t hitungKurtosis(uint16_t *buffer, uint8_t start, uint8_t end) {
  int n = end - start + 1;
  if (n < 4)
    return 0;

  uint32_t sum = 0;
  for (int i = start; i <= end; i++)
    sum += (uint32_t)buffer[i];
  uint32_t mean = sum / n;

  uint32_t m2 = 0, m4 = 0;
  for (int i = start; i <= end; i++) {
    int32_t diff = (int32_t)buffer[i] - mean;
    uint32_t diffSq = (uint32_t)diff * diff;
    m2 += diffSq;
    m4 += diffSq * diffSq; // Pangkat 4 untuk Kurtosis
  }

  float variance = m2 / n;
  if (variance < 0.001)
    return 0;

  // Kurtosis = (m4/n) / (variance^2)
  return (m4 / n) / (variance * variance);
}