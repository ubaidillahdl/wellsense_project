#include "signalFilter.h"

// Variabel internal filter
int32_t signalAcCurrent = 0;
int32_t signalAcPrevious = 0;
int32_t dcOffsetEstimated = 0;
int32_t dcFilterReg = 0;

// Buffer dan indeks untuk FIR
int32_t firBuffer[32];
uint8_t firIndex = 0;

// Tambahkan ini di bagian atas variabel internal filter
int32_t dcBuffer[100];    // Buffer kapasitas 100
uint8_t dcIndex = 0;      // Index berjalan untuk DC
int32_t dcRunningSum = 0; // Variabel pembantu untuk menjumlahkan 25 data

// Koefisien filter
static const uint16_t FIR_COEFFS[12] = {172, 321, 579, 927, 1360, 1858, 2390, 2916, 3391, 3768, 4012, 4096};

/**
 * Fungsi utama untuk memproses sinyal
 */
int32_t signalFilter(int32_t rawSample)
{
  signalAcPrevious = signalAcCurrent;

  // 1. Estimasi komponen DC
  dcOffsetEstimated = estimateDcComponent(&dcFilterReg, rawSample);

  // 2. Filter Low Pass setelah DC dibuang
  signalAcCurrent = -1 * lowPassFirFilter(rawSample - dcOffsetEstimated);
  // signalAcCurrent = rawSample - dcOffsetEstimated;

  return signalAcCurrent;
}

/**
 * Estimasi nilai rata-rata (DC) sinyal
 */
int32_t estimateDcComponent(int32_t *reg, int32_t sample)
{
  *reg += ((((int32_t)sample << 15) - *reg) >> 4);
  // *reg += ((((int32_t)sample << 15) - *reg) >> 10);
  return (*reg >> 15);
}

/**
 * Filter FIR untuk menghaluskan sinyal
 */
int32_t lowPassFirFilter(int32_t inputAc)
{
  firBuffer[firIndex] = inputAc;

  int32_t result = multiply32(FIR_COEFFS[11], firBuffer[(firIndex - 11) & 0x1F]);

  for (uint8_t i = 0; i < 11; i++)
  {
    result += multiply32(FIR_COEFFS[i], firBuffer[(firIndex - i) & 0x1F] + firBuffer[(firIndex - 22 + i) & 0x1F]);
  }

  firIndex++;
  firIndex %= 32;

  return (result >> 11);
  // return (result >> 15);
}

/**
 * Perkalian 32-bit yang aman
 */
int32_t multiply32(int32_t x, int32_t y)
{
  return (x * y);
}
