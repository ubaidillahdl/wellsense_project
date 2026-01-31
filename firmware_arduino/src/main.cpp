#include "MAX30105.h"
#include <PengolahSinyalPPG.h>
#include <Wire.h>

#define BUFFER_LENGTH 200

MAX30105 particleSensor;

const byte interruptPin = 3;
volatile bool dataReady = false;

uint32_t bufferM[BUFFER_LENGTH];
uint32_t bufferI[BUFFER_LENGTH];
uint8_t bufferIdx = 0;

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
  particleSensor.setup(255, 1, 2, 200, 411, 16384);

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
      bufferM[bufferIdx] = particleSensor.getFIFOIR();
      bufferI[bufferIdx] = particleSensor.getFIFORed();
      particleSensor.nextSample();

      bufferIdx++;
    }

    if (bufferIdx >= BUFFER_LENGTH) {
      for (uint8_t i = 0; i < BUFFER_LENGTH; i++) {
        Serial.print("M:");
        Serial.print(bufferM[i]);
        Serial.print("\tI:");
        Serial.println(bufferM[i]);
      }
      bufferIdx = 0;
    }
    particleSensor.getINT1();
  }
}