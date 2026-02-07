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

            // 3. Kirim data sensor dari Buffer
            for (uint8_t i = 0; i < PANJANG_BUFFER; i++) {
                  sim800.print(wadah.bufferIR[i]);
                  sim800.print(F(":"));
                  sim800.print(wadah.bufferRed[i]);
                  sim800.print(F(","));
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
                              String vitals = sim800.readStringUntil('#');
                              Serial.print(F(">>> Feedback dari Server : "));
                              Serial.println(vitals);

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