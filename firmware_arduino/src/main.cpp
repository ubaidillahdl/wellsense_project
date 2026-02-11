#include <Arduino.h>
#include <Wire.h>

#include "Functions.h"
#include "Global.h"

// --- Inisialisasi Objek ---
MAX30105 particleSensor;
SoftwareSerial sim800(rxSim, txSim);
SSD1306AsciiWire oled;

// --- Variabel Global & State ---
DataSensor wadah;
DesimasiState desimRed, desimIR;
LpfState filterRed, filterIR;
HasilVitals dataVitals;
State currentState = ST_PERTAMA;

bool dataReady = false;
uint8_t bufferIdx = 0;
int32_t rawRed, filteredRed, rawIR, filteredIR;
unsigned long waktuMulai = 0;
bool butuhRetryCepat = false;
bool layarSudahBersih = false;

void setup() {
      initSistem();  // Serial, I2C, Pin mode
      initDisp();
      initSensorMAX();  // Konfigurasi register MAX30105
}

void loop() {
      switch (currentState) {
            case ST_PERTAMA: {
                  delay(50);
                  tampilVitals();
                  waktuMulai = millis();
                  currentState = ST_STANDBY;
            } break;

            case ST_STANDBY: {
                  unsigned long durasiTunggu = butuhRetryCepat ? 200 : 6000;

                  if (!adaTangan()) {
                        prosesStandby();
                        break;
                  };

                  if (millis() - waktuMulai >= durasiTunggu) {
                        bangunSesi();  // Inisialisasi sensor
                        bufferIdx = 0;
                        butuhRetryCepat = false;
                        layarSudahBersih = false;
                        waktuMulai = millis();
                        currentState = ST_SAMPLING;
                  }
            } break;

            case ST_SAMPLING: {
                  if (!adaTangan()) {
                        detachInterrupt(digitalPinToInterrupt(interruptPin));
                        waktuMulai = millis();
                        currentState = ST_STANDBY;
                        break;
                  }

                  if (dataReady) prosesSampling();
                  if (bufferIdx >= PANJANG_BUFFER) {
                        detachInterrupt(digitalPinToInterrupt(interruptPin));
                        waktuMulai = millis();
                        currentState = ST_KIRIM_DATA;
                  }
            } break;

            case ST_KIRIM_DATA: {
                  if (prosesKirimData()) {
                        waktuMulai = millis();
                        currentState = ST_DISPLAY;
                        break;
                  }
                  waktuMulai = millis();
                  currentState = ST_STANDBY;
            } break;

            case ST_DISPLAY: {
                  if (!butuhRetryCepat) {
                        tampilVitals();
                  }
                  waktuMulai = millis();
                  currentState = ST_STANDBY;
            } break;
      }
}