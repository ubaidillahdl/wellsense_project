# Wellsense Project 🩺

Sistem monitoring sinyal PPG (Photoplethysmogram) menggunakan sensor MAX30105 dan Arduino Pro Mini. Proyek ini fokus pada pengolahan sinyal digital secara real-time untuk deteksi detak jantung yang akurat.

## 🚀 Status Terakhir: v1.3 (Stable DSP)

Versi terbaru (v1.3) telah mengimplementasikan:

- **Chebyshev Filter:** Mengurangi noise frekuensi tinggi untuk sinyal yang lebih halus.
- **Sliding Window Buffer (200 samples):** Pengolahan data real-time tanpa jeda (zero-phase delay).
- **Dynamic Prominence:** Kalibrasi otomatis ambang batas puncak (peak) berdasarkan kekuatan sinyal.

## 🛠️ Arsitektur Sistem

- **Hardware:** Arduino Pro Mini, MAX30105 Sensor.
- **Library Khusus:** `PengolahSinyalPPG` (Custom DSP Library).
- **Format Data:** Output Serial menggunakan prefix `RT:` untuk pengolahan lebih lanjut di sisi Server/MATLAB.

## 📌 Rencana Pengembangan (Roadmap)

- [x] v1.1 - Batch Processing
- [x] v1.3 - Real-time Sliding Window & Chebyshev Filter
- [ ] v2.0 - Integrasi **SIM800C** & Server Side Processing (Python).
- [ ] Implementasi pengiriman data via GPRS (menggantikan Serial).

> **Catatan:** Pengolahan berat sengaja dipindah ke sisi server untuk menjaga efisiensi RAM pada microcontroller karena keterbatasan memori saat menggunakan modul SIM800C.
