<?php

namespace App\Filament\Resources\Penggunas\Tables;

use Filament\Actions\BulkActionGroup;
use Filament\Actions\DeleteBulkAction;
use Filament\Actions\EditAction;
use Filament\Tables\Columns\TextColumn;
use Filament\Tables\Table;

class PenggunasTable
{
    public static function configure(Table $table): Table
    {
        return $table
            ->columns([
                TextColumn::make('nama')
                    ->label('Nama Pengguna')
                    ->sortable()
                    ->width('17ch')
                    ->limit(17)
                    ->searchable(),
                TextColumn::make('email')
                    ->label('Alamat Email')
                    ->width('20ch')
                    ->limit(20)
                    ->searchable()
                    ->visibleFrom('md'),
                TextColumn::make('kata_sandi')
                    ->wrap()
                    ->width('15')
                    ->limit(15)
                    ->label('Kata Sandi')
                    ->visibleFrom('xl'),
                TextColumn::make('created_at')
                    ->label('Terdaftar Pada')
                    ->date()
                    ->sortable()
                    ->width('15ch')
                    ->limit(15)
                    ->visibleFrom('xl'),
                TextColumn::make('updated_at')
                    ->label('Diperbarui Pada')
                    ->date()
                    ->sortable()
                    ->width('15ch')
                    ->limit(15)
                    ->visibleFrom('2xl'),
                TextColumn::make('spacer')
                    ->label('')
                    ->grow(),
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
