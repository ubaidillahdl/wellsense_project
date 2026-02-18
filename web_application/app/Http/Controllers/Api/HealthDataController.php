<?php

namespace App\Http\Controllers\Api;

use Illuminate\Support\Facades\Validator;
use App\Events\HealthDataReceived;
use App\Http\Controllers\Controller;
use Illuminate\Support\Facades\Log;
use App\Jobs\ProcessGeminiInsight;
use App\Models\DataKesehatan;
use Illuminate\Http\Request;
use App\Models\Device;

class HealthDataController extends Controller
{
    /**
     * Pemicu Broadcast Real-time dan Analisis AI.
     * Dipanggil oleh script Python setelah data tersimpan di database.
     */
    public function store(Request $request)
    {
        // 1. Validasi Token Device
        $validator = Validator::make($request->all(), [
            'device_token' => 'required|string',
        ]);

        if ($validator->fails()) {
            return response()->json([
                'status' => 'error',
                'message' => $validator->errors()
            ], 422);
        }

        try {
            // 2. Verifikasi autentikasi device
            $device = Device::where('device_token', $request->device_token)->first();

            if (!$device) {
                return response()->json([
                    'status' => 'error',
                    'message' => 'Device Token tidak valid!'
                ], 403);
            }

            // 3. Ambil entri data terbaru untuk diproses
            $latestData = DataKesehatan::where('device_id', $device->id)->latest()->first();

            if ($latestData) {
                // Ambil data sebelumnya untuk komparasi tren/lonjakan
                $previousData = DataKesehatan::where('device_id', $device->id)->where('id', '<', $latestData->id)->latest()->first();

                // Evaluasi apakah data menunjukkan indikasi anomali medis
                $shouldAskAI = $this->cekAnomali($latestData, $previousData);

                // Susun payload untuk kebutuhan dashboard (WebSocket)
                $payload = [
                    'filtered_ir' => $latestData->filtered_ir,
                    'vitals' => [
                        'hr' => $latestData->hr,
                        'spo2' => $latestData->spo2,
                        'sbp' => $latestData->sbp,
                        'dbp' => $latestData->dbp,
                        'hb' => $latestData->hb,
                    ],
                ];

                // 4. Kirim data secara real-time via Laravel Reverb
                event(new HealthDataReceived(($payload)));

                // 5. Eksekusi analisis AI jika terdeteksi anomali
                if ($shouldAskAI) {
                    ProcessGeminiInsight::dispatch($latestData);
                }
            }

            return response()->json([
                'status' => 'success',
                'message' => 'Broadcast data kesehatan berhasil dikirim.',
            ], 200);
        } catch (\Exception $e) {
            // Log untuk kebutuhan debugging internal
            Log::error("HealthDataController@store: Gagal memicu analisis data. Pesan: " . $e->getMessage());

            return response()->json([
                'status' => 'error',
                'message' => 'Terjadi kesalahan internal saat memproses data kesehatan.'
            ], 500);
        }
    }

    /**
     * Mengevaluasi data berdasarkan ambang batas statis, tren, 
     * serta manajemen frekuensi request (Cooldown Cache).
     */
    private function cekAnomali($saatIni, $sebelumnya)
    {
        // Safety check: pastikan data saat ini tidak null
        if (!$saatIni) return false;

        // --- 1. CEK KONDISI KRITIS (BYPASS COOLDOWN) ---
        // Gunakan property id dari model untuk key cache yang unik
        $cacheKey = 'cooldown_ai_user_' . ($saatIni->pengguna_id ?? 'guest');

        $kritis = ($saatIni->sbp > 170) ||
            ($saatIni->hr > 130 || $saatIni->hr < 45) ||
            ($saatIni->spo2 < 88);

        if ($kritis) {
            // Bongkar paksa cache agar Gemini langsung merespons kondisi darurat
            cache()->forget($cacheKey);
            return true;
        }

        // --- 2. CEK COOLDOWN CACHE ---
        // Jika masih dalam masa tenang 10 menit, skip request ke Gemini
        if (cache()->has($cacheKey)) {
            return false;
        }

        // --- 3. AMBANG BATAS STATIS ---
        $anomaliStatis =
            ($saatIni->sbp > 140 || $saatIni->sbp < 90) ||
            ($saatIni->dbp > 90 || $saatIni->dbp < 60) ||
            ($saatIni->hr > 100 || $saatIni->hr < 60) ||
            ($saatIni->spo2 < 95) ||
            ($saatIni->hb < 110);

        // --- 4. AMBANG BATAS TREN ---
        $anomaliTren = false;
        if ($sebelumnya) {
            $anomaliTren = abs($saatIni->sbp - $sebelumnya->sbp) > 15 ||
                abs($saatIni->dbp - $sebelumnya->dbp) > 15 ||
                abs($saatIni->hr - $sebelumnya->hr) > 20 ||
                abs($saatIni->spo2 - $sebelumnya->spo2) > 2 ||
                abs($saatIni->hb - $sebelumnya->hb) > 1;
        }

        // --- 5. EKSEKUSI ---
        if ($anomaliStatis || $anomaliTren) {
            // Set cooldown 10 menit
            cache()->put($cacheKey, true, now()->addMinutes(10));
            return true;
        }
        return false;
    }
}
