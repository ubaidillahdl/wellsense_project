#include <MAX30105.h>
#include <Wire.h>

#include "SoftwareSerial.h"

#define PANJANG_BUFFER 150  // 3 detik data (50Hz)
#define SERVER_IP "192.168.0.105"
#define SERVER_PORT "5005"

#define IR_THRESHOLD_ADA_TANGAN 150000

MAX30105 particleSensor;
SoftwareSerial sim800(4, 5);  // RX, TX

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
      int32_t x1 = 0;  // History input (n-1)
      int32_t y1 = 0;  // History output (n-1)
};

// Deklarasi Global
DesimasiState desimRed, desimIR;
LpfState filterRed;
LpfState filterIR;

uint8_t bufferIdx = 0;
int32_t rawRed, filteredRed, rawIR, filteredIR;
unsigned long waktuMulai = 0;
unsigned long waktuMulaiSesi = 0;
bool sedangIstirahat = false;

// --- PROTOTYPE ---
bool updateDesimasi(DesimasiState &d, int32_t inputVal, int16_t &outputVal);
bool hubungkanKePython();
int sisaRAM();
bool adaTangan();
void handleInterrupt() { dataReady = true; }

void setup() {
      Serial.begin(115200);
      sim800.begin(115200);
      sim800.println("ATE0");  // Matikan echo modul agar respon bersih

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
      attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, FALLING);

      particleSensor.setPulseAmplitudeRed(0);   // Mematikan LED Merah
      particleSensor.setPulseAmplitudeIR(255);  // IR tetap Full Power untuk testing

      Serial.print(F(">>> RAM Awal Setup \t: "));
      Serial.print(sisaRAM());
      Serial.println(F(" byte"));
}

void loop() {
      // --- 0. GERBANG PROXIMITY (CEK TANGAN) ---
      // Jika tangan lepas, sistem langsung "tidur" dan abaikan kode di bawahnya
      if (!adaTangan()) {
            if (!sedangIstirahat) {
                  bufferIdx = 0;
                  sedangIstirahat = true;
                  detachInterrupt(digitalPinToInterrupt(interruptPin));
                  particleSensor.setPulseAmplitudeRed(0);  // Matikan LED Merah
            }

            // Efek Kedip Standby: Indikator alat hidup tapi belum dipakai
            static unsigned long lastFlash = 0;
            if (millis() - lastFlash > 250) {
                  particleSensor.setPulseAmplitudeRed(20);
                  delay(20);
                  particleSensor.setPulseAmplitudeRed(0);
                  lastFlash = millis();
            }
            return;  // KELUAR DARI LOOP: Abaikan semua proses filter & kirim data di bawah
      }

      // --- 1. PROSES BANGUN & COOLDOWN ---
      // Memberi jeda 3 detik setelah kirim data sebelum mulai ambil data lagi
      if (sedangIstirahat) {
            // Cek apakah sudah cukup waktu cooldown (misal 3 detik) agar tidak spamming
            if (millis() - waktuMulai >= 3000) {
                  particleSensor.setPulseAmplitudeRed(255);  // Nyalakan LED Full

                  // Pembersihan Buffer: Buang data sisa/sampah dari sensor
                  particleSensor.check();
                  while (particleSensor.available()) particleSensor.nextSample();
                  particleSensor.getINT1();

                  attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, FALLING);
                  sedangIstirahat = false;  // Masuk mode aktif
                  waktuMulaiSesi = millis();
                  bufferIdx = 0;
            } else {
                  return;  // Masih dalam masa tunggu (cooldown)
            }
      }

      // --- 2. PENGAMBILAN DATA (400Hz) ---
      // Hanya jalan jika ada interrupt (dataReady) dan tangan terdeteksi
      if (dataReady) {
            dataReady = false;
            particleSensor.check();

            while (particleSensor.available()) {
                  rawRed = particleSensor.getFIFORed();
                  rawIR = particleSensor.getFIFOIR();

                  // Digital Filter LPF (Integer Math agar ringan)
                  filteredRed = (8815 * filterRed.y1 + 592 * rawRed + 592 * filterRed.x1) / 10000;
                  filteredIR = (8815 * filterIR.y1 + 592 * rawIR + 592 * filterIR.x1) / 10000;

                  // Update Filter History
                  filterRed.x1 = rawRed;
                  filterRed.y1 = filteredRed;
                  filterIR.x1 = rawIR;
                  filterIR.y1 = filteredIR;

                  // Desimasi: Turunkan ke 50Hz agar buffer cukup 3 detik
                  int16_t outRed, outIR;
                  updateDesimasi(desimIR, filteredIR, outIR);

                  if (updateDesimasi(desimRed, filteredRed, outRed)) {
                        // MODIFIKASI DI SINI:
                        // Hanya simpan ke buffer jika sudah lewat 1 detik sejak sesi dimulai
                        // 1 detik = 50 data (karena frekuensi desimasi kita 50Hz)
                        if (millis() - waktuMulaiSesi >= 1000) {
                              if (bufferIdx < PANJANG_BUFFER) {
                                    wadah.bufferRed[bufferIdx] = outRed;
                                    wadah.bufferIR[bufferIdx] = outIR;
                                    bufferIdx++;
                              }
                        }
                  }
                  particleSensor.nextSample();
            }
            particleSensor.getINT1();
      }

      // --- 3. PENGIRIMAN DATA (Saat Buffer Penuh) ---
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
                  totalChar += 1;  // Untuk karakter '\n' di akhir

                  sim800.print("AT+CIPSEND=");
                  sim800.println(totalChar);
                  delay(200);  // Jeda tunggu prompt '>'

                  // Kirim data ke SIM800 & Serial Monitor (Debug)
                  for (uint8_t i = 0; i < PANJANG_BUFFER; i++) {
                        sim800.print(wadah.bufferIR[i]);
                        sim800.print(F(":"));
                        sim800.print(wadah.bufferRed[i]);
                        sim800.print(F(","));
                  }
                  Serial.println(F(">>> DATA Terkirim ke Server !"));

                  sim800.print(F("\n"));

                  // Tunggu respon balik dari Python (Feedback Vitals)
                  unsigned long waitReply = millis();
                  bool dataReceived = false;

                  while (millis() - waitReply < 5000) {
                        if (sim800.available()) {
                              char c = sim800.read();
                              if (c == '*') {  // Paket Data Terdeteksi
                                    String vitals = sim800.readStringUntil('#');
                                    Serial.print(F(">>> Feedback dari Server : "));
                                    Serial.println(vitals);

                                    dataReceived = true;
                                    break;
                              } else if (c == 'H') {  // Heartbeat Terdeteksi
                                    Serial.println(F(">>> Tidak ada data dari Server !"));
                                    dataReceived = true;
                                    break;
                              }
                        }
                  }
                  if (!dataReceived) Serial.println(F(">>> Gagal mendapat respon dari Server !"));

                  delay(500);
                  sim800.println(F("AT+CIPCLOSE"));  // Tutup koneksi agar server Python bisa proses handle()
            }

            // Masuk masa istirahat/cooldown
            bufferIdx = 0;
            sedangIstirahat = true;
            waktuMulai = millis();
            particleSensor.setPulseAmplitudeRed(0);
      }
}

// Fungsi Desimasi: Menurunkan frekuensi sampling dengan rata-rata bertahap
bool updateDesimasi(DesimasiState &d, int32_t inputVal, int16_t &outputVal) {
      d.s1Sum += inputVal;
      d.s1Count++;

      if (d.s1Count >= 2) {  // Stage 1: 400Hz -> 200Hz
            d.s2Sum += d.s1Sum / 2;
            d.s2Count++;
            d.s1Sum = 0;
            d.s1Count = 0;

            if (d.s2Count >= 2) {  // Stage 2: 200Hz -> 100Hz
                  d.s3Sum += d.s2Sum / 2;
                  d.s3Count++;
                  d.s2Sum = 0;
                  d.s2Count = 0;

                  if (d.s3Count >= 2) {  // Stage 3: 100Hz -> 50Hz
                        outputVal = (int16_t)(d.s3Sum / 2);
                        d.s3Sum = 0;
                        d.s3Count = 0;
                        return true;  // Data 50Hz siap
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
            sim800.read();  // Kuras buffer serial
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

bool adaTangan() {
      // Ambil nilai IR instan tanpa ganggu antrean FIFO
      long currentIR = particleSensor.getIR();
      static unsigned long lastUpdate = 0;

      // Filter jangkauan nilai IR (Threshold hasil tuning)
      if (currentIR > 150000 && currentIR < 220000) {
            return true;
      } else {
            // Pengingat Serial Monitor tiap 500ms agar tidak banjir teks
            if (millis() - lastUpdate > 500) {
                  Serial.println(F(">>> Pasang Sensor ke Pergelangan !"));
                  lastUpdate = millis();
            }
            return false;
      }
}