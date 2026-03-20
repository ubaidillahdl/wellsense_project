<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Model;

class DataKesehatan extends Model
{
    protected $table = 'data_kesehatan';

    /**
     * Konversi Otomatis: JSON DB <-> Array PHP
     */
    protected $casts = [
        'raw_ir'          => 'array',
        'raw_red'         => 'array',
        'filtered_ir'     => 'array',
        'filtered_red'    => 'array',
        'features' => 'array',
        'hr'              => 'float',
        'spo2'            => 'float',
        'sbp'             => 'float',
        'dbp'             => 'float',
        'hb'              => 'float',
    ];

    /**
     * Relasi: Data ini milik siapa?
     */
    public function pengguna()
    {
        return $this->belongsTo(Pengguna::class, 'pengguna_id');
    }

    /**
     * Relasi: Data ini dikirim dari alat mana?
     */
    public function perangkat()
    {
        return $this->belongsTo(Perangkat::class, 'perangkat_id');
    }
}
