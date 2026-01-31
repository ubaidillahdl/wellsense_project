#include "MAX30105.h"
#include <Wire.h>


MAX30105 particleSensor;

const byte interruptPin = 3;
volatile bool dataReady = false;

// Gunakan int32_t untuk menyimpan data

int64_t xRawRed;
int64_t xRawIR;

int64_t xRed, yRed;
int64_t xIR, yIR;

int64_t xRed1 = 0, yRed1 = 0;
int64_t xIR1 = 0, yIR1 = 0;

void handleInterrupt() { dataReady = true; }

void setup() {
  Serial.begin(115200);
  pinMode(interruptPin, INPUT_PULLUP);

  // Inisialisasi Sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println(F("Sensor tidak ditemukan"));
    while (1)
      ;
  }

  // --- SETTING SENSOR (Sangat Mempengaruhi Kualitas Sinyal) ---
  particleSensor.setup(255, 1, 2, 400, 411, 16384);

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
      xRed = particleSensor.getFIFORed();
      xIR = particleSensor.getFIFOIR();

      xRawRed = xRed;
      xRawIR = xIR;

      // Gunakan casting (int64_t) untuk mencegah overflow saat perkalian
      // Lalu bagi 10000 untuk kembali ke skala normal
      yRed = (8815 * yRed1 + 592 * xRed + 592 * xRed1) / 10000;
      yIR = (8815 * yIR1 + 592 * xIR + 592 * xIR1) / 10000;

      // Update history
      xRed1 = xRed;
      yRed1 = yRed;
      xIR1 = xIR;
      yIR1 = yIR;

      particleSensor.nextSample();
    }

    particleSensor.getINT1();

    Serial.print("M:");
    Serial.print(long(xRawRed));
    Serial.print("\tI:");
    Serial.println(long(xRawIR));
  }
}