#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "Global.h"

// --- System.cpp: Manajemen Hardware & Memori ---
void handleInterrupt();
int sisaRAM();
void initSistem();

// --- Sensor.cpp: Logika MAX30105 & Signal Processing ---
bool adaTangan();
bool updateDesimasi(DesimasiState &d, int32_t inputVal, int16_t &outputVal);
void initSensorMAX();
void prosesStandby();
void prosesSampling();
void bangunSesi();

// --- Network.cpp: Komunikasi SIM800C ---
bool hubungkanKePython();
bool prosesKirimData();
void pecahDataFeedback(String raw);

#endif