<?php

namespace App\Filament\Resources\DataKesehatans\Pages;

use App\Filament\Resources\DataKesehatans\DataKesehatanResource;
use Filament\Resources\Pages\ViewRecord;
use Filament\Schemas\Schema;
// use Illuminate\Database\Eloquent\Model;

class ViewDataKesehatan extends ViewRecord
{
    protected static string $resource = DataKesehatanResource::class;
    protected ?string $heading = '';

    protected function getHeaderActions(): array
    {
        return [
            // EditAction::make(),
        ];
    }

    public function infolist(Schema $schema): Schema
    {
        return \App\Filament\Resources\DataKesehatans\Schemas\DataKesehatanInfolist::configure(
            $schema->record($this->record)
        );
    }
}