<?php

namespace App\Filament\Resources\Perangkats\Tables;

use Filament\Actions\BulkActionGroup;
use Filament\Actions\DeleteBulkAction;
use Filament\Actions\EditAction;
use Filament\Tables\Columns\TextColumn;
use Filament\Tables\Table;

class PerangkatsTable
{
    public static function configure(Table $table): Table
    {
        return $table
            ->columns([
                TextColumn::make('token_perangkat')
                    ->width('16ch')
                    ->limit(16)
                    ->copyable()
                    ->searchable(),
                TextColumn::make('nama_perangkat')
                    ->width('20ch')
                    ->limit(20)
                    ->visibleFrom('md')
                    ->searchable()
                    ->sortable(),
                TextColumn::make('pengguna.nama')
                    ->label('Nama Pemilik')
                    ->width('15ch')
                    ->limit(15)
                    ->copyable()
                    ->searchable()
                    ->visibleFrom('xl')
                    ->sortable(),
                TextColumn::make('created_at')
                    ->label('Didaftarkan Pada')
                    ->width('15ch')
                    ->limit(15)
                    ->date()
                    ->visibleFrom('xl')
                    ->sortable(),
                TextColumn::make('updated_at')
                    ->label('Diperbarui Pada')
                    ->width('15ch')
                    ->limit(15)
                    ->date()
                    ->visibleFrom('2xl')
                    ->sortable(),
                TextColumn::make('spacer')
                    ->label('')
                    ->grow()
            ])
            ->filters([
                //
            ])
            ->recordActions([
                EditAction::make()
                    ->iconButton(),
            ])
            ->toolbarActions([
                BulkActionGroup::make([
                    DeleteBulkAction::make(),
                ]),
            ])
            ->recordUrl(null)
            ->recordAction(null);
    }
}
