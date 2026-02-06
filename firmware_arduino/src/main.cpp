#include "SoftwareSerial.h"
#include <MAX30105.h>
#include <Wire.h>

#define PANJANG_BUFFER 150 // 3 detik data (50Hz)
#define SERVER_IP "192.168.0.105"
#define SERVER_PORT "5005"

MAX30105 particleSensor;
SoftwareSerial sim800(4, 5); // RX, TX

const byte interruptPin = 3;
volatile bool dataReady = false;

// --- STRUKTUR DATA & STATE ---

struct DataSensor {
  int16_t bufferIR[PANJANG_BUFFER];
  int16_t bufferRed[PANJANG_BUFFER];
} wadah;

// Penurunan sample rate (400Hz -> 50Hz)
struct DesimasiState {
  int32_t s1Sum = 0, s2Sum = 0, s3Sum = 0;
  uint8_t s1Count = 0, s2Count = 0, s3Count = 0;
};

// State Filter LPF 8Hz (Menghilangkan noise frekuensi tinggi)
struct LpfState {
  int32_t x1 = 0; // History input (n-1)
  int32_t y1 = 0; // History output (n-1)
};

// Deklarasi Global
DesimasiState desimRed, desimIR;
LpfState filterRed;
LpfState filterIR;

uint8_t bufferIdx = 0;
int32_t rawRed, filteredRed, rawIR, filteredIR;
unsigned long waktuMulai = 0;
bool sedangIstirahat = false;

// --- PROTOTYPE ---
bool updateDesimasi(DesimasiState &d, int32_t inputVal, int16_t &outputVal);
bool hubungkanKePython();
int sisaRAM();
void handleInterrupt() { dataReady = true; }

void setup() {
  Serial.begin(115200);
  sim800.begin(115200);
  sim800.println("ATE0"); // Matikan echo modul agar respon bersih

  pinMode(interruptPin, INPUT_PULLUP);
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println(F("Sensor tidak ditemukan"));
    while (1)
      ;
  }

  // Config: LED Power 255, 400Hz Sampling, Pulse Width 411
  particleSensor.setup(255, 1, 2, 400, 411, 16384);
  particleSensor.enableDATARDY();
  particleSensor.getINT1();
  attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt,
                  FALLING);

  Serial.print(F(">>> RAM Awal Setup \t: "));
  Serial.print(sisaRAM());
  Serial.println(F(" byte"));
}

void loop() {
  // --- 1. PENGAMBILAN DATA (400Hz) ---
  if (!sedangIstirahat && dataReady) {
    dataReady = false;
    particleSensor.check();

    while (particleSensor.available()) {
      rawRed = particleSensor.getFIFORed();
      rawIR = particleSensor.getFIFOIR();

      // Digital Filter LPF (Integer Math agar ringan)
      filteredRed =
          (8815 * filterRed.y1 + 592 * rawRed + 592 * filterRed.x1) / 10000;
      filteredIR =
          (8815 * filterIR.y1 + 592 * rawIR + 592 * filterIR.x1) / 10000;

      // Update Filter History
      filterRed.x1 = rawRed;
      filterRed.y1 = filteredRed;
      filterIR.x1 = rawIR;
      filterIR.y1 = filteredIR;

      // Desimasi: Turunkan ke 50Hz agar buffer cukup 3 detik
      int16_t outRed, outIR;
      updateDesimasi(desimIR, filteredIR, outIR);

      if (updateDesimasi(desimRed, filteredRed, outRed)) {
        if (bufferIdx < PANJANG_BUFFER) {
          wadah.bufferRed[bufferIdx] = outRed;
          wadah.bufferIR[bufferIdx] = outIR;
          bufferIdx++;
        }
      }
      particleSensor.nextSample();
    }
    particleSensor.getINT1();
  }

  // --- 2. PENGIRIMAN DATA (Saat Buffer Penuh) ---
  if (bufferIdx >= PANJANG_BUFFER) {
    detachInterrupt(digitalPinToInterrupt(interruptPin));

    if (hubungkanKePython()) {
      // Hitung panjang karakter manual untuk AT+CIPSEND
      long totalChar = 0;
      for (uint8_t i = 0; i < PANJANG_BUFFER; i++) {
        char buf[20];
        totalChar +=
            sprintf(buf, "%u:%u,", wadah.bufferIR[i], wadah.bufferRed[i]);
      }
      totalChar += 1; // Untuk karakter '\n' di akhir

      sim800.print("AT+CIPSEND=");
      sim800.println(totalChar);
      delay(200); // Jeda tunggu prompt '>'

      // Kirim data ke SIM800 & Serial Monitor (Debug)
      for (uint8_t i = 0; i < PANJANG_BUFFER; i++) {
        sim800.print(wadah.bufferIR[i]);
        sim800.print(F(":"));
        sim800.print(wadah.bufferRed[i]);
        sim800.print(F(","));

        // Serial.print(F("M:"));
        // Serial.print(wadah.bufferRed[i]);
        // Serial.print(F("\tI:"));
        // Serial.println(wadah.bufferIR[i]);
      }
      Serial.println(F(">>> DATA Terkirim ke Server !"));

      sim800.print(F("\n"));

      // --- 3. TUNGGU FEEDBACK PYTHON (Data Vital / Heartbeat) ---
      unsigned long waitReply = millis();
      bool dataReceived = false;

      while (millis() - waitReply < 5000) {
        if (sim800.available()) {
          char c = sim800.read();
          if (c == '*') { // Paket Data Terdeteksi
            String vitals = sim800.readStringUntil('#');
            Serial.print(F(">>> Feedback dari Server : "));
            Serial.println(vitals);

            dataReceived = true;
            break;
          } else if (c == 'H') { // Heartbeat Terdeteksi
            Serial.println(F(">>> Tidak ada data dari Server !"));
            dataReceived = true;
            break;
          }
        }
      }
      if (!dataReceived)
        Serial.println(F(">>> Gagal mendapat respon dari Server !"));

      delay(500);
      sim800.println(F("AT+CIPCLOSE")); // Tutup koneksi agar server Python bisa
                                        // proses handle()
    }
    bufferIdx = 0;
    sedangIstirahat = true;
    waktuMulai = millis();
  }

  // --- 4. COOLDOWN (3 Detik) ---
  if (sedangIstirahat && (millis() - waktuMulai >= 3000)) {
    sedangIstirahat = false;

    // Flushing: Buang data sisa/basi selama masa istirahat agar buffer sensor
    // fresh
    particleSensor.check();
    while (particleSensor.available())
      particleSensor.nextSample();
    particleSensor.getINT1();

    attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt,
                    FALLING);
  }
}

// Fungsi Desimasi: Menurunkan frekuensi sampling dengan rata-rata bertahap
bool updateDesimasi(DesimasiState &d, int32_t inputVal, int16_t &outputVal) {
  d.s1Sum += inputVal;
  d.s1Count++;

  if (d.s1Count >= 2) { // Stage 1: 400Hz -> 200Hz
    d.s2Sum += d.s1Sum / 2;
    d.s2Count++;
    d.s1Sum = 0;
    d.s1Count = 0;

    if (d.s2Count >= 2) { // Stage 2: 200Hz -> 100Hz
      d.s3Sum += d.s2Sum / 2;
      d.s3Count++;
      d.s2Sum = 0;
      d.s2Count = 0;

      if (d.s3Count >= 2) { // Stage 3: 100Hz -> 50Hz
        outputVal = (int16_t)(d.s3Sum / 2);
        d.s3Sum = 0;
        d.s3Count = 0;
        return true; // Data 50Hz siap
      }
    }
  }
  return false;
}

int sisaRAM() {
  extern int __heap_start, *__brkval;
  int v;
  return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}

bool hubungkanKePython() {
  Serial.print(F("\n>>> RAM Sebelum Buffer Aktif\t: "));
  Serial.print(sisaRAM());
  Serial.println(F(" byte"));

  while (sim800.available())
    sim800.read(); // Kuras buffer serial
  sim800.print(F("AT+CIPSTART=\"TCP\",\""));
  sim800.print(SERVER_IP);
  sim800.print(F("\","));
  sim800.println(SERVER_PORT);

  unsigned long timeout = millis();
  while (millis() - timeout < 3000) {
    if (sim800.available()) {
      char tempBuffer[50];
      memset(tempBuffer, 0, sizeof(tempBuffer));

      // Titik Pantau 3: Saat buffer aktif
      Serial.print(F(">>> RAM Setelah Buffer Aktif\t: "));
      Serial.print(sisaRAM());
      Serial.println(F(" byte"));

      int len = sim800.readBytes(tempBuffer, sizeof(tempBuffer) - 1);
      tempBuffer[len] = '\0';

      if (strstr(tempBuffer, "OK") || strstr(tempBuffer, "CONNECT") ||
          strstr(tempBuffer, "ALREADY CONNECTED")) {
        Serial.println(F("\n>>> SUKSES Terhubung ke Server !"));
        return true;
      }
    }
  }
  Serial.println(F("\n>>> GAGAL Terhubung ke Server !"));
  return false;
}
