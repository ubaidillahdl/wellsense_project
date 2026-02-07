#include "Functions.h"
#include "Global.h"

// --- CEK DETEKSI OBJEK ---
bool adaTangan() {
      long currentIR = particleSensor.getIR();  // Baca IR instan
      static unsigned long lastUpdate = 0;

      // Threshold deteksi tangan (berdasarkan hasil tuning)
      if (currentIR > 150000 && currentIR < 240000) {
            return true;
      } else {
            // Notifikasi ke Serial tiap 500ms
            if (millis() - lastUpdate > 500) {
                  Serial.println(F(">>> Pasang Sensor ke Pergelangan !"));
                  lastUpdate = millis();
            }
            return false;
      }
}

// --- LOGIKA DESIMASI (400Hz -> 50Hz) ---
bool updateDesimasi(DesimasiState &d, int32_t inputVal, int16_t &outputVal) {
      d.s1Sum += inputVal;
      d.s1Count++;

      if (d.s1Count >= 2) {  // Stage 1: 400Hz -> 200Hz
            d.s2Sum += d.s1Sum / 2;
            d.s2Count++;
            d.s1Sum = 0;
            d.s1Count = 0;

            if (d.s2Count >= 2) {  // Stage 2: 200Hz -> 100Hz
                  d.s3Sum += d.s2Sum / 2;
                  d.s3Count++;
                  d.s2Sum = 0;
                  d.s2Count = 0;

                  if (d.s3Count >= 2) {  // Stage 3: 100Hz -> 50Hz
                        outputVal = (int16_t)(d.s3Sum / 2);
                        d.s3Sum = 0;
                        d.s3Count = 0;
                        return true;  // Data 50Hz siap
                  }
            }
      }
      return false;
}

// --- INISIALISASI SENSOR ---
void initSensorMAX() {
      if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
            Serial.println(F("Sensor tidak ditemukan"));
            while (1)
                  ;  // Berhenti jika sensor gagal init
      }

      // Config: LED 255 (Full), 400Hz Sample, Pulse Width 411, Range 16384
      particleSensor.setup(255, 1, 2, 400, 411, 16384);
      particleSensor.enableDATARDY();
      particleSensor.getINT1();  // Clear interrupt flag
      attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, FALLING);

      particleSensor.setPulseAmplitudeRed(0);   // Red mati saat standby
      particleSensor.setPulseAmplitudeIR(255);  // IR tetap nyala untuk deteksi tangan
}

// --- INDIKATOR STANDBY (LED Kedip) ---
void prosesStandby() {
      static unsigned long lastFlash = 0;
      if (millis() - lastFlash > 500) {
            particleSensor.setPulseAmplitudeRed(20);  // Kedip redup
            delay(20);
            particleSensor.setPulseAmplitudeRed(0);
            lastFlash = millis();
      }
}

// --- PROSES PENGAMBILAN DATA ---
void prosesSampling() {
      dataReady = false;
      particleSensor.check();

      while (particleSensor.available()) {
            rawRed = particleSensor.getFIFORed();
            rawIR = particleSensor.getFIFOIR();

            // LPF Digital (Integer math): Meredam noise frekuensi tinggi
            filteredRed = (8815 * filterRed.y1 + 592 * rawRed + 592 * filterRed.x1) / 10000;
            filteredIR = (8815 * filterIR.y1 + 592 * rawIR + 592 * filterIR.x1) / 10000;

            // Simpan history filter
            filterRed.x1 = rawRed;
            filterRed.y1 = filteredRed;
            filterIR.x1 = rawIR;
            filterIR.y1 = filteredIR;

            // Turunkan rate data ke 50Hz
            int16_t outRed, outIR;
            updateDesimasi(desimIR, filteredIR, outIR);

            if (updateDesimasi(desimRed, filteredRed, outRed)) {
                  // Abaikan data 1 detik pertama agar pembacaan stabil
                  if (millis() - waktuMulaiSesi >= 1000) {
                        if (bufferIdx < PANJANG_BUFFER) {
                              wadah.bufferRed[bufferIdx] = outRed;
                              wadah.bufferIR[bufferIdx] = outIR;
                              bufferIdx++;
                        }
                  }
            }
            particleSensor.nextSample();
      }
      particleSensor.getINT1();  // Clear interrupt
}

// --- MULAI SESI BARU ---
void bangunSesi() {
      Serial.println(butuhRetryCepat ? F("\n>>> Kegagalan Terdeteksi: Mencoba ulang...") : F("\n>>> Memulai sesi pengukuran baru..."));

      if (butuhRetryCepat) butuhRetryCepat = false;

      particleSensor.setPulseAmplitudeRed(255);  // LED Red Full Power

      // Flush sisa data di FIFO sensor
      particleSensor.check();
      while (particleSensor.available()) particleSensor.nextSample();
      particleSensor.getINT1();

      attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, FALLING);

      sedangIstirahat = false;  // Masuk mode aktif
      waktuMulaiSesi = millis();
      bufferIdx = 0;  // Reset index buffer
}