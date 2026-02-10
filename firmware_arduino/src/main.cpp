#include <Arduino.h>
#include <Wire.h>

#include "Functions.h"
#include "Global.h"

// --- Inisialisasi Objek ---
MAX30105 particleSensor;
SoftwareSerial sim800(rxSim, txSim);

// --- Variabel Global & State ---
DataSensor wadah;
DesimasiState desimRed, desimIR;
LpfState filterRed, filterIR;
HasilVitals dataVitals;
State currentState = ST_STANDBY;

bool dataReady = false;
uint8_t bufferIdx = 0;
int32_t rawRed, filteredRed, rawIR, filteredIR;
unsigned long waktuMulai = 0;
bool butuhRetryCepat = false;

void setup() {
      initSistem();     // Serial, I2C, Pin mode
      initSensorMAX();  // Konfigurasi register MAX30105
}

void loop() {
      switch (currentState) {
            case ST_STANDBY: {
                  // Tentukan cooldown: 0.2 detik jika gagal, 3 detik jika normal
                  unsigned long durasiTunggu = butuhRetryCepat ? 200 : 6000;

                  if (adaTangan()) {
                        // Cek apakah sudah melewati masa tunggu (cooldown)
                        if (millis() - waktuMulai >= durasiTunggu) {
                              bangunSesi();
                              butuhRetryCepat = false;
                              waktuMulai = millis();
                              bufferIdx = 0;
                              currentState = ST_SAMPLING;  // Mulai ambil data
                        }
                  } else {
                        prosesStandby();  // LED kedip indikator standby
                  }
            } break;

            case ST_SAMPLING: {
                  // Keamanan: Jika jari lepas, batalkan dan balik ke standby
                  if (!adaTangan()) {
                        detachInterrupt(digitalPinToInterrupt(interruptPin));
                        waktuMulai = millis();
                        currentState = ST_STANDBY;
                        break;
                  }

                  // Ambil data jika interupsi sensor menyala (flag dari ISR)
                  if (dataReady) prosesSampling();

                  // Cek apakah data sudah terkumpul penuh sesuai target
                  if (bufferIdx >= PANJANG_BUFFER) {
                        detachInterrupt(digitalPinToInterrupt(interruptPin));
                        currentState = ST_KIRIM_DATA;  // Lanjut kirim ke Server
                  }
            } break;

            case ST_KIRIM_DATA: {
                  // Jalankan fungsi kirim (outputnya bool: true/false)
                  if (prosesKirimData()) {
                        // Sukses: (Nanti tambahkan logic pindah ke ST_DISPLAY_HASIL)

                        // bufferIdx = 0;
                        waktuMulai = millis();
                        currentState = ST_STANDBY;
                  } else {
                        // Gagal: Balik ke standby untuk coba lagi nanti
                        currentState = ST_STANDBY;
                  }
            } break;
      }
}