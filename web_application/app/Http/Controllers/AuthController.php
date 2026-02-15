<?php

namespace App\Http\Controllers;

use App\Models\Pengguna;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\Auth;
use Illuminate\Support\Facades\Hash;

class AuthController extends Controller
{
    /**
     * 1. Tampilkan Form Login
     */
    public function showLogin()
    {
        // Mengarah ke file: resources/views/auth/login.blade.php
        return view('auth.login');
    }


    /**
     * 2. Proses Verifikasi Login (POST)
     */
    public function login(Request $request)
    {
        // Validasi format input
        $credentials = $request->validate([
            'email' => ['required', 'email'],
            'password' => ['required'],
        ]);

        // Cek kecocokan data ke Database (Email & Password otomatis di-hash)
        if (Auth::attempt($credentials)) {
            // Login Berhasil: Buat ID session baru biar aman dari hacker
            $request->session()->regenerate();

            // Lempar ke halaman yang dituju sebelumnya, atau ke dashboard (/)
            return redirect()->intended('/');
        }

        // Login Gagal: Balik ke form login dengan pesan error
        return back()->withErrors([
            'email' => 'Email atau password salah',
        ])->onlyInput('email'); // Tetap tampilkan email yang tadi diketik (biar gak ngetik ulang)
    }


    /**
     * 3. Proses Logout
     */
    public function logout(Request $request)
    {
        // Hapus status login di sistem Laravel
        Auth::logout();

        $request->session()->invalidate(); // Hancurkan data session
        $request->session()->regenerateToken(); // Ganti token CSRF baru (lebih aman)

        // Tendang balik ke halaman login
        return redirect('/login');
    }


    /**
     * 4. Tampilkan Halaman Register
     */
    public function showRegister()
    {
        return view('auth.register');
    }


    /**
     * 5. Proses Pendaftaran User Baru
     */
    public function register(Request $request)
    {
        // Validasi: email harus unik di tabel 'pengguna'
        $request->validate([
            'nama' => ['required', 'string', 'max:255'],
            'email' => ['required', 'string', 'email', 'max:255', 'unique:pengguna'],
            'password' => ['required', 'min:6', 'confirmed'], // 'confirmed' butuh input 'password_confirmation'
        ]);

        // Masukkan data ke Database via Model Pengguna
        $user = Pengguna::create([
            'nama' => $request->nama,
            'email' => $request->email,
            'kata_sandi' => Hash::make($request->password),
        ]);

        // Setelah daftar, otomatis buat user langsung berstatus "Logged In"
        Auth::login($user);

        // Lempar ke dashboard dengan pesan sukses sementara (Flash Session)
        return redirect('/')->with('success', 'Pendaftaran berhasil!');
    }
}
