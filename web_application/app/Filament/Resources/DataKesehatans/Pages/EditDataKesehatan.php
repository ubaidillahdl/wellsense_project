<?php

namespace App\Filament\Resources\DataKesehatans\Pages;

use App\Filament\Resources\DataKesehatans\DataKesehatanResource;
use Filament\Actions\DeleteAction;
use Filament\Resources\Pages\EditRecord;

class EditDataKesehatan extends EditRecord
{
    protected static string $resource = DataKesehatanResource::class;

    protected function getHeaderActions(): array
    {
        return [
            DeleteAction::make(),
        ];
    }
}