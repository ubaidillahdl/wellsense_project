#include "Functions.h"
#include "Global.h"

int8_t prosesKirimData() {
      // 0. VALIDASI KONEKSI
      if (!cekInternet()) return 0;         // Gagal Internet (OFFLINE)
      if (!hubungkanKePython()) return -1;  // Gagal Port/Server (REFUSED)

      // 1. HITUNG PANJANG DATA (Wajib untuk AT+CIPSEND)
      long totalChar = strlen(DEVICE_TOKEN) + 1;  // Token + Pemisah "|"
      for (uint8_t i = 0; i < PANJANG_BUFFER; i++) {
            char buf[20];
            // Simulasi format "IR:RED," untuk menghitung jumlah karakter
            totalChar +=
                sprintf(buf, "%u:%u,", wadah.bufferIR[i], wadah.bufferRed[i]);
      }
      totalChar += 1;  // Karakter Newline (\n) di akhir

      // 2. REQUEST KIRIM KE SIM800
      sim800.print("AT+CIPSEND=");
      sim800.println(totalChar);
      delay(200);  // Tunggu respon ">" dari SIM800

      // 3. PROSES TRANSMISI DATA
      sim800.print(DEVICE_TOKEN);
      sim800.print(F("|"));
      for (uint8_t i = 0; i < PANJANG_BUFFER; i++) {
            sim800.print(wadah.bufferIR[i]);
            sim800.print(F(":"));
            sim800.print(wadah.bufferRed[i]);
            sim800.print(F(","));
      }
      sim800.print(F("\n"));
      Serial.println(F(">>> DATA Terkirim ke Server !"));

      // 4. MENUNGGU RESPON BALIK (TIMEOUT 5 DETIK)
      unsigned long waitReply = millis();
      bool dataReceived = false;

      while (millis() - waitReply < 5000) {
            if (sim800.available()) {
                  char c = sim800.read();
                  if (c == '*') {  // Header paket terdeteksi
                        char feedbackBuf[64];
                        memset(feedbackBuf, 0, sizeof(feedbackBuf));  // Bersihkan sisa data

                        // Baca sampai terminator '#' (Format: *DATA#)
                        uint8_t len = sim800.readBytesUntil('#', feedbackBuf, sizeof(feedbackBuf) - 1);
                        feedbackBuf[len] = '\0';

                        if (pecahDataFeedback(feedbackBuf)) {
                              return 1;  // SUKSES: Data diolah & tampil
                        } else {
                              butuhRetryCepat = true;
                              return -3;  // UNSTEADY: Data rusak/gagal analisis
                        }
                        dataReceived = true;
                        break;
                  }
            }
      }

      // 5. PENANGANAN JIKA TIDAK ADA BALASAN
      if (!dataReceived) {
            Serial.println(F(">>> GAGAL mendapat respon Server !"));
            butuhRetryCepat = true;
      }

      delay(500);
      sim800.println(F("AT+CIPCLOSE"));  // Tutup sesi TCP demi keamanan
      return -2;                         // TIMEOUT
}

bool cekInternet() {
      // Kuras sisa data di buffer serial SIM800
      while (sim800.available()) sim800.read();

      // Perintah cek internet dengan cara ping IP google
      sim800.println(F("AT+PING=\"8.8.8.8\""));

      // Tunggu respon sukses maksimal 3 detik
      unsigned long timeout = millis();
      while (millis() - timeout < 3000) {
            if (sim800.available()) {
                  char tempBuffer[50];
                  memset(tempBuffer, 0, sizeof(tempBuffer));

                  int len = sim800.readBytes(tempBuffer, sizeof(tempBuffer) - 1);
                  tempBuffer[len] = '\0';

                  // Cek indikasi koneksi berhasil
                  if (strstr(tempBuffer, "OK")) {
                        Serial.println(F(">>> SUKSES Terhubung ke Internet !"));
                        return true;
                  }
            }
      }
      Serial.println(F(">>> GAGAL Terhubung ke Internet !"));
      return false;
}

bool hubungkanKePython() {
      // Kuras sisa data di buffer serial SIM800
      while (sim800.available()) sim800.read();

      // Perintah buka koneksi ke IP & Port server
      sim800.print(F("AT+CIPSTART=\"TCP\",\""));
      sim800.print(SERVER_IP);
      sim800.print(F("\","));
      sim800.println(SERVER_PORT);

      // Tunggu respon sukses maksimal 3 detik
      unsigned long timeout = millis();
      while (millis() - timeout < 3000) {
            if (sim800.available()) {
                  char tempBuffer[50];
                  memset(tempBuffer, 0, sizeof(tempBuffer));

                  int len = sim800.readBytes(tempBuffer, sizeof(tempBuffer) - 1);
                  tempBuffer[len] = '\0';

                  // Cek indikasi koneksi berhasil
                  if (strstr(tempBuffer, "OK") || strstr(tempBuffer, "CONNECT") ||
                      strstr(tempBuffer, "ALREADY CONNECTED")) {
                        Serial.println(F(">>> SUKSES Terhubung ke Server !"));
                        return true;
                  }
            }
      }
      Serial.println(F(">>> GAGAL Terhubung ke Server !"));
      return false;
}

bool pecahDataFeedback(char *buf) {
      bool hasilAnalisa = true;

      char *ptr = strtok(buf, ";");
      if (ptr) dataVitals.hr = atoi(ptr);

      ptr = strtok(NULL, ";");
      if (ptr) dataVitals.spo2 = atoi(ptr);

      ptr = strtok(NULL, ";");
      if (ptr) dataVitals.sbp = atoi(ptr);

      ptr = strtok(NULL, ";");
      if (ptr) dataVitals.dbp = atoi(ptr);

      ptr = strtok(NULL, ";");
      if (ptr) dataVitals.hb = atoi(ptr);

      ptr = strtok(NULL, ";");
      if (ptr) dataVitals.std = atoi(ptr);

      if (dataVitals.hr > 0) {
            // Print hasil ke Serial untuk verifikasi
            Serial.println(F("\n======= HASIL ANALISA ======="));
            Serial.print(F("HR\t: "));
            Serial.print(dataVitals.hr);
            Serial.println(F("\tbpm"));
            Serial.print(F("SPO2\t: "));
            Serial.print(dataVitals.spo2);
            Serial.println(F("\t%"));
            Serial.print(F("SBP\t: "));
            Serial.print(dataVitals.sbp);
            Serial.println(F("\tmmHg"));
            Serial.print(F("DBP\t: "));
            Serial.print(dataVitals.dbp);
            Serial.println(F("\tmmHg"));
            Serial.print(F("HB\t: "));
            Serial.print(dataVitals.hb);
            Serial.println(F("\tg/L"));
            Serial.print(F("STD\t: "));
            Serial.print(dataVitals.std);
            Serial.println(F("\tcounts"));
            Serial.println(F("============================="));
      } else {
            Serial.print(F(">>> STD "));
            Serial.print(dataVitals.std);  // Tampilkan tanpa desimal agar bersih
            Serial.println(F(" counts, sinyal tidak stabil !"));

            hasilAnalisa = false;
      }
      return hasilAnalisa;
}