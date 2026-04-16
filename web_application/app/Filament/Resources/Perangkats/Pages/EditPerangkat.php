<?php

namespace App\Filament\Resources\Perangkats\Pages;

use App\Filament\Resources\Perangkats\PerangkatResource;
use Filament\Actions\DeleteAction;
use Filament\Resources\Pages\EditRecord;

class EditPerangkat extends EditRecord
{
    protected static string $resource = PerangkatResource::class;
    protected ?string $heading = '';

    protected function getHeaderActions(): array
    {
        return [
            DeleteAction::make(),
        ];
    }
}