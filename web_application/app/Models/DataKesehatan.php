<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Model;

class DataKesehatan extends Model
{
    protected $table = 'data_kesehatan';

    protected $fillable = [
        'pengguna_id',
        'raw_ir',
        'raw_red',
        'filtered_ir',
        'filtered_red',
        'features',
        'hr',
        'spo2',
        'sbp',
        'dbp',
        'hb'
    ];

    /**
     * Casting otomatis: Mengubah JSON di DB menjadi Array/Objek di PHP
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

    // Relasi balik: Data ini milik siapa?
    public function pengguna()
    {
        return $this->belongsTo(Pengguna::class, 'pengguna_id');
    }

    // Relasi ke device
    public function device()
    {
        return $this->belongsTo(Device::class, 'device_id');
    }
}
