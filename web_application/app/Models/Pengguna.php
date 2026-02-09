<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Model;

class Pengguna extends Model
{
    protected $table = 'pengguna';

    protected $fillable = [
        'nama',
        'email',
        'kata_sandi'
    ];

    // Sembunyikan kata_sandi dari output JSON/Array
    protected $hidden = [
        'kata_sandi'
    ];

    // Relasi ke Device (Sangat penting untuk tracking jam tangan)
    public function devices()
    {
        return $this->hasMany(Device::class, 'pengguna_id');
    }
}
