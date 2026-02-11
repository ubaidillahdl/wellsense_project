#include "Functions.h"
#include "Global.h"

void initDisp() {
      u8g2.begin();
      u8g2.setFont(u8g2_font_6x10_tr);

      u8g2.firstPage();
      do {
            u8g2.drawUTF8(0, 20, "READY");
      } while (u8g2.nextPage());
}

void tampilGrafikSesiSelesai() {
      u8g2.firstPage();
      do {
            u8g2.setFont(u8g2_font_5x8_tr);
            u8g2.drawStr(0, 10, "DATA SELESAI - KIRIM...");

            // Cari nilai Min dan Max untuk Auto-Scaling Y
            int16_t minVal = 32767;
            int16_t maxVal = -32768;
            for (uint8_t i = 0; i < PANJANG_BUFFER; i++) {
                  if (wadah.bufferIR[i] < minVal) minVal = wadah.bufferIR[i];
                  if (wadah.bufferIR[i] > maxVal) maxVal = wadah.bufferIR[i];
            }

            // Gambar grafik
            for (uint8_t i = 1; i < 128; i++) {
                  // Map index 0-127 ke index buffer 0-149 (Scaling X)
                  uint8_t idxPrev = (uint16_t)(i - 1) * PANJANG_BUFFER / 128;
                  uint8_t idxNow = (uint16_t)i * PANJANG_BUFFER / 128;

                  // Map nilai sensor ke tinggi layar 15-63 (Scaling Y)
                  int yPrev = map(wadah.bufferIR[idxPrev], minVal, maxVal, 60, 15);
                  int yNow = map(wadah.bufferIR[idxNow], minVal, maxVal, 60, 15);

                  u8g2.drawLine(i - 1, yPrev, i, yNow);
            }
      } while (u8g2.nextPage());
}