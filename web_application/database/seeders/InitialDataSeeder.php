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
        Admin::create([
            'nama' => 'WellSense SuperAdmin',
            'email' => 'admin@wellsense.io',
            'kata_sandi' => Hash::make('wellsense')
        ]);

        $users = collect([
            ['nama' => 'Budi Santoso', 'email' => 'budi.santoso@gmail.com'],
            ['nama' => 'Siti Aminah', 'email' => 'siti.aminah@yahoo.com'],
            ['nama' => 'Andi Wijaya', 'email' => 'andi.wijaya@outlook.com'],
            ['nama' => 'Dewi Lestari', 'email' => 'dewi.lestari@gmail.com'],
            ['nama' => 'Rizky Pratama', 'email' => 'rizky.pratama@wellsense.io'],
        ])->map(fn($u) => Pengguna::create([
            'nama' => $u['nama'],
            'email' => $u['email'],
            'kata_sandi' => Hash::make('wellsense')
        ]));

        $assignments = [
            ['user_idx' => 0, 'name' => 'WellSense Home'],
            ['user_idx' => 0, 'name' => 'WellSense Office'],
            ['user_idx' => 1, 'name' => 'Personal Tracker'],
            ['user_idx' => 1, 'name' => 'Elderly Monitor'],
            ['user_idx' => 2, 'name' => 'WellSense Pro'],
            ['user_idx' => 2, 'name' => 'Backup Device'],
            ['user_idx' => 3, 'name' => 'Lestari Care'],
            ['user_idx' => 4, 'name' => 'Rizky Watch'],
        ];

        foreach ($assignments as $task) {
            Perangkat::create([
                'pengguna_id' => $users[$task['user_idx']]->id,
                'token_perangkat' => 'WS-' . str_pad(rand(0, 99999999999), 15, '0', STR_PAD_LEFT),
                'nama_perangkat' => $task['name']
            ]);
        }
        $this->command->info('Data Pengguna (Admin & User Kosong) serta Device berhasil dibuat!');
    }
}