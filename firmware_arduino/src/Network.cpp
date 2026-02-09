#include "Functions.h"
#include "Global.h"

// --- MEMBUKA KONEKSI TCP ---
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

// --- PROSES KIRIM DATA & TERIMA FEEDBACK ---
void prosesKirimData() {
      detachInterrupt(digitalPinToInterrupt(interruptPin));  // Stop sampling selama upload

      if (hubungkanKePython()) {
            // 1. Hitung total karakter yang akan dikirim (untuk AT+CIPSEND)
            long totalChar = 0;

            // Tambahkan panjang Token + karakter pemisah '|'
            totalChar += strlen(DEVICE_TOKEN);
            totalChar += 1;  // Untuk karakter '|'

            for (uint8_t i = 0; i < PANJANG_BUFFER; i++) {
                  char buf[20];
                  totalChar +=
                      sprintf(buf, "%u:%u,", wadah.bufferIR[i], wadah.bufferRed[i]);
            }
            totalChar += 1;  // Untuk newline '\n'

            // 2. Beritahu SIM800 jumlah data yang akan dikirim
            sim800.print("AT+CIPSEND=");
            sim800.println(totalChar);
            delay(200);  // Jeda agar SIM800 siap menerima data

            // 3. KIRIM IDENTITAS (TOKEN)
            sim800.print(DEVICE_TOKEN);
            sim800.print(F("|"));  // Pemisah utama

            // 4. Kirim data sensor dari Buffer
            for (uint8_t i = 0; i < PANJANG_BUFFER; i++) {
                  sim800.print(wadah.bufferIR[i]);
                  sim800.print(F(":"));
                  sim800.print(wadah.bufferRed[i]);
                  sim800.print(F(","));

                  // Serial.print(wadah.bufferIR[i]);
                  // Serial.print(", ");
            }
            sim800.print(F("\n"));
            Serial.println(F(">>> DATA Terkirim ke Server !"));

            // 4. Tunggu feedback hasil olah data dari Python
            unsigned long waitReply = millis();
            bool dataReceived = false;

            while (millis() - waitReply < 5000) {  // Timeout 5 detik
                  if (sim800.available()) {
                        char c = sim800.read();
                        if (c == '*') {  // Paket Data Terdeteksi
                              String payload = sim800.readStringUntil('#');
                              pecahDataFeedback(payload);
                              // Serial.print(F(">>> Feedback dari Server : "));
                              // Serial.println(vitals);

                              butuhRetryCepat = false;
                              dataReceived = true;
                              break;
                        } else if (c == 'H') {  // Heartbeat/Server Ready tapi data kosong
                              Serial.println(F(">>> Tidak ada data dari Server !"));
                              butuhRetryCepat = true;
                              dataReceived = true;
                              break;
                        }
                  }
            }

            if (!dataReceived) {
                  Serial.println(F(">>> Gagal mendapat respon dari Server !"));
                  butuhRetryCepat = true;
            }

            delay(500);
            sim800.println(F("AT+CIPCLOSE"));  // Tutup koneksi TCP

            // --- RESET STATE UNTUK SESI BERIKUTNYA ---
            bufferIdx = 0;
            sedangIstirahat = true;
            waktuMulai = millis();
            particleSensor.setPulseAmplitudeRed(0);  // Matikan LED Red
      }
}

void pecahDataFeedback(String raw) {
      char buf[80];
      raw.toCharArray(buf, sizeof(buf));

      char *ptr = strtok(buf, ";");
      if (ptr) dataVitals.hr = atof(ptr);

      ptr = strtok(NULL, ";");
      if (ptr) dataVitals.spo2 = atof(ptr);

      ptr = strtok(NULL, ";");
      if (ptr) dataVitals.sbp = atof(ptr);

      ptr = strtok(NULL, ";");
      if (ptr) dataVitals.dbp = atof(ptr);

      ptr = strtok(NULL, ";");
      if (ptr) dataVitals.hb = atof(ptr);

      ptr = strtok(NULL, ";");
      if (ptr) dataVitals.std = atoi(ptr);

      if (dataVitals.hr > 0) {
            // Print hasil ke Serial untuk verifikasi
            Serial.println(F("\n======= HASIL ANALISA ======="));
            Serial.print(F("HR\t: "));
            Serial.print(dataVitals.hr, 2);
            Serial.println(F("\t\tbpm"));
            Serial.print(F("SPO2\t: "));
            Serial.print(dataVitals.spo2, 2);
            Serial.println(F("\t\t%"));
            Serial.print(F("SBP\t: "));
            Serial.print(dataVitals.sbp, 4);
            Serial.println(F("\tmmHg"));
            Serial.print(F("DBP\t: "));
            Serial.print(dataVitals.dbp, 4);
            Serial.println(F("\tmmHg"));
            Serial.print(F("HB\t: "));
            Serial.print(dataVitals.hb, 2);
            Serial.println(F("\t\tg/dL"));
            Serial.print(F("STD\t: "));
            Serial.print(dataVitals.std);
            Serial.println(F("\t\tcounts"));
            Serial.println(F("============================="));
      } else {
            // Jika HR == 0, artinya analisis GAGAL (Sensor lepas/Noise)
            Serial.print(F(">>> STD "));
            Serial.print(dataVitals.std);  // Tampilkan tanpa desimal agar bersih

            Serial.println(F(" counts, sinyal tidak stabil..."));
      }
}