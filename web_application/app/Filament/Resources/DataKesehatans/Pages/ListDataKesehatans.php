<?php

namespace App\Filament\Resources\DataKesehatans\Pages;

use App\Filament\Resources\DataKesehatans\DataKesehatanResource;
// use Filament\Actions\CreateAction;
use Filament\Resources\Pages\ListRecords;

class ListDataKesehatans extends ListRecords
{
    protected static string $resource = DataKesehatanResource::class;

    protected function getHeaderActions(): array
    {
        return [
            // CreateAction::make(),
        ];
    }
}
