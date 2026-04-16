#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "Global.h"

// --- System.cpp: Manajemen Hardware & Memori ---
void handleInterrupt();
void initSistem();
void vibrate();

// --- Sensor.cpp: Logika MAX30105 & Signal Processing ---
bool adaTangan();
bool updateDesimasi(DesimasiState &d, int32_t inputVal, int16_t &outputVal);
void initSensorMAX();
void prosesStandby();
void prosesSampling();
void bangunSesi();
void normalisasiBuffer();

// --- Network.cpp: Komunikasi SIM800C ---
bool cekInternet();
void hubungkanKeInternet();
bool hubungkanKePython();
int8_t prosesKirimData();
bool pecahDataFeedback(char *buf);
char *resAT(const char *command, uint16_t timeout, char *buf, uint8_t bufSize, bool earlyExit = false);

// --- Display.cpp
void initDisp();
void tampilVitals();
void updateTampilan();

#endif