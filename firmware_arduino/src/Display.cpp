#include "Functions.h"
#include "Global.h"

void initDisp() {
      Wire.begin();
      Wire.setClock(400000L);
      oled.begin(&Adafruit128x64, I2C_ADDRESS);

      oled.setCursor(13, 3);
      oled.setFont(Cooper19);
      oled.print(F("Wellsense"));

      delay(3000);
      oled.clear();
}

void tampilVitals() {
      uint8_t sbp = (uint8_t)ceil(dataVitals.sbp);
      uint8_t dbp = (uint8_t)ceil(dataVitals.dbp);
      uint8_t hr = constrain((uint8_t)ceil(dataVitals.hr), 0, 99);
      uint8_t spo2 = constrain((uint8_t)ceil(dataVitals.spo2), 0, 99);
      uint8_t hb = constrain((uint8_t)ceil(dataVitals.hb), 0, 99);

      // --- SISI KIRI (SBP & DBP) ---
      // Tetap hapus total karena anchorX (label) bisa geser
      int anchorX = (sbp >= 100 || dbp >= 100) ? 38 : 26;
      for (uint8_t r = 3; r <= 7; r++) {
            oled.clearField(0, r, 65);
      }

      oled.setFont(lcdnums12x16);
      oled.setCursor(0, 3);
      if (sbp < 100 && anchorX == 38) oled.print(F(" "));
      oled.print(sbp);
      oled.setCursor(0, 6);
      if (dbp < 100 && anchorX == 38) oled.print(F(" "));
      oled.print(dbp);

      oled.setFont(font5x7);
      oled.setCursor(anchorX, 3);
      oled.print(F("SBP"));
      oled.setCursor(anchorX, 4);
      oled.print(F("mmHg"));
      oled.setCursor(anchorX, 6);
      oled.print(F("DBP"));
      oled.setCursor(anchorX, 7);
      oled.print(F("mmHg"));

      // --- SISI KANAN (HR, SPO2, HB) ---
      // Label statis ditulis SEBELUMNYA (misal di setup atau saat jari masuk)
      // Di sini kita HANYA update angkanya saja agar label gausah dihapus

      oled.setFont(lcdnums12x16);

      // 1. Update Angka HR (Kolom 79, Baris 0-1)
      oled.clearField(79, 0, 24);
      oled.clearField(79, 1, 24);  // Cuma hapus area angka (2 digit = 24px)
      oled.setCursor(79, 0);
      if (hr < 10) oled.print(F(" "));
      oled.print(hr);

      // 2. Update Angka SPO2 (Kolom 79, Baris 3-4)
      oled.clearField(79, 3, 24);
      oled.clearField(79, 4, 24);
      oled.setCursor(79, 3);
      if (spo2 < 10) oled.print(F(" "));
      oled.print(spo2);

      // 3. Update Angka HB (Kolom 79, Baris 6-7)
      oled.clearField(79, 6, 24);
      oled.clearField(79, 7, 24);
      oled.setCursor(79, 6);
      if (hb < 10) oled.print(F(" "));
      oled.print(hb);

      // --- LABEL KANAN (TULIS ULANG TANPA CLEAR) ---
      // Tujuannya agar label tetap muncul jika layar sempat terhapus di state sebelumnya
      oled.setFont(font5x7);
      oled.setCursor(105, 0);
      oled.print(F("HR"));
      oled.setCursor(105, 1);
      oled.print(F("bpm"));
      oled.setCursor(105, 3);
      oled.print(F("SPO2"));
      oled.setCursor(105, 4);
      oled.print(F("%"));
      oled.setCursor(105, 6);
      oled.print(F("HB"));
      oled.setCursor(105, 7);
      oled.print(F("g/dL"));
}
