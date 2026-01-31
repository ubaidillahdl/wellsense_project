#include "MAX30105.h"
#include <PengolahSinyalPPG.h>
#include <Wire.h>


MAX30105 particleSensor;
ChebyFilter filterM;
ChebyFilter filterI;

const byte interruptPin = 3;
volatile bool dataReady = false;

int16_t rawM;
int16_t rawI;

void handleInterrupt() { dataReady = true; }

void setup() {
  chebyInit(&filterM);
  chebyInit(&filterI);

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
      rawM = particleSensor.getFIFOIR();
      rawI = particleSensor.getFIFORed();
      particleSensor.nextSample();
    }

    particleSensor.getINT1();

    Serial.print("M:");
    Serial.print(rawM);
    Serial.print("\tI:");
    Serial.println(rawI);
  }
}