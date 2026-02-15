<?php

namespace App\Events;

use Illuminate\Broadcasting\Channel;
use Illuminate\Broadcasting\InteractsWithSockets;
use Illuminate\Contracts\Broadcasting\ShouldBroadcast;
use Illuminate\Foundation\Events\Dispatchable;
use Illuminate\Queue\SerializesModels;

class HealthDataReceived implements ShouldBroadcast
{
    use Dispatchable, InteractsWithSockets, SerializesModels;

    /**
     * Data yang ditaruh di variabel public akan otomatis 
     * terkirim sebagai payload ke WebSocket (Reverb).
     */
    public $data;

    /**
     * Konstruktor: Menerima data dari Controller
     */
    public function __construct($data)
    {
        $this->data = $data;
    }

    /**
     * Tentukan Channel (pintu) pengiriman.
     * Menggunakan Channel publik agar bisa diakses tanpa login.
     */
    public function broadcastOn(): array
    {
        return [
            new Channel('health-updates'),
        ];
    }
}
