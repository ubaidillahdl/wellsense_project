#include "MAX30105.h"
#include <Wire.h>


MAX30105 particleSensor;

const byte interruptPin = 3;
volatile bool dataReady = false;

float rawM;
float rawI;

void handleInterrupt() { dataReady = true; }

void setup() {
  Serial.begin(2000000);
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