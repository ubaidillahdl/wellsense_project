<?php

namespace Database\Seeders;

use App\Models\Admin;
use App\Models\Pengguna;
use App\Models\Perangkat;
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
            'nama' => 'WS01 Client',
            'email' => 'ws01client@wellsense.io',
            'kata_sandi' => Hash::make('wellsense')
        ]);

        // Hubungkan alat ke Admin
        Perangkat::create([
            'pengguna_id' => $admin->id,
            'token_perangkat' => 'WS-866501012348821',
            'nama_perangkat' => 'Wellsense Care'
        ]);

        // 2. Buat Pengguna Biasa (Tanpa alat / Masih kosong)
        Pengguna::create([
            'nama' => 'WS02 Client',
            'email' => 'ws02client@wellsense.io',
            'kata_sandi' => Hash::make('wellsense')
        ]);

        Admin::create([
            'nama' => 'WS01 Admin',
            'email' => 'ws01admin@wellsense.io',
            'kata_sandi' => Hash::make('wellsense')
        ]);

        $this->command->info('Data Pengguna (Admin & User Kosong) serta Device berhasil dibuat!');
    }
}
