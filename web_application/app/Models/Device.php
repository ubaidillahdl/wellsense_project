<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Model;

class Device extends Model
{
    protected $table = 'devices';

    // Kolom yang boleh diisi saat registrasi alat (misal via Admin/Seeder)
    protected $fillable = [
        'device_token',
        'nama_device',
        'pengguna_id'
    ];

    /**
     * Relasi: Device ini milik satu pengguna
     */
    public function pengguna()
    {
        return $this->belongsTo(Pengguna::class, 'pengguna_id');
    }

    /**
     * Relasi: Satu device bisa punya banyak data kesehatan
     */
    public function dataKesehatan()
    {
        return $this->hasMany(DataKesehatan::class, 'device_id');
    }
}
