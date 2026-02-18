<section class=" flex-[0.95] flex justify-center items-center">
    <div class="bg-gray-800 h-[calc(100%-2vh)]  w-[calc(100%-2vh)] sm:rounded-[3vh] rounded-[4vh] flex justify-center items-center @container-[size]"
        x-data="{
            hr: {{ round($latestData->hr) }},
            spo2: {{ round($latestData->spo2) }},
        }"
        @vitals-updated.window="
                hr = Math.round($event.detail.vitals.hr);
                spo2 = Math.round($event.detail.vitals.spo2);">

        <div class="w-[calc(100%-3vh)] h-[calc(100%-3vh)] flex flex-col">
            <div class="h-[3vh] text-[1.9vh] sm:text-[1.5vh] font-semibold flex items-center text-gray-400">

                {{-- Label HR --}}
                <div class="flex-1 h-[3vh] text-[1.9vh] sm:text-[1.5vh] font-semibold flex items-center text-gray-400">
                    <x-circle-icon class="bg-gray-200 text-gray-800">
                        <svg class="h-[calc(100%-1vh)]" aria-hidden="true" xmlns="http://www.w3.org/2000/svg"
                            width="24" height="24" fill="none" viewBox="0 0 24 24">
                            <path stroke="currentColor" stroke-linecap="round" stroke-linejoin="round" stroke-width="3"
                                d="M12.01 6.001C6.5 1 1 8 5.782 13.001L12.011 20l6.23-7C23 8 17.5 1 12.01 6.002Z" />
                        </svg>
                    </x-circle-icon>
                    <span>HR <span class=" text-[1.5vh] sm:text-[1.25vh] opacity-50">(bpm)</span></span>
                </div>

                {{-- Label HR --}}
                <div class="flex-1 h-[3vh] text-[1.9vh] sm:text-[1.5vh] font-semibold flex items-center text-gray-400">
                    <x-circle-icon class="bg-gray-200 text-gray-800">
                        <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24"
                            fill="none" stroke="currentColor" stroke-width="3" stroke-linecap="round"
                            stroke-linejoin="round" class="feather feather-droplet h-[calc(100%-1vh)]">
                            <path d="M12 2.69l5.66 5.66a8 8 0 1 1-11.31 0z" />
                        </svg>
                    </x-circle-icon>
                    <span>SPO2 <span class=" text-[1.5vh] sm:text-[1.25vh] opacity-50">(%)</span></span>
                </div>

            </div>

            <div
                class="h-[calc(100%-20cqh)] sm:h-[calc(100%-25cqh)] flex-1 mt-[0.8vh] sm:rounded-[2.5vh] rounded-[3vh] flex gap-[2vh">

                {{-- Value HR --}}
                <div class=" flex-1 flex items-center @container-[size]">
                    <div class="flex-1 flex justify-center h-full">
                        <span class="text-[70cqh] sm:text-[75cqh]" x-text="hr"></span>
                    </div>
                </div>

                {{-- Value Spo2 --}}
                <div class="flex-1 flex items-center @container-[size]">
                    <div class="flex-1 flex justify-center h-full">
                        <span class="text-[70cqh] sm:text-[75cqh] " x-text="spo2"></span>
                    </div>
                </div>
            </div>

        </div>
</section>
