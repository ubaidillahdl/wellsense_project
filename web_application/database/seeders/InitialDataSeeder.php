<?php

namespace Database\Seeders;

use App\Models\Device;
use App\Models\Pengguna;
use Illuminate\Database\Seeder;
use Illuminate\Support\Facades\Hash;

class InitialDataSeeder extends Seeder
{
    /**
     * Run the database seeds.
     */
    public function run(): void
    {
        // 1. Buat Pengguna Admin (Sudah punya alat)
        $admin = Pengguna::create([
            'nama' => 'Admin Wellsense',
            'email' => 'admin@wellsense.io',
            'kata_sandi' => Hash::make('wellsense')
        ]);

        // Hubungkan alat ke Admin
        Device::create([
            'pengguna_id' => $admin->id,
            'device_token' => 'WS-866501012348821',
            'nama_device' => 'Wellsense Care'
        ]);

        // 2. Buat Pengguna Biasa (Tanpa alat / Masih kosong)
        Pengguna::create([
            'nama' => 'User Testing',
            'email' => 'user@wellsense.io',
            'kata_sandi' => Hash::make('wellsense')
        ]);

        $this->command->info('Data Pengguna (Admin & User Kosong) serta Device berhasil dibuat!');
    }
}
