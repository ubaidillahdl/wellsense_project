<?php

namespace App\Filament\Resources\Penggunas\Schemas;

use Filament\Forms\Components\TextInput;
use Filament\Schemas\Schema;

class PenggunaForm
{
    public static function configure(Schema $schema): Schema
    {
        return $schema
            ->components([
                TextInput::make('nama')
                    ->required(),
                TextInput::make('email')
                    ->label('Alamat Email')
                    ->email()
                    ->required(),
                TextInput::make('kata_sandi')
                    ->label('Kata Sandi')
                    ->required(),
            ]);
    }
}
