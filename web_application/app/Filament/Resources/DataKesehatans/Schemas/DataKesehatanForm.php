<?php

namespace App\Filament\Resources\DataKesehatans\Schemas;

use Filament\Forms\Components\Textarea;
use Filament\Forms\Components\TextInput;
use Filament\Schemas\Schema;


class DataKesehatanForm
{
    public static function configure(Schema $schema): Schema
    {
        return $schema
            ->components([
                TextInput::make('pengguna_id')
                    ->required()
                    ->numeric(),
                TextInput::make('perangkat_id')
                    ->required()
                    ->numeric(),
                Textarea::make('raw_ir')
                    ->columnSpanFull(),
                Textarea::make('raw_red')
                    ->columnSpanFull(),
                Textarea::make('filtered_ir')
                    ->columnSpanFull(),
                Textarea::make('filtered_red')
                    ->columnSpanFull(),
                Textarea::make('features')
                    ->columnSpanFull(),
                TextInput::make('hr')
                    ->numeric(),
                TextInput::make('spo2')
                    ->numeric(),
                TextInput::make('sbp')
                    ->numeric(),
                TextInput::make('dbp')
                    ->numeric(),
                TextInput::make('hb')
                    ->numeric(),
            ]);
    }
}
