#include "detektorSiklus.h"
#include "PengolahSinyalPPG.h"
#include "cariLembah.h"
#include "cariPuncak.h"
#include "puncakValid.h"

void detektorSiklus(float *sinyalBuffer, uint8_t panjangBuffer,

                    uint8_t *outValidPeaks, uint8_t *outVBefore,
                    uint8_t *outVAfter, uint8_t *outNumValid) {
  // 1. Deklarasi variabel lokal/sementara untuk menampung hasil mentah
  float outPeakValues[5];
  uint8_t outPeakLocs[5];
  uint8_t outNumPeaks;

  float outValleyValues[5];
  uint8_t outValleyLocs[5];
  uint8_t outNumValleys;

  float minPromP = 0.7 * cariNilaiMax(sinyalBuffer, panjangBuffer);
  float minPromL = 0.5 * cariNilaiMax(sinyalBuffer, panjangBuffer);
  uint8_t minDist = 0.4 * panjangBuffer;

  cariPuncak(sinyalBuffer, panjangBuffer, minPromP, minDist, outPeakValues,
             outPeakLocs, &outNumPeaks);
  cariLembah(sinyalBuffer, panjangBuffer, minPromL, minDist, outValleyValues,
             outValleyLocs, &outNumValleys);

  puncakValid(outPeakLocs, outNumPeaks, outValleyLocs, outNumValleys,
              outValidPeaks, outVBefore, outVAfter, outNumValid);
}