<?php

namespace App\Http\Controllers\Api;

use App\Events\HealthDataReceived;
use App\Http\Controllers\Controller;
use App\Models\DataKesehatan;
use App\Models\Device;
use Illuminate\Support\Facades\Log;
use Illuminate\Support\Facades\Validator;
use Illuminate\Http\Request;

class HealthDataController extends Controller
{
    /**
     * Path 2: Trigger Broadcast ke Dashboard
     * Alur: Python Simpan ke DB -> Python Hit API Ini -> Laravel Broadcast ke Reverb
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
            // 2. Verifikasi kepemilikan device
            $device = Device::where('device_token', $request->device_token)->first();

            if (!$device) {
                return response()->json([
                    'status' => 'error',
                    'message' => 'Device Token tidak valid!'
                ], 403);
            }

            // 3. Ambil data terbaru yang baru saja disimpan oleh Python (Path 1)
            $latestData = DataKesehatan::where('device_id', $device->id)->latest()->first();

            if ($latestData) {
                // Susun payload agar rapi saat diterima Frontend/JavaScript
                $payload = [
                    'nama_device' => $device->nama_device,
                    'filtered_ir' => $latestData->filtered_ir,
                    'vitals' => [
                        'hr' => $latestData->hr,
                        'spo2' => $latestData->spo2,
                        'sbp' => $latestData->sbp,
                        'dbp' => $latestData->dbp,
                        'hb' => $latestData->hb,
                    ]
                ];

                // 4. Kirim data ke "udara" via WebSocket (Reverb)
                event(new HealthDataReceived(($payload)));
            }

            return response()->json([
                'status' => 'success',
                'message' => 'Path 2 Berhasil: Sinyal pemicu dashboard diterima.',
                'data' => $payload
            ], 200);
        } catch (\Exception $e) {
            // 5. Catat log jika ada error sistem (Cek storage/logs/laravel.log)
            Log::error("Error pada Path 2: " . $e->getMessage());
            return response()->json([
                'status' => 'error',
                'message' => 'Terjadi kesalahan di server'
            ], 500);
        }
    }
}
