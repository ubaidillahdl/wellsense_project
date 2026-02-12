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
SystemState systemState = SYS_IDLE;
ScreenState screenState = SCR_STANDBY;

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
      switch (systemState) {
            case SYS_IDLE: {
                  unsigned long durasiTunggu = butuhRetryCepat ? 200 : 6000;
                  if (adaTangan()) {
                        if (millis() - waktuMulai >= durasiTunggu) {
                              bangunSesi();
                              bufferIdx = 0;
                              butuhRetryCepat = false;
                              waktuMulai = millis();
                              systemState = SYS_SAMPLING;
                              screenState = SCR_MEASURING;
                        }
                  } else {
                        prosesStandby();
                  }
            } break;

            case SYS_SAMPLING: {
                  if (adaTangan()) {
                        if (dataReady) prosesSampling();
                        if (bufferIdx >= PANJANG_BUFFER) {
                              detachInterrupt(digitalPinToInterrupt(interruptPin));
                              waktuMulai = millis();
                              systemState = SYS_SENDING;
                              screenState = SCR_SENDING;
                        }
                  } else {
                        detachInterrupt(digitalPinToInterrupt(interruptPin));
                        waktuMulai = millis();
                        systemState = SYS_IDLE;
                        screenState = SCR_STANDBY;
                        break;
                  }
            } break;

            case SYS_SENDING: {
                  if (prosesKirimData()) {
                        waktuMulai = millis();
                        systemState = SYS_IDLE;
                        screenState = SCR_RESULT;
                        break;
                  }
                  waktuMulai = millis();
                  butuhRetryCepat = true;
                  systemState = SYS_IDLE;
                  screenState = SCR_STANDBY;
            } break;
      }
}