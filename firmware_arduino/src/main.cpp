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
unsigned long waktuMulaiSesi = 0;
bool sedangIstirahat = false;
bool butuhRetryCepat = false;

void setup() {
      initSistem();     // Serial, I2C, Pin mode
      initSensorMAX();  // Konfigurasi register MAX30105
}

void loop() {
      switch (currentState) {
            case ST_STANDBY: {
                  unsigned long durasiTunggu = butuhRetryCepat ? 200 : 3000;

                  // Logika: Hanya masuk sampling jika SUDAH cukup waktu DAN ADA tangan
                  if ((millis() - waktuMulai >= durasiTunggu) && adaTangan()) {
                        butuhRetryCepat = false;
                        bufferIdx = 0;
                        bangunSesi();
                        currentState = ST_SAMPLING;
                  } else {
                        // Jika salah satu syarat di atas tidak terpenuhi, tetap standby
                        prosesStandby();
                  }
            } break;

            case ST_SAMPLING: {
                  // 1. Proteksi: Kalau jari tiba-tiba lepas saat sampling
                  if (!adaTangan()) {
                        Serial.println(F("\n>>> Jari terlepas! Membatalkan sesi..."));

                        // Bersihkan keadaan sebelum balik ke Standby
                        detachInterrupt(digitalPinToInterrupt(interruptPin));
                        particleSensor.setPulseAmplitudeRed(0);  // Matikan LED Red

                        waktuMulai = millis();  // Mulai hitung cooldown 3 detik
                        currentState = ST_STANDBY;
                        break;
                  }

                  // 2. Sampling: Jika ada interupsi data dari sensor (dataReady set di ISR)
                  if (dataReady) {
                        prosesSampling();  // Di sini terjadi filter & downsampling
                  }

                  // 3. Cek Selesai: Apakah data sudah terkumpul sesuai PANJANG_BUFFER?
                  if (bufferIdx >= PANJANG_BUFFER) {
                        Serial.println(F("\n>>> Sampling selesai. Masuk ke transmisi..."));

                        // Matikan interupsi agar tidak sampling lagi saat kirim data
                        detachInterrupt(digitalPinToInterrupt(interruptPin));

                        currentState = ST_KIRIM_DATA;
                  }

            } break;

            case ST_KIRIM_DATA: {
                  // 1. Matikan semua gangguan (Interupsi sensor sudah mati di step sebelumnya)
                  Serial.println(F("\n>>> Memulai Transmisi SIM800C..."));

                  // 2. Jalankan fungsi kirim
                  // Kita buat fungsi ini mengembalikan 'true' jika server balas OK
                  if (prosesKirimData()) {
                        Serial.println(F(">>> Server: Data Diterima."));
                  }
            }
      }

      // // --- 0. CEK KEBERADAAN TANGAN (Proximity) ---
      // if (!adaTangan()) {
      //       if (!sedangIstirahat) {
      //             bufferIdx = 0;
      //             sedangIstirahat = true;
      //             detachInterrupt(digitalPinToInterrupt(interruptPin));
      //             particleSensor.setPulseAmplitudeRed(0);
      //       }
      //       prosesStandby();  // Idle mode
      //       return;
      // }

      // // --- 1. JEDA / COOLDOWN (Manajemen Sesi) ---
      // if (sedangIstirahat) {
      //       // Retry cepat jika error, atau 3 detik jika sesi baru
      //       unsigned long durasiTunggu = butuhRetryCepat ? 200 : 3000;
      //       if (millis() - waktuMulai >= durasiTunggu) {
      //             bangunSesi();  // Re-aktifkan sensor & interrupt
      //       } else {
      //             return;  // Masih dalam masa tunggu
      //       }
      // }

      // // --- 2. SAMPLING DATA (Interupsi 400Hz) ---
      // if (dataReady) {
      //       prosesSampling();  // Low Pass Filter & Downsampling ke 50Hz
      // }

      // // --- 3. TRANSMISI DATA ---
      // if (bufferIdx >= PANJANG_BUFFER) {
      //       prosesKirimData();  // Upload ke Server via SIM800C
      // }
}