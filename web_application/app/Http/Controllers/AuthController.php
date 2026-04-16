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
        $request->validate([
            'email' => ['required', 'email', 'exists:pengguna,email'],
            'password' => ['required'],
        ]);

        // Cek kecocokan data ke Database (Email & Password otomatis di-hash)
        if (Auth::guard('web')->attempt($request->only('email', 'password'))) {
            // Login Berhasil: Buat ID session baru biar aman dari hacker
            $request->session()->regenerate();

            // Lempar ke halaman yang dituju sebelumnya, atau ke dashboard (/)
            return redirect()->intended('/');
        }

        // Jika password salah (tapi email benar), kita lempar error manual 
        // agar sistem 'otomatis' Laravel tetap jalan melempar balik ke form
        throw \Illuminate\Validation\ValidationException::withMessages([
            'email' => [trans('auth.failed')],
        ]);
    }


    /**
     * 3. Proses Logout
     */
    public function logout(Request $request)
    {
        Auth::guard('web')->logout();

        // $request->session()->invalidate(); // Hancurkan data session
        // $request->session()->forget(Auth::guard('web')->getName());
        $request->session()->forget('login_web_' . sha1(\Illuminate\Auth\SessionGuard::class));
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
            'nama' => ['required', 'string', 'max:100'],
            'email' => ['required', 'string', 'email', 'max:100', 'unique:pengguna'],
            'password' => ['required', 'min:8', 'confirmed'], // 'confirmed' butuh input 'password_confirmation'
        ]);

        // Masukkan data ke Database via Model Pengguna
        $user = Pengguna::create([
            'nama' => $request->nama,
            'email' => $request->email,
            'kata_sandi' => Hash::make($request->password),
        ]);

        // Setelah daftar, otomatis buat user langsung berstatus "Logged In"
        Auth::guard('web')->login($user);

        // Lempar ke dashboard dengan pesan sukses sementara (Flash Session)
        return redirect('/');
    }
}