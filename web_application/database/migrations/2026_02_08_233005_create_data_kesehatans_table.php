<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

return new class extends Migration
{
    /**
     * Run the migrations.
     */
    public function up(): void
    {
        Schema::disableForeignKeyConstraints();

        Schema::create('data_kesehatan', function (Blueprint $table) {
            $table->id();

            $table->foreignId('pengguna_id')->constrained('pengguna')->onDelete('cascade');
            $table->foreignId('device_id')->constrained('devices')->onDelete('cascade');

            $table->longText('filtered_ir')->nullable();
            $table->longText('filtered_red')->nullable();

            $table->decimal('hr', 8, 4)->nullable();
            $table->decimal('spo2', 8, 4)->nullable();
            $table->decimal('sbp', 8, 4)->nullable();
            $table->decimal('dbp', 8, 4)->nullable();
            $table->decimal('hb', 8, 4)->nullable();

            $table->timestamps();
        });
    }

    /**
     * Reverse the migrations.
     */
    public function down(): void
    {
        Schema::dropIfExists('data_kesehatan');
    }
};
