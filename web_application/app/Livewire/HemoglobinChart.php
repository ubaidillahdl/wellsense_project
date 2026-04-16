<?php

namespace App\Livewire;

use App\Models\DataKesehatan;
use Filament\Widgets\ChartWidget;

class HemoglobinChart extends ChartWidget
{
    protected ?string $heading = null;
    protected static bool $isLazy = false;
    protected ?string $maxHeight = '10rem';
    public $record;

    protected ?array $options = [
        'plugins' => [
            'legend' => [
                'display' => false,
            ],
        ],
    ];

    protected function getData(): array
    {
        $data = DataKesehatan::where('pengguna_id', $this->record->pengguna_id)
            ->latest()
            ->take(10)
            ->get()
            ->reverse();

        return [
            'datasets' => [
                [
                    'data' => $data->pluck('hb')->toArray(),
                    'tension' => 0.3,
                ],
            ],
            'labels' => $data->pluck('created_at')->map(fn($d) => $d->format('H:i'))->toArray(),
        ];
    }

    protected function getOptions(): array
    {
        return [
            'scales' => [
                'x' => [
                    'display' => true,
                    'grid' => [
                        'display' => true,
                        'color' => 'rgba(255, 255, 255, 0.05)',
                    ],
                ],
                'y' => [
                    'display' => true,
                    'grid' => [
                        'display' => true,
                        'color' => 'rgba(255, 255, 255, 0.05)',
                    ],
                ],
            ],
            'plugins' => [
                'legend' => [
                    'display' => false,
                ],
            ],
        ];
    }

    protected function getType(): string
    {
        return 'line';
    }
}