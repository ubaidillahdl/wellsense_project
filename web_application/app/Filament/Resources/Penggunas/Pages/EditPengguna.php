<?php

namespace App\Filament\Resources\Penggunas\Pages;

use App\Filament\Resources\Penggunas\PenggunaResource;
use Filament\Actions\DeleteAction;
use Filament\Resources\Pages\EditRecord;

class EditPengguna extends EditRecord
{
    protected static string $resource = PenggunaResource::class;
    protected ?string $heading = '';

    protected function getHeaderActions(): array
    {
        return [
            DeleteAction::make(),
        ];
    }
}