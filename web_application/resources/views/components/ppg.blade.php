<section class="flex-1 flex items-center justify-center">
    <div
        class="bg-gray-800 h-[calc(100%-2vh)] w-[calc(100%-2vh)] sm:rounded-[3vh] rounded-[4vh] flex flex-col justify-center items-center">
        <div class="w-[calc(100%-3vh)] h-[calc(100%-3vh)] flex flex-col">

            {{-- Label PPG --}}
            <div class="h-[3vh] flex text-gray-400 justify-between items-center">
                <div class="text-[1.9vh] sm:text-[1.5vh] font-semibold flex items-center">
                    <x-circle-icon class="bg-green-500">
                        <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24"
                            fill="none" stroke="currentColor" stroke-width="3" stroke-linecap="round"
                            stroke-linejoin="round" class="feather feather-activity h-[calc(100%-1vh)] text-white">
                            <polyline points="22 12 18 12 15 21 9 3 6 12 2 12" />
                        </svg>
                    </x-circle-icon>
                    PPG
                </div>

                {{-- Status Pembaruan --}}
                <div class="text-[1.5vh] sm:text-[1.25vh] opacity-50 pe-[2vh]" x-data x-init="$store.timer.setAwal({{ $diffSeconds }})">
                    Diperbarui <span x-text="$store.timer.formatTeks"></span>
                </div>

            </div>

            {{-- Value PPG --}}
            <div class="bg-gray-600 flex-1 mt-[0.8vh] sm:rounded-[2.5vh] rounded-[3vh] overflow-hidden">
                <div x-data="ppgChart({{ json_encode($latestData->filtered_ir) }})" class="w-full h-full">
                    <canvas id="ppgCanvas"></canvas>
                </div>
            </div>

        </div>
    </div>
</section>
