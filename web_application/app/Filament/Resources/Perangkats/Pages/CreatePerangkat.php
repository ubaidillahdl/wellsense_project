<?php

namespace App\Filament\Resources\Perangkats\Pages;

use App\Filament\Resources\Perangkats\PerangkatResource;
use Filament\Resources\Pages\CreateRecord;

class CreatePerangkat extends CreateRecord
{
    protected static string $resource = PerangkatResource::class;
    protected ?string $heading = '';
}