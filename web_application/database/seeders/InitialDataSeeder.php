<?php

namespace Database\Seeders;

use App\Models\Device;
use App\Models\Pengguna;
// use Illuminate\Database\Console\Seeds\WithoutModelEvents;
use Illuminate\Database\Seeder;
use Illuminate\Support\Facades\Hash;

class InitialDataSeeder extends Seeder
{
    /**
     * Run the database seeds.
     */
    public function run(): void
    {
        $user = Pengguna::create([
            'nama' => 'Abang Developer',
            'email' => 'admin@wellsense.com',
            'kata_sandi' => Hash::make('password123')
        ]);

        Device::create([
            'pengguna_id' => $user->id,
            'device_token' => 'WS-01-PROTOTYPE',
            'nama_device' => 'Sensor Prototipe 1'
        ]);

        $this->command->info('Data Pengguna dan Device berhasil dibuat!');
    }
}
