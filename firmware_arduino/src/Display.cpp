#include "Functions.h"
#include "Global.h"

void initDisp() {
      u8g2.begin();

      u8g2.setFont(u8g2_font_ncenB14_tr);
      u8g2.firstPage();
      do {
            u8g2.setCursor(17, 40);
            u8g2.print(F("Wellsense"));
      } while (u8g2.nextPage());

      delay(5000);
      dispClear();
}

void dispClear() {
      u8g2.firstPage();
      do {
      } while (u8g2.nextPage());
}

void tampilkanPesanStandby() {
      static unsigned long lastUpdate = 0;
      // Update layar tiap 1 detik saja agar tidak menghambat loop utama
      if (millis() - lastUpdate < 1000) return;
      lastUpdate = millis();

      u8g2.firstPage();
      do {
            u8g2.setFont(u8g2_font_6x12_tf);  // Font simpel & hemat RAM
            u8g2.drawStr(25, 15, "WELLSENSE v1.0");

            u8g2.drawHLine(0, 20, 128);  // Garis pemisah

            u8g2.setFont(u8g2_font_7x13_tf);
            u8g2.drawStr(15, 45, "Tempelkan Jari");
            u8g2.drawStr(25, 60, "Untuk Mulai");
      } while (u8g2.nextPage());
}

// void tampilkanGrafikPPGAuto() {
//       int16_t minVal = 32767, maxVal = -32768;

//       // 1. Cari Min/Max untuk scaling dinamis
//       for (uint8_t i = 0; i < PANJANG_BUFFER; i++) {
//             if (wadah.bufferIR[i] < minVal) minVal = wadah.bufferIR[i];
//             if (wadah.bufferIR[i] > maxVal) maxVal = wadah.bufferIR[i];
//       }

//       // 2. Proteksi: Jika garis datar (no signal), kasih jarak biar gak div by zero
//       if (maxVal == minVal) maxVal = minVal + 10;

//       // 3. Gambar ke OLED
//       u8g2.firstPage();
//       do {
//             u8g2.setFont(u8g2_font_6x10_tf);
//             u8g2.drawStr(0, 10, "PPG Signal");  // Judul kecil biar rapi

//             for (uint8_t i = 0; i < PANJANG_BUFFER - 1; i++) {
//                   // Map data ke tinggi layar (OLED biasanya 64px, kita pakai area 15-60)
//                   uint8_t y1 = map(wadah.bufferIR[i], minVal, maxVal, 60, 15);
//                   uint8_t y2 = map(wadah.bufferIR[i + 1], minVal, maxVal, 60, 15);

//                   // x-scale: Jika PANJANG_BUFFER > lebar OLED (128), i perlu dikali scale
//                   u8g2.drawLine(i, y1, i + 1, y2);
//             }
//       } while (u8g2.nextPage());
// }