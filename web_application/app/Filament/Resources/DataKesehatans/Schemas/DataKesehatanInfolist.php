<?php

namespace App\Filament\Resources\DataKesehatans\Schemas;

use Filament\Infolists\Components\TextEntry;
use Filament\Schemas\Schema;

class DataKesehatanInfolist
{
    public static function configure(Schema $schema): Schema
    {
        return $schema
            ->components([
                TextEntry::make('pengguna_id')
                    ->numeric(),
                TextEntry::make('perangkat_id')
                    ->numeric(),
                TextEntry::make('raw_ir')
                    ->placeholder('-')
                    ->columnSpanFull(),
                TextEntry::make('raw_red')
                    ->placeholder('-')
                    ->columnSpanFull(),
                TextEntry::make('filtered_ir')
                    ->placeholder('-')
                    ->columnSpanFull(),
                TextEntry::make('filtered_red')
                    ->placeholder('-')
                    ->columnSpanFull(),
                TextEntry::make('features')
                    ->placeholder('-')
                    ->columnSpanFull(),
                TextEntry::make('hr')
                    ->numeric()
                    ->placeholder('-'),
                TextEntry::make('spo2')
                    ->numeric()
                    ->placeholder('-'),
                TextEntry::make('sbp')
                    ->numeric()
                    ->placeholder('-'),
                TextEntry::make('dbp')
                    ->numeric()
                    ->placeholder('-'),
                TextEntry::make('hb')
                    ->numeric()
                    ->placeholder('-'),
                TextEntry::make('created_at')
                    ->dateTime()
                    ->placeholder('-'),
                TextEntry::make('updated_at')
                    ->dateTime()
                    ->placeholder('-'),
            ]);
    }
}
