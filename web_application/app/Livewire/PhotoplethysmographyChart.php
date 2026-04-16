<?php

namespace App\Livewire;

use Filament\Widgets\ChartWidget;

class PhotoplethysmographyChart extends ChartWidget
{
    protected ?string $heading = 'Photoplethysmography (PPG)';
    protected static bool $isLazy = false;
    protected ?string $maxHeight = '10rem';
    public $record;

    protected function getData(): array
    {
        $sinyal = $this->record->filtered_ir ?? [];
        $interval = 0.02;
        return [
            'datasets' => [
                [
                    'data' => $sinyal,
                    'tension' => 0.3,
                    'pointRadius' => 0,
                    'pointHoverRadius' => 0,
                    'pointHitRadius' => 0,
                ],
            ],
            'labels' => collect($sinyal)->map(function ($value, $index) use ($interval) {
                return number_format($index * $interval, 2) . 's';
            })->toArray(),
        ];
    }

    protected function getOptions(): array
    {
        return [
            'scales' => [
                'x' => [
                    'display' => true,
                    'ticks' => [
                        'maxTicksLimit' => 6,
                    ],
                    'grid' => [
                        'display' => true,
                        'color' => 'rgba(255, 255, 255, 0.05)',
                    ],
                ],
                'y' => [
                    'display' => true,
                    'ticks' => [
                        'maxTicksLimit' => 3,
                    ],
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