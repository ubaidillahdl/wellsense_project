#include "MAX30105.h"
#include <PengolahSinyalPPG.h>
#include <Wire.h>


MAX30105 particleSensor;
ChebyFilter filterM;
ChebyFilter filterI;

#define BUFFER_SIZE 200

const byte interruptPin = 3;
volatile bool dataReady = false;

// float bufferM[BUFFER_SIZE];
float bufferI[BUFFER_SIZE];
uint32_t bufferIdx = 0;

// --- PENYIMPANAN HASIL DETEKSI ---
uint8_t outPeakLocs[5]; // Lokasi (indeks) puncak ditemukan
float outPeakValues[5]; // Nilai (amplitudo) puncak ditemukan
uint8_t outNumPeaks;    // Jumlah puncak yang berhasil dideteksi

uint8_t outValleyLocs[5]; // Lokasi (indeks) lembah ditemukan
float outValleyValues[5]; // Nilai (amplitudo) lembah ditemukan
uint8_t outNumValleys;    // Jumlah lembah yang berhasil dideteksi

void handleInterrupt() { dataReady = true; }

void setup() {
  // chebyInit(&filterM);
  chebyInit(&filterI);

  Serial.begin(2000000);
  pinMode(interruptPin, INPUT_PULLUP);

  // Inisialisasi Sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println(F("Sensor tidak ditemukan"));
    while (1)
      ;
  }

  // --- SETTING SENSOR (Sangat Mempengaruhi Kualitas Sinyal) ---
  particleSensor.setup(255, 1, 2, BUFFER_SIZE, 411, 16384);

  particleSensor.enableDATARDY();
  particleSensor.getINT1();
  attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt,
                  FALLING);
}

void loop() {
  if (dataReady) {
    dataReady = false;
    particleSensor.check();
    while (particleSensor.available()) {
      if (bufferIdx < BUFFER_SIZE)
        bufferI[bufferIdx] =
            chebyProcess(&filterM, particleSensor.getFIFORed());
      particleSensor.nextSample();
      bufferIdx++;
    }
  }
  particleSensor.getINT1();

  if (bufferIdx >= BUFFER_SIZE) {
    float maxVal = cariNilaiMax(bufferI, BUFFER_SIZE);
    float minPromP = 0.7 * maxVal;
    float minPromL = 0.5 * maxVal;
    uint8_t minDist = 0.4 * BUFFER_SIZE;

    cariPuncak(bufferI, BUFFER_SIZE, minPromP, minDist, outPeakValues,
               outPeakLocs, &outNumPeaks);
    cariLembah(bufferI, BUFFER_SIZE, minPromL, minDist, outValleyValues,
               outValleyLocs, &outNumValleys);

    for (uint8_t i = 0; i < BUFFER_SIZE; i++) {
      float pVal = 0; // Default 0 jika bukan puncak
      float lVal = 0; // Default 0 jika bukan lembah

      for (uint8_t j = 0; j < outNumPeaks; j++) {
        if (i == outPeakLocs[j])
          pVal = bufferI[i];
      }
      for (uint8_t j = 0; j < outNumValleys; j++) {
        if (i == outValleyLocs[j])
          lVal = bufferI[i];
      }

      // Serial.print("M:");
      // Serial.print(bufferM[i]);
      Serial.print("I:");
      Serial.print(bufferI[i]);
      Serial.print("\t");
      Serial.print(pVal); // Kolom 2: Marker Puncak
      Serial.print("\t");
      Serial.println(lVal); // Kolom 3: Marker Lembah
    }
    bufferIdx = 0;
    particleSensor.check();
  }
}