#include "Functions.h"
#include "Global.h"

void initDisp() {
      Wire.begin();
      Wire.setClock(400000L);
      oled.begin(&Adafruit128x64, I2C_ADDRESS);

      oled.setCursor(0, 3);
      oled.setFont(Cooper19);
      oled.print(F("Wellsense"));

      delay(3000);
      oled.clear();
}

void tampilVitals() {
      // 1. KONVERSI & PEMBATASAN NILAI: Ambil angka bulat & batasi 0-99 untuk sisi kanan
      uint8_t sbp = (uint8_t)ceil(dataVitals.sbp);
      uint8_t dbp = (uint8_t)ceil(dataVitals.dbp);
      uint8_t hr = constrain((uint8_t)ceil(dataVitals.hr), 0, 99);
      uint8_t spo2 = constrain((uint8_t)ceil(dataVitals.spo2), 0, 99);
      uint8_t hb = constrain((uint8_t)ceil(dataVitals.hb), 0, 99);

      // --- SISI KIRI (Tensi: SBP & DBP) ---
      // Geser posisi label jika angka tensi mencapai ratusan (3 digit)
      uint8_t anchorX = (sbp >= 100 || dbp >= 100) ? 38 : 26;

      // Hapus area kiri (baris 3 sampai 7)
      for (uint8_t r = 3; r <= 7; r++) oled.clearField(0, r, 65);

      // Cetak angka SBP & DBP (Font Besar)
      oled.setFont(lcdnums12x16);
      oled.setCursor(0, 3);
      if (sbp < 100 && anchorX == 38) oled.print(F(" "));  // Alignment spasi
      oled.print(sbp);
      oled.setCursor(0, 6);
      if (dbp < 100 && anchorX == 38) oled.print(F(" "));
      oled.print(dbp);

      // Cetak Label Tensi (Font Kecil)
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
      oled.setFont(lcdnums12x16);

      // 1. Update HR: Hapus area angka saja (24px) lalu cetak
      oled.clearField(79, 0, 24);
      oled.clearField(79, 1, 24);
      oled.setCursor(79, 0);
      if (hr < 10) oled.print(F(" "));
      oled.print(hr);

      // 2. Update SPO2
      oled.clearField(79, 3, 24);
      oled.clearField(79, 4, 24);
      oled.setCursor(79, 3);
      if (spo2 < 10) oled.print(F(" "));
      oled.print(spo2);

      // 3. Update HB
      oled.clearField(79, 6, 24);
      oled.clearField(79, 7, 24);
      oled.setCursor(79, 6);
      if (hb < 10) oled.print(F(" "));
      oled.print(hb);

      // --- LABEL KANAN (Static Labels) ---
      // Ditulis ulang tanpa clear agar tetap muncul meski state berubah
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

unsigned long lastStateTime = 0;
const int jedaState = 1000;  // Jeda minimal 1 detik antar perpindahan layar
void updateTampilan() {
      // 1. CEK PERUBAHAN: Hanya update jika state berubah atau ada data baru
      if (screenState == lastScreen && !dataUpdate) return;

      // 2. FILTER WAKTU: Jangan ganti layar terlalu cepat (kecuali FINISHED)
      if (screenState != lastScreen) {
            if (millis() - lastStateTime < jedaState && screenState != SCR_FINISHED) return;
            lastStateTime = millis();
      }

      oled.setFont(font5x7);
      oled.set1X();

      String statusTeks = "";
      uint8_t progressLevel = 0;

      // 3. LOGIKA TEKS & PROGRESS: Tentukan apa yang muncul di layar
      switch (screenState) {
            case SCR_READY:
                  statusTeks = "READY";
                  progressLevel = 0;
                  break;
            case SCR_SAMPLING:
                  // Pakai RESAMPLING jika sebelumnya gagal karena tangan goyang
                  statusTeks = (res == -3 || res == -2 || res == -1 || res == 0) ? "RESAMPLING" : "SAMPLING";
                  progressLevel = 1;
                  break;
            case SCR_UPLOADING:
                  statusTeks = "UPLOADING";
                  progressLevel = 2;
                  break;
            case SCR_FINISHED:
                  statusTeks = "FINISHED";
                  progressLevel = 3;
                  tampilVitals();  // Cetak BPM & SpO2 ke layar
                  break;
            case SCR_NET_ERR:
                  statusTeks = "OFFLINE";  // Sinyal/Internet bermasalah
                  progressLevel = 0;
                  break;
            case SCR_SRV_ERR:
                  // Detail error berdasarkan hasil respon (res)
                  switch (res) {
                        case -1:
                              statusTeks = "REFUSED";  // Server Python mati
                              break;
                        case -2:
                              statusTeks = "TIMEOUT";  // Server tidak merespon
                              break;
                        case -3:
                              statusTeks = "UNSTEADY";  // Tangan goyang
                              break;
                  }
                  progressLevel = 2;
                  break;
      }

      // 4. RENDERING BARIS 1: Status Teks + SpO2 Cleaner (10 Karakter)
      oled.setCursor(0, 0);
      oled.print(statusTeks);
      for (int i = 0; i < (10 - (int)statusTeks.length()); i++) oled.print(F(" "));

      // 5. RENDERING BARIS 2: Progress Indicator ( o . . . )
      oled.setCursor(0, 1);
      for (int i = 0; i <= 3; i++) oled.print((i <= progressLevel) ? F("o") : F("."));

      // 6. FINISHING: Catat state terakhir & reset flag data
      lastScreen = screenState;
      dataUpdate = false;
}