<?php

use App\Models\DataKesehatan;
use Illuminate\Support\Facades\Route;

Route::get('/', function () {
    $latestData = DataKesehatan::latest()->first();

    return view('dashboard', [
        'latestData' => $latestData
    ]);
});
