# Wellsense IoT Project

Proyek sistem pemantauan vital sign (PPG) berbasis IoT. Proyek ini mencakup pengembangan firmware microcontroller, server pemrosesan data, dan dashboard pemantauan.

## 🚀 Struktur Proyek
- **firmware_arduino/**: Kode sumber C++ untuk Pro Mini/Arduino menggunakan PlatformIO.
- **python_server/**: (Coming Soon) Server pengolah data untuk komputasi algoritma berat.
- **web_dashboard/**: (Coming Soon) Antarmuka pengguna berbasis web untuk visualisasi data.

## 📌 Riwayat Versi (Milestones)

### [v1.0] - Era Full Arduino (Initial Release)
- Implementasi algoritma deteksi puncak (peak) dan lembah (valley) secara lokal di Arduino.
- Filter sinyal PPG menggunakan `PengolahSinyalPPG.h`.
- Output data diformat untuk visualisasi real-time (MATLAB/Serial Plotter).
- **Kondisi**: Versi stabil terakhir sebelum pemindahan logika ke server Python karena keterbatasan RAM.

---

## 🛠️ Pengembangan
### Prasyarat
- [VS Code](https://code.visualstudio.com/) + Extension [PlatformIO](https://platformio.org/).
- Driver USB to Serial (untuk Pro Mini).

### Cara Penggunaan
1. Clone repository ini.
2. Buka folder `firmware_arduino` di VS Code via PlatformIO.
3. Hubungkan board dan lakukan **Build & Upload**.

## 📝 Catatan Implementasi
- Hardware: Pro Mini 3.3V 8MHz / 16MHz.
- Sensor: MAX30105.
- Komunikasi: Saat ini masih menggunakan Serial, rencana migrasi ke SIM800C (GSM).
