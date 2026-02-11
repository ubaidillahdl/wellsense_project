#ifndef GLOBAL_H
#define GLOBAL_H

#include <Arduino.h>
#include <MAX30105.h>
#include <SoftwareSerial.h>
#include <U8g2lib.h>

// --- 1. KONFIGURASI JARINGAN & BUFFER ---
#define PANJANG_BUFFER 150         // 150 data @50Hz = 3 detik rekaman
#define SERVER_IP "192.168.0.105"  // IP Server Python (sesuaikan dengan WiFi/LAN)
#define SERVER_PORT "5005"         // Port socket Python
#define DEVICE_TOKEN "WS-01-PROTOTYPE"

// --- 2. DEFINISI STRUKTUR DATA (State Management) ---
struct DesimasiState {
      int32_t s1Sum, s2Sum, s3Sum;
      uint8_t s1Count, s2Count, s3Count;
};

struct LpfState {
      int32_t x1 = 0;
      int32_t y1 = 0;
};

struct DataSensor {
      int16_t bufferIR[PANJANG_BUFFER];
      int16_t bufferRed[PANJANG_BUFFER];
};

struct HasilVitals {
      float hr;
      float spo2;
      float sbp;
      float dbp;
      float hb;
      int std;
};

enum State {
      ST_STANDBY,
      ST_SAMPLING,
      ST_KIRIM_DATA,
      ST_DISPLAY
};

// --- 3. DEKLARASI EXTERN (Variabel Global antar File) ---
extern DesimasiState desimRed, desimIR;
extern LpfState filterRed, filterIR;
extern DataSensor wadah;
extern HasilVitals dataVitals;
extern State currentState;

extern uint8_t bufferIdx;
extern int32_t rawRed, filteredRed, rawIR, filteredIR;
extern unsigned long waktuMulai, waktuMulaiSesi;
extern bool butuhRetryCepat;
extern bool dataReady;

// --- 4. OBJEK PERIPHERAL ---
extern MAX30105 particleSensor;
extern SoftwareSerial sim800;
extern U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2;

// --- 5. KONFIGURASI PIN ---
const byte interruptPin = 3;  // Pin INT dari MAX30105
const byte rxSim = 4;         // RX Wemos -> TX SIM800
const byte txSim = 5;         // TX Wemos -> RX SIM800

#endif
