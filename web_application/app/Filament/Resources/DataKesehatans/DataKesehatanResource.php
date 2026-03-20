<?php

namespace App\Filament\Resources\DataKesehatans;

use App\Filament\Resources\DataKesehatans\Pages\CreateDataKesehatan;
use App\Filament\Resources\DataKesehatans\Pages\EditDataKesehatan;
use App\Filament\Resources\DataKesehatans\Pages\ListDataKesehatans;
use App\Filament\Resources\DataKesehatans\Schemas\DataKesehatanForm;
use App\Filament\Resources\DataKesehatans\Tables\DataKesehatansTable;
use App\Models\DataKesehatan;
use BackedEnum;
use Filament\Resources\Resource;
use Filament\Schemas\Schema;
use Filament\Support\Icons\Heroicon;
use Filament\Tables\Table;

class DataKesehatanResource extends Resource
{
    protected static ?string $model = DataKesehatan::class;

    protected static string|BackedEnum|null $navigationIcon = Heroicon::OutlinedRectangleStack;

    protected static ?string $recordTitleAttribute = 'data_kesehatan';
    protected static ?string $modelLabel = 'Data Kesehatan';
    protected static ?string $pluralModelLabel = 'Data Kesehatan';
    protected static ?string $navigationLabel = 'Data Kesehatan';
    protected static ?string $slug = 'data-kesehatan';
    protected static ?int $navigationSort = 3;

    public static function form(Schema $schema): Schema
    {
        return DataKesehatanForm::configure($schema);
    }

    public static function table(Table $table): Table
    {
        return DataKesehatansTable::configure($table);
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
            'index' => ListDataKesehatans::route('/'),
            'create' => CreateDataKesehatan::route('/create'),
            'edit' => EditDataKesehatan::route('/{record}/edit'),
        ];
    }
}
