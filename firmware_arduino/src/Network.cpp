#include "Functions.h"
#include "Global.h"

// char* trimCRLF(char* str) {
//       // Trim depan
//       while (*str == '\r' || *str == '\n' || *str == ' ')
//             str++;

//       // Trim belakang
//       char* end = str + strlen(str) - 1;
//       while (end > str && (*end == '\r' || *end == '\n' || *end == ' '))
//             *end-- = '\0';

//       return str;
// }

void hubungkanKeInternet() {
      char buf[128];

      oled.clear();
      oled.setFont(System5x7);
      oled.setScrollMode(SCROLL_MODE_AUTO);

      auto logPesanRes = [&]() {
            oled.println(buf);
            delay(1500);
      };

      resAT("AT", 1000, buf, sizeof(buf));
      Serial.println(buf);

      while (!strstr(buf, "OK")) {
            Serial.println(F(">>> Menunggu SIM800C..."));

            oled.clear();
            for (uint8_t titik = 1; titik <= 5; titik++) {
                  oled.print(F("."));
                  delay(200);
            }

            delay(300);
            resAT("AT", 1000, buf, sizeof(buf));
      }
      Serial.println(F(">>> SIM800C Terhubung"));
      oled.println("");
      logPesanRes();

      // auto logPesanF = [&](const __FlashStringHelper* pesan) {
      //       oled.println(pesan);
      //       delay(1500);
      // };

      // auto logPesanRes = [&]() {
      //       oled.println(buf);  // langsung pakai buf, tidak perlu parameter
      //       delay(1500);
      // };

      // memset(buf, 0, sizeof(buf));
      // uint8_t index = 0;
      // while (true) {
      //       while (sim800.available()) {
      //             char c = sim800.read();
      //             if (index < sizeof(buf) - 1) {
      //                   buf[index++] = c;
      //                   buf[index] = '\0';
      //             }
      //             if (strstr(buf, "SMS Ready")) break;
      //       }

      //       Serial.println(buf);
      //       delay(1000);
      // }
      // Serial.println(F(">>> AMAN"));

      // Tunggu SIM800C selesai booting
      // memset(buf, 0, sizeof(buf));
      // Serial.println(buf);
      // uint32_t t = millis();
      // while (!strstr(buf, "RDY")) {
      //       if (sim800.available()) {
      //             uint16_t len = strlen(buf);
      //             if (len < sizeof(buf) - 1) {
      //                   buf[len] = sim800.read();
      //             }
      //       }
      //       // Timeout 10 detik, kalau lewat langsung lanjut
      //       if (millis() - t > 10000) break;
      // }
      // Serial.println(F(">>> SIM800C Boot Selesai"));

      // //
      // //
      // //
      // //
      // resAT("AT+CIPSTATUS", 1000, buf, sizeof(buf), false);
      // Serial.println(buf);  // debug dulu
      // if (!strstr(buf, "IP GPRSACT")) {
      //       // Cek kartu sim
      //       resAT("AT+CPIN?", 1000, buf, sizeof(buf));
      //       while (!strstr(buf, "READY")) {
      //             Serial.println(F(">>> Kartu SIM Belum Siap..."));
      //             logPesanRes();

      //             delay(500);
      //             resAT("AT+CPIN?", 1000, buf, sizeof(buf));
      //       }
      //       Serial.println(F(">>> Kartu SIM Sudah Siap"));
      //       logPesanRes();

      //       // Cek registrasi jaringan
      //       resAT("AT+CREG?", 1000, buf, sizeof(buf));
      //       while (!strstr(buf, ",1") && !strstr(buf, ",5")) {
      //             Serial.println(F(">>> Mendaftarkan ke Jaringan..."));
      //             logPesanRes();

      //             delay(500);
      //             resAT("AT+CREG?", 1000, buf, sizeof(buf));
      //       }
      //       Serial.println(F(">>> Terdaftar di Jaringan..."));
      //       logPesanRes();

      //       // Cek kekuatan sinyal
      //       resAT("AT+CSQ", 1000, buf, sizeof(buf));
      //       char* p = strstr(buf, ":");
      //       while (p == NULL) {
      //             delay(500);
      //             Serial.println(F(">>> CSQ Gagal Dibaca"));
      //             logPesanRes();

      //             resAT("AT+CSQ", 1000, buf, sizeof(buf));
      //             p = strstr(buf, ":");
      //       }

      //       uint8_t csqValue = atoi(p + 1);
      //       while (csqValue <= 10 || csqValue == 99) {
      //             delay(500);
      //             Serial.print(F(">>> Sinyal Lemah "));
      //             Serial.println(csqValue);

      //             resAT("AT+CSQ", 1000, buf, sizeof(buf));
      //             p = strstr(buf, ":");
      //             csqValue = atoi(p + 1);
      //       }
      //       Serial.print(F(">>> Kekuatan Sinyal "));
      //       Serial.println(csqValue);
      //       logPesanRes();

      //       // Menghubungkan ke jaringan
      //       resAT("AT+CGATT?", 1000, buf, sizeof(buf));
      //       while (!strstr(buf, ": 1")) {
      //             Serial.println(F(">>> Menghubungkan ke Jaringan..."));
      //             logPesanRes();

      //             delay(500);
      //             resAT("AT+CGATT?", 1000, buf, sizeof(buf));
      //       }
      //       Serial.println(F(">>> Terhubung ke Jaringan"));
      //       logPesanRes();

      //       // Set APN
      //       if (strstr(resAT("AT+CIPSTATUS", 1000, buf, sizeof(buf)), "IP INITIAL")) {
      //             resAT("AT+CSTT=\"internet\"", 1000, buf, sizeof(buf));
      //             while (!strstr(buf, "OK")) {
      //                   Serial.println(F(">>> Mencoba Set APN..."));
      //                   logPesanRes();

      //                   delay(500);
      //                   resAT("AT+CSTT=\"internet\"", 1000, buf, sizeof(buf));
      //             }
      //       }
      //       Serial.println(F(">>> SUKSES Set APN"));
      //       logPesanRes();

      //       // Menghubungkan ke Internet
      //       resAT("AT+CIICR", 5000, buf, sizeof(buf));
      //       while (!strstr(buf, "OK")) {
      //             Serial.println(F(">>> Menghubungkan ke Internet..."));
      //             logPesanRes();

      //             delay(500);
      //             resAT("AT+CIICR", 5000, buf, sizeof(buf));
      //       }
      // }
      // Serial.println(F(">>> Terhubung ke Internet"));
      // logPesanRes();
      // //
      // //
      // //
      // //
}

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

      while (millis() - waitReply < 20000) {
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

      // sim800.println(F("AT+HTTPINIT"));
      // delay(200);
      // sim800.println(F("AT+HTTPPARA=\"URL\",\"https://google.com\""));
      // delay(200);
      // sim800.println(F("AT+HTTPACTION=0"));
      sim800.println(F("AT+CIPPING=\"8.8.8.8\",1,20"));

      // Tunggu respon sukses maksimal 3 detik
      unsigned long timeout = millis();
      while (millis() - timeout < 15000) {
            if (sim800.available()) {
                  char tempBuffer[50];
                  memset(tempBuffer, 0, sizeof(tempBuffer));

                  int len = sim800.readBytes(tempBuffer, sizeof(tempBuffer) - 1);
                  tempBuffer[len] = '\0';

                  // Cek indikasi koneksi berhasil
                  if (strstr(tempBuffer, "OK")) {
                        // Serial.println(tempBuffer);
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

      // command buka koneksi ke IP & Port server
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
                  if (strstr(tempBuffer, "CONNECT OK") ||
                      strstr(tempBuffer, "ALREADY CONNECTED")) {
                        // Serial.println(tempBuffer);
                        Serial.println(F(">>> SUKSES Terhubung ke Server !"));
                        return true;
                  }
            }
      }
      Serial.println(F(">>> GAGAL Terhubung ke Server !"));
      return false;
}

bool pecahDataFeedback(char* buf) {
      bool hasilAnalisa = true;

      char* ptr = strtok(buf, ";");
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

char* resAT(const char* command, uint16_t timeout, char* buf, uint8_t bufSize, bool earlyExit = false) {
      memset(buf, 0, bufSize);

      // Flush semua data lama yang nyangkut di serial buffer
      while (sim800.available()) sim800.read();

      sim800.println(command);

      uint8_t index = 0;
      uint32_t t = millis();
      while (millis() - t < timeout) {
            while (sim800.available()) {
                  char c = sim800.read();
                  if (index < bufSize - 1) {
                        buf[index++] = c;
                        buf[index] = '\0';
                  }
            }
            // if (earlyExit && strstr(buf, "OK") || strstr(buf, "ERROR")) break;
      }
      return buf;
}