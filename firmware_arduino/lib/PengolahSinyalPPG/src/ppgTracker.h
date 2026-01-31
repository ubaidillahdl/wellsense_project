#ifndef PPG_TRACKER_H
#define PPG_TRACKER_H

#include <Arduino.h>

enum PpgState { MENCARI_V1, MENCARI_P, MENCARI_V2 };

struct PpgResult {
  bool isCycleValid;
  long v1Idx, pIdx, v2Idx;
  float v1Val, pVal, v2Val; // Ini akan berisi 0 jika tidak ada event
  PpgState currentState;
};

void initPpgTracker();
PpgResult updateStateMachine(float val);

#endif