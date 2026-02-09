<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Model;

class DataKesehatan extends Model
{
    protected $table = 'data_kesehatan';

    protected $fillable = [
        'pengguna_id',
        'filtered_ir',
        'filtered_red',
        'hr',
        'spo2',
        'sbp',
        'dbp',
        'hb'
    ];

    // Relasi balik: Data ini milik siapa?
    public function pengguna()
    {
        return $this->belongsTo(Pengguna::class, 'pengguna_id');
    }
}
