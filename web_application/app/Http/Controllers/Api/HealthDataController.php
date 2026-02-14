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
     * Handle Path 2: Trigger Dashboard Real-time
     * (Data sudah disimpan ke DB via Python Path 1)
     */
    public function store(Request $request)
    {
        // 1. Validasi Input: Pastikan Python mengirimkan token
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
            // 2. Verifikasi Device: Cek apakah token terdaftar di database kita
            $device = Device::where('device_token', $request->device_token)->first();

            if (!$device) {
                return response()->json([
                    'status' => 'error',
                    'message' => 'Device Token tidak valid!'
                ], 403);
            }

            // 3. Logika Path 2 (Trigger Only):
            // Kita tidak melakukan Save ke DB di sini (Path 1 sudah handle).
            // Mendatang: Letakkan fungsi Broadcast/Websocket di sini.
            $latestData = DataKesehatan::where('device_id', $device->id)->latest()->first();

            if ($latestData) {
                // Ini perintah untuk membroadcast data ke WebSocket (Reverb)
                event(new HealthDataReceived(($latestData)));
            }


            return response()->json([
                'status' => 'success',
                'message' => 'Path 2 Berhasil: Sinyal pemicu dashboard diterima.',
                'data' => [
                    'nama_device' => $device->nama_device,
                    'vitals' => [
                        'hr' => $latestData->hr,
                        'spo2' => $latestData->spo2,
                        'sbp' => $latestData->sbp,
                        'dbp' => $latestData->dbp,
                        'hb' => $latestData->hb,
                    ]
                ]
            ], 200);
        } catch (\Exception $e) {
            // 4. Error Handling: Catat di storage/logs/laravel.log jika server bermasalah
            Log::error("Error pada Path 2: " . $e->getMessage());
            return response()->json([
                'status' => 'error',
                'message' => 'Terjadi kesalahan di server'
            ], 500);
        }
    }
}
