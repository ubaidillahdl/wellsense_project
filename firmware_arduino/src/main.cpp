#include "MAX30105.h"
#include <PengolahSinyalPPG.h>
#include <Wire.h>


MAX30105 particleSensor;
ChebyFilter chebyFilter;

void setup() {
  chebyInit(&chebyFilter);
  Serial.begin(115200);

  // Inisialisasi Sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println(F("Sensor tidak ditemukan"));
    while (1)
      ;
  }

  // --- SETTING SENSOR (Sangat Mempengaruhi Kualitas Sinyal) ---
  particleSensor.setup(255, 1, 2, 200, 411, 16384);
  initPpgTracker(); // Inisialisasi tracker
}

void loop() {
  while (particleSensor.available() == false)
    particleSensor.check();

  float filteredVal = chebyProcess(&chebyFilter, particleSensor.getRed());
  PpgResult result = updateStateMachine(filteredVal);

  Serial.print("RT:");
  Serial.print(filteredVal, 4);
  Serial.print("\t\t");
  Serial.print(result.v1Val);
  Serial.print("\t");
  Serial.print(result.pVal);
  Serial.print("\t");
  Serial.print(result.v2Val);

  Serial.println();
  particleSensor.nextSample();
}