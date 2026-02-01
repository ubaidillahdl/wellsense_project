<?php

namespace App\Http\Controllers\Api\V1;

use Illuminate\Http\Request;
use Illuminate\Support\Facades\Log;
use App\Http\Controllers\Controller;

class IotController extends Controller
{
    /**
     * Display a listing of the resource.
     */
    public function index()
    {
        //
    }

    /**
     * Store a newly created resource in storage.
     */
    public function store(Request $request)
    {
        //
    }

    /**
     * Display the specified resource.
     */
    public function show(string $id)
    {
        //
    }

    /**
     * Update the specified resource in storage.
     */
    public function update(Request $request, string $id)
    {
        //
    }

    /**
     * Remove the specified resource from storage.
     */
    public function destroy(string $id)
    {
        //
    }


    public function ingest(Request $request)
    {
        $packetId = $request->input('packet_id');
        $deviceId = $request->input('device_id');
        $data     = $request->input('data');

        // Tulis ke file storage/logs/laravel.log agar kita bisa liat aslinya
        Log::info('Data Masuk:', [
            'packet_id' => $packetId,
            'device_id' => $deviceId,
            'data_type' => gettype($data), // Cek apakah ini string atau array
            'raw_body'  => $request->getContent() // Ini isi mentah yang dikirim Wemos
        ]);

        if (!$packetId || !$deviceId || !is_array($data)) {
            return response()->json([
                'status' => 'error',
                'message' => 'format data tidak valid'
            ], 400);
        }

        return response()->json([
            'status' => 'ok',
            'ack_packet' => $packetId
        ]);
    }
}
