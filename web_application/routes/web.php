<?php

use App\Http\Controllers\AuthController;
use App\Http\Controllers\DashboardController;
use Illuminate\Support\Facades\Route;

/*
Pengaturan Akses Tamu (Belum Login)
*/

Route::middleware(['guest'])->group(function () {

    // Login: Tampilkan form & Proses verifikasi
    Route::get('/login', [AuthController::class, 'showLogin'])->name('login');
    Route::post('/login', [AuthController::class, 'login']);

    // Register: Tampilkan form & Simpan user baru
    Route::get('/register', [AuthController::class, 'showRegister'])->name('register');
    Route::post('/register', [AuthController::class, 'register']);
});


/*
Pengaturan Akses Pengguna (Sudah Login)
*/
Route::middleware(['auth'])->group(function () {
    // Halaman Utama: Ambil data sensor terakhir untuk ditampilkan di dashboard
    Route::get('/', [DashboardController::class, 'index'])->name('dashboard');

    // Proses keluar sistem & hapus session
    Route::post('/logout', [AuthController::class, 'logout'])->name('logout');
});
