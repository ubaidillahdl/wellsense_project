#include <Wire.h>
#include "MAX30105.h"
#include "findPeaks.h"
#include "signalFilter.h"

MAX30105 particleSensor;

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
uint16_t irBuffer[100];  // Buffer 16-bit (Uno/Nano)
#else
uint32_t irBuffer[100];  // Buffer 32-bit (ESP32/STM32)
#endif

int32_t bufferLength, num_valleys;
int32_t valley_locs[15];

void setup() {
  Serial.begin(115200);

  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println(F("Sensor tidak ditemukan"));
    while (1)
      ;
  }

  byte ledBrightness = 0x7F;  //Options: 0=Off to 255=50mA
  byte sampleAverage = 4;     //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2;           //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  byte sampleRate = 100;      //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411;       //Options: 69, 118, 215, 411
  int adcRange = 4096;        //Options: 2048, 4096, 8192, 16384

  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);  //Configure sensor with these settings
}

void loop() {
  bufferLength = 100;

  // Isi buffer awal (100 sampel)
  Serial.print("Ambil sampel awal");

  for (byte i = 0; i < bufferLength; i++) {
    while (particleSensor.available() == false) particleSensor.check();
    irBuffer[i] = applySignalFilter(particleSensor.getIR());
    particleSensor.nextSample();

    // Setiap 5 sampel, tambahkan satu titik agar tidak terlalu panjang
    if (i % 15 == 0) {
      Serial.print(".");
    }
  }

  // Cari puncak pertama
  findPeaks(irBuffer, bufferLength, valley_locs, &num_valleys);

  while (1) {
    // Geser buffer ke kiri (buang 25 data lama)
    for (byte i = 25; i < 100; i++) {
      irBuffer[i - 25] = irBuffer[i];
    }

    // Ambil 25 data baru
    for (byte i = 75; i < 100; i++) {
      while (particleSensor.available() == false) particleSensor.check();  //Check the sensor for new data
      irBuffer[i] = applySignalFilter(particleSensor.getIR());
      particleSensor.nextSample();  //We're finished with this sample so move to next sample
    }

    if (num_valleys >= 2) {
      for (byte i = 0; i < bufferLength; i++) {
        Serial.println(irBuffer[i]);
      }
      // Tambahkan jeda setelah kirim 1 buffer agar mata sempat melihat
      delay(500);
    }

    // Update deteksi puncak
    findPeaks(irBuffer, bufferLength, valley_locs, &num_valleys);
  }
}
