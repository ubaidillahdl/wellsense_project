<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Model;

class Device extends Model
{
    protected $table = 'devices';

    protected $fillable = [
        'device_token',
        'nama_device',
        'pengguna_id'
    ];

    // Relasi: Device ini milik siapa?
    public function pengguna()
    {
        return $this->belongsTo(Pengguna::class, 'pengguna_id');
    }

    // Relasi: Device ini sudah menghasilkan data apa saja?
    public function dataKesehatan()
    {
        return $this->hasMany(DataKesehatan::class, 'device_id');
    }
}
