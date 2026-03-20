<?php

namespace App\Filament\Resources\Perangkats;

use App\Filament\Resources\Perangkats\Pages\CreatePerangkat;
use App\Filament\Resources\Perangkats\Pages\EditPerangkat;
use App\Filament\Resources\Perangkats\Pages\ListPerangkats;
use App\Filament\Resources\Perangkats\Schemas\PerangkatForm;
use App\Filament\Resources\Perangkats\Tables\PerangkatsTable;
use App\Models\Perangkat;
use BackedEnum;
use Filament\Resources\Resource;
use Filament\Schemas\Schema;
use Filament\Tables\Table;

class PerangkatResource extends Resource
{
    protected static ?string $model = Perangkat::class;

    protected static string|BackedEnum|null $navigationIcon = 'heroicon-o-device-tablet';

    protected static ?string $recordTitleAttribute = 'nama_perangkat';
    protected static ?string $modelLabel = 'Perangkat';
    protected static ?string $pluralModelLabel = 'Perangkat';
    protected static ?string $navigationLabel = 'Perangkat';
    protected static ?string $slug = 'perangkat';
    protected static ?int $navigationSort = 2;

    public static function form(Schema $schema): Schema
    {
        return PerangkatForm::configure($schema);
    }

    public static function table(Table $table): Table
    {
        return PerangkatsTable::configure($table);
    }

    public static function getRelations(): array
    {
        return [
            //
        ];
    }

    public static function getPages(): array
    {
        return [
            'index' => ListPerangkats::route('/'),
            'create' => CreatePerangkat::route('/create'),
            'edit' => EditPerangkat::route('/{record}/edit'),
        ];
    }
}
