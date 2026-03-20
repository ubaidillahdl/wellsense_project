<?php

namespace App\Models;

use Filament\Models\Contracts\FilamentUser;
use Filament\Models\Contracts\HasName;
use Filament\Panel;
use Illuminate\Foundation\Auth\User as Authenticatable;
use Illuminate\Notifications\Notifiable;

class Admin extends Authenticatable implements FilamentUser, HasName
{
    use Notifiable;

    protected $table = 'admin';

    protected $fillable = [
        'nama',
        'email',
        'kata_sandi'
    ];

    protected $hidden = [
        'kata_sandi',
        'remember_token',
    ];

    public function getAuthPassword()
    {
        return $this->kata_sandi;
    }

    public function canAccessPanel(Panel $panel): bool
    {
        return true; // Semua data di tabel admins bisa login
    }

    public function getFilamentName(): string
    {
        return $this->nama;
    }
}
