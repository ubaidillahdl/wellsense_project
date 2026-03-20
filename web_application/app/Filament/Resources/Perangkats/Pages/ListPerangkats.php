<?php

namespace App\Filament\Resources\Perangkats\Pages;

use App\Filament\Resources\Perangkats\PerangkatResource;
use Filament\Actions\CreateAction;
use Filament\Resources\Pages\ListRecords;

class ListPerangkats extends ListRecords
{
    protected static string $resource = PerangkatResource::class;

    protected function getHeaderActions(): array
    {
        return [
            CreateAction::make(),
        ];
    }
}
