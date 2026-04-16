<?php

namespace App\Filament\Resources\DataKesehatans\Schemas;

use Filament\Infolists\Components\TextEntry;
use Filament\Schemas\Components\Grid;
use Filament\Schemas\Components\Livewire;
use Filament\Schemas\Components\Section;
use Filament\Schemas\Schema;

class DataKesehatanInfolist
{
    public static function configure(Schema $schema): Schema
    {
        return $schema
            ->components([
                Livewire::make(\App\Livewire\PhotoplethysmographyChart::class)
                    ->columnSpanFull(),
                Grid::make()
                    ->columns([
                        'default' => 1,
                        'sm' => 2,
                        'xl' => 3,
                        '2xl' => 4,
                    ])
                    ->schema([
                        Section::make()
                            ->schema([
                                TextEntry::make('hr')
                                    ->label('Heart Rate (HR)')
                                    ->numeric(
                                        decimalPlaces: 2
                                    )
                                    ->suffix(' bpm')
                                    ->weight('bold'),
                                Livewire::make(\App\Livewire\HeartRateChart::class),
                            ]),
                        Section::make()
                            ->schema([
                                TextEntry::make('spo2')
                                    ->label('Oxygen Saturation (SpO2)')
                                    ->numeric(
                                        decimalPlaces: 2
                                    )
                                    ->suffix(' %')
                                    ->weight('bold'),
                                Livewire::make(\App\Livewire\OxygenSaturationChart::class),
                            ]),
                        Section::make()
                            ->schema([
                                TextEntry::make('sbp')
                                    ->label('Systolic Blood Pressure (SBP)')
                                    ->numeric(
                                        decimalPlaces: 4
                                    )
                                    ->suffix(' mmHg')
                                    ->weight('bold'),
                                Livewire::make(\App\Livewire\SystolicBloodPressureChart::class),
                            ]),
                        Section::make()
                            ->schema([
                                TextEntry::make('dbp')
                                    ->label('Diastolic Blood Pressure (DBP)')
                                    ->numeric(
                                        decimalPlaces: 4
                                    )
                                    ->suffix(' mmHg')
                                    ->weight('bold'),
                                Livewire::make(\App\Livewire\DiastolicBloodPressureChart::class),
                            ]),
                        Section::make()
                            ->schema([
                                TextEntry::make('hb')
                                    ->label('Hemoglobin (Hb)')
                                    ->numeric(
                                        decimalPlaces: 4
                                    )
                                    ->suffix(' g/L')
                                    ->weight('bold'),
                                Livewire::make(\App\Livewire\HemoglobinChart::class),
                            ]),
                    ])->columnSpanFull(),

            ]);
    }
}