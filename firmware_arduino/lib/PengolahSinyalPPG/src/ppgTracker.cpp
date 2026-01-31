#include "ppgTracker.h"

static PpgState state = MENCARI_V1;
static float localMax = -10000, localMin = 10000;
static long sampleCounter = 0;

static long v1IdxTemp = 0;
static long pIdxTemp = 0;
static long v2IdxTemp = 0;

void initPpgTracker() {
  state = MENCARI_V1;
  sampleCounter = 0;
}

PpgResult updateStateMachine(float val) {
  static float prevVal = 0;
  PpgResult res = {false, 0, 0, 0, 0.0f, 0.0f, 0.0f, state};

  sampleCounter++;

  bool crossedUp = (prevVal <= 0 && val > 0);
  bool crossedDown = (prevVal >= 0 && val < 0);

  switch (state) {
    // Cari V1
  case MENCARI_V1:
    if (val < 0 && val < localMin) {
      localMin = val;
      v1IdxTemp = sampleCounter;
    }

    if (crossedUp) {
      res.v1Val = localMin;
      res.v1Idx = v1IdxTemp;

      localMin = 10000;
      state = MENCARI_P;
    }
    break;

    //   Cari P
  case MENCARI_P:
    if (val > 0 && val > localMax) {
      localMax = val;
      pIdxTemp = sampleCounter;
    }

    if (crossedDown) {
      res.pVal = localMax;
      res.pIdx = pIdxTemp;

      localMax = -10000;
      state = MENCARI_V2;
    }
    break;

  // Cari V2
  case MENCARI_V2:
    if (val < 0 && val < localMin) {
      localMin = val;
      v2IdxTemp = sampleCounter;
    }

    if (crossedUp) {
      res.v2Val = localMin;
      res.v2Idx = v2IdxTemp;
      res.isCycleValid = true;

      res.v1Val = localMin;
      res.v1Idx = v2IdxTemp;

      v1IdxTemp = v2IdxTemp;
      localMin = 10000;
      localMax = -10000;
      state = MENCARI_P;
    }
    break;
  }

  prevVal = val;
  return res;
}