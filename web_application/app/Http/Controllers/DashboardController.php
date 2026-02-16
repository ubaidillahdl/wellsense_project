<?php

namespace App\Http\Controllers;

use App\Models\DataKesehatan;
use Illuminate\Support\Facades\Auth;

class DashboardController extends Controller
{
    /**
     * Tampilkan Halaman Utama Dashboard
     */
    public function index()
    {
        // Ambil ID pengguna yang sedang login
        $userId = Auth::id();

        // 1. Cari data terbaru yang 'pengguna_id'-nya cocok dengan user yang login
        // 2. Jika tidak ada data (null), buat objek kosong 'new DataKesehatan' agar Blade tidak error
        $latestData = DataKesehatan::where('pengguna_id', $userId)->latest()->first()
            ?? new DataKesehatan();

        // 3. Kirim data ke View (resources/views/dashboard.blade.php)
        return view('dashboard', compact('latestData'));
    }
}
