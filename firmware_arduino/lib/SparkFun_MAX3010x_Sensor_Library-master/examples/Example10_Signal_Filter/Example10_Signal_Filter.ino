#include <Wire.h>
#include "MAX30105.h"
#include "signalFilter.h"

MAX30105 particleSensor;

void setup()
{
  Serial.begin(9600);
  Serial.println("MAX30105 Basic Readings Example");

  // Initialize sensor
  if (particleSensor.begin() == false)
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }

  particleSensor.setup(); //Configure sensor. Use 6.4mA for LED drive
}

void loop()
{
  int filteredIR = applySignalFilter(particleSensor.getIR());
  int filteredRed = applySignalFilter(particleSensor.getRed());

  Serial.print(filteredIR);
  Serial.print('\t');
  Serial.println(filteredRed);
}
