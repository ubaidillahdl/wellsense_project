<?php

namespace App\Http\Controllers;

use App\Models\AiInsight;
use App\Models\DataKesehatan;
use Illuminate\Support\Facades\Auth;

class DashboardController extends Controller
{
    public function index()
    {
        $userId = Auth::id();

        $latestData = DataKesehatan::where('pengguna_id', $userId)->latest()->first() ??  new DataKesehatan();
        $latestInsight = AiInsight::where('pengguna_id', $userId)->latest()->value('insight') ?? 'Tidak ada saran !';
        $diffSeconds = abs(now()->diffInSeconds($latestData->created_at));

        return view('dashboard', compact('latestData', 'latestInsight', 'diffSeconds'));
    }
}
