#include "MAX30105.h"
#include <PengolahSinyalPPG.h>
#include <Wire.h>


MAX30105 particleSensor;
ChebyFilter filterM;
ChebyFilter filterI;

float bufferM[200];
float bufferI[200];
uint8_t bufferIdx = 0;

void setup() {
  chebyInit(&filterM);
  chebyInit(&filterI);

  Serial.begin(2000000);

  // Inisialisasi Sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println(F("Sensor tidak ditemukan"));
    while (1)
      ;
  }

  // --- SETTING SENSOR (Sangat Mempengaruhi Kualitas Sinyal) ---
  particleSensor.setup(255, 1, 2, 200, 411, 16384);
}

void loop() {
  particleSensor.check();

  while (particleSensor.available()) {
    bufferM[bufferIdx] = chebyProcess(&filterI, particleSensor.getFIFOIR());
    bufferI[bufferIdx] = chebyProcess(&filterM, particleSensor.getFIFORed());
    particleSensor.nextSample();

    bufferIdx++;
  }

  if (bufferIdx >= 200) {
    for (uint8_t i = 0; i < 200; i++) {
      Serial.print("M:");
      Serial.print(bufferM[i]);
      Serial.print("\tI:");
      Serial.println(bufferM[i]);
    }
    bufferIdx = 0;
  }
}