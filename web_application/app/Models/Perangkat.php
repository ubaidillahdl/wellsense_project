<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Model;

class Perangkat extends Model
{
    protected $table = 'perangkat';


    // Kolom yang boleh diisi saat registrasi alat (misal via Admin/Seeder)
    protected $fillable = [
        'token_perangkat',
        'nama_perangkat',
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
        return $this->hasMany(DataKesehatan::class, 'perangkat_id');
    }
}
