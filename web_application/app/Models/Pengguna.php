<?php

namespace App\Models;

use Illuminate\Foundation\Auth\User as Authenticatable;
use Illuminate\Notifications\Notifiable;

class Pengguna extends Authenticatable
{
    use Notifiable;

    protected $table = 'pengguna';

    protected $fillable = [
        'nama',
        'email',
        'kata_sandi'
    ];

    /**
     * Keamanan: Sembunyikan password agar tidak bocor saat API dipanggil
     */
    protected $hidden = [
        'kata_sandi',
        'remember_token',
    ];

    /**
     * Override Laravel: Memberitahu Laravel bahwa kolom password kita bernama 'kata_sandi'
     */
    public function getAuthPassword()
    {
        return $this->kata_sandi;
    }

    /**
     * Relasi: Satu pengguna bisa memiliki banyak alat (Contoh: Jam Tangan & Alat Monitor Kasur)
     */
    public function perangkat()
    {
        return $this->hasMany(Perangkat::class, 'pengguna_id');
    }
}