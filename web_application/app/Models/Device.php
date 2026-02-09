<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Model;

class Device extends Model
{
    // Tambahkan ini jika nama tabelnya jamak tapi modelnya tunggal 
    // (Sebenarnya Laravel sudah otomatis, tapi lebih aman jika didefinisikan)
    protected $table = 'devices';

    protected $fillable = ['device_token', 'pengguna_id'];

    // --- TAMBAHAN: Relasi balik ke Pengguna ---
    // Gunanya: Supaya Abang bisa panggil $device->pengguna->nama
    public function pengguna()
    {
        return $this->belongsTo(Pengguna::class, 'pengguna_id');
    }
}
