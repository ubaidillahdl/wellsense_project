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
ScreenState screenState = SCR_READY;
ScreenState lastScreen = SCR_READY;

bool dataReady = false;
uint8_t bufferIdx = 0;
int32_t rawRed, filteredRed, rawIR, filteredIR;
unsigned long waktuMulai = 0;
bool butuhRetryCepat = false;
bool dataUpdate = false;
int8_t res = 0;

void setup() {
      initSistem();  // Serial, I2C, Pin mode
      initDisp();
      initSensorMAX();  // Konfigurasi register MAX30105
}

void loop() {
      switch (systemState) {
            case SYS_IDLE: {
                  // Jika habis error, jeda cuma 0.2 dtk. Jika sukses/normal, jeda 6 dtk.
                  unsigned long durasiTunggu = butuhRetryCepat ? 200 : 6000;
                  if (adaTangan()) {
                        if (millis() - waktuMulai >= durasiTunggu) {
                              bangunSesi();   // Siapkan sensor/variabel
                              bufferIdx = 0;  // Reset penampung data
                              butuhRetryCepat = false;
                              waktuMulai = millis();
                              systemState = SYS_SAMPLING;
                              screenState = SCR_SAMPLING;
                        }
                  } else {
                        prosesStandby();  // Animasi atau hemat daya
                  }
            } break;

            case SYS_SAMPLING: {
                  if (adaTangan()) {
                        if (dataReady) prosesSampling();  // Ambil data dari sensor

                        // Jika data sudah penuh (sesuai panjang buffer)
                        if (bufferIdx >= PANJANG_BUFFER) {
                              detachInterrupt(digitalPinToInterrupt(interruptPin));
                              waktuMulai = millis();
                              systemState = SYS_SENDING;
                              screenState = SCR_UPLOADING;
                        }
                  } else {
                        // Tangan lepas saat sampling -> Batal, balik ke IDLE
                        detachInterrupt(digitalPinToInterrupt(interruptPin));
                        waktuMulai = millis();
                        systemState = SYS_IDLE;
                        screenState = SCR_READY;
                        break;
                  }
            } break;

            case SYS_SENDING: {
                  res = prosesKirimData();  // Kirim ke SIM800C -> Python

                  waktuMulai = millis();
                  systemState = SYS_IDLE;  // Selesai kirim pasti balik ke IDLE

                  switch (res) {
                        case 1:  // SUKSES
                              if (!butuhRetryCepat) {
                                    dataUpdate = true;
                                    screenState = SCR_FINISHED;
                              }
                              break;
                        case 0:  // OFFLINE (Masalah Sinyal/Internet)
                              butuhRetryCepat = true;
                              screenState = SCR_NET_ERR;
                              break;
                        default:  // ERROR SERVER (Refused/Timeout/Unsteady)
                              butuhRetryCepat = true;
                              screenState = SCR_SRV_ERR;
                              break;
                  }
            } break;
      }

      updateTampilan();  // Refresh OLED berdasarkan screenState & res
}