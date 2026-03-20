<?php

namespace App\Filament\Resources\Perangkats\Schemas;

use Filament\Forms;
use Filament\Forms\Components\Select;
use Filament\Forms\Components\TextInput;

use Filament\Schemas\Schema;

class PerangkatForm
{
    public static function configure(Schema $schema): Schema
    {
        return $schema
            ->components([
                TextInput::make('token_perangkat')
                    ->required(),
                TextInput::make('nama_perangkat'),
                Select::make('pengguna_id')
                    ->relationship('pengguna', 'nama')
                    ->required()
                    ->preload(),
            ]);
    }
}
