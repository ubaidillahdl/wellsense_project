<?php

namespace App\Jobs;

use Illuminate\Contracts\Queue\ShouldQueue;
use Illuminate\Foundation\Queue\Queueable;
use Illuminate\Support\Facades\Http;
use App\Events\HealthDataReceived;
use Illuminate\Support\Facades\Log;
use Illuminate\Support\Facades\DB;

class ProcessGeminiInsight implements ShouldQueue
{
    use Queueable;

    // Properti untuk menyimpan data yang dikirim saat Job dipicu
    public $latestData;

    /**
     * Inisialisasi Job dengan data kesehatan terbaru dan nama device
     */
    public function __construct($latestData)
    {
        $this->latestData = $latestData;
    }

    /**
     * Eksekusi proses analisis AI di background (Queue)
     */
    public function handle(): void
    {
        // 1. Konfigurasi API Gemini
        $apiKey = env('GEMINI_API_KEY');

        // Prompt yang spesifik agar respon Gemini singkat dan padat
        // $prompt = "Analisis: BP {$this->latestData->sbp}/{$this->latestData->dbp}, HR {$this->latestData->hr}, SpO2 {$this->latestData->spo2}. 1 saran medis < 15 kata.";

        // Prompt baru yang memaksa Gemini menyebutkan parameter bermasalah
        $prompt = "Analisis data vital ini: BP {$this->latestData->sbp}/{$this->latestData->dbp}, " .
            "HR {$this->latestData->hr}, SpO2 {$this->latestData->spo2}, Hb {$this->latestData->hb}. " .
            "Tugas: Sebutkan parameter apa saja yang tidak normal, lalu berikan 1 saran medis singkat. " .
            "Gunakan Bahasa Indonesia. Total jawaban maksimal 20 kata.";


        try {
            // 2. Request ke API Gemini menggunakan Facade Http
            $response = Http::post("https://generativelanguage.googleapis.com/v1beta/models/gemini-2.5-flash:generateContent?key={$apiKey}", [
                'contents' => [[
                    'parts' => [['text' => $prompt]]
                ]]
            ]);

            Log::info("Gemini Job: Status API  " . $response->status());

            if ($response->successful()) {
                // Ambil teks hasil analisis dari struktur JSON Gemini
                $insight = $response->json()['candidates'][0]['content']['parts'][0]['text'] ?? null;

                if ($insight) {
                    // 3. Simpan hasil analisis ke database (Relasi ke Pengguna)
                    DB::table('ai_insights')->insert([
                        'pengguna_id' => $this->latestData->pengguna_id,
                        'insight'           => $insight,
                        'created_at'        => now(),
                        'updated_at'        => now(),
                    ]);

                    // 4. Kirim sinyal ke Dashboard untuk update kolom Insight AI secara real-time
                    event(new HealthDataReceived([
                        'is_update_insight' => true, // Flag khusus untuk membedakan dengan broadcast data vital biasa
                        'ai_insight' => $insight
                    ]));

                    Log::info("Gemini Job: Insight berhasil disimpan dan disiarkan.");
                } else {
                    Log::error("Gemini Job Gagal: " . $response->body());
                }
            }
        } catch (\Throwable $e) {
            Log::error("Gemini Job Exception: " . $e->getMessage());
        }
    }
}
