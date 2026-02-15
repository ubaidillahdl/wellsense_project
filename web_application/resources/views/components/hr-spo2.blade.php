<section class=" flex-[0.8] flex items-center justify-center">
    <div class=" h-[calc(100%-2vh)]  w-[calc(100%-2vh)] sm:rounded-[3vh] rounded-[4vh] flex gap-[2vh]"
        x-data="{
            hr: {{ round($latestData->hr) }},
            spo2: {{ round($latestData->spo2) }},
        }"
        @vitals-updated.window="
        if ($event.detail.vitals.hr) {
            hr = Math.round($event.detail.vitals.hr);
        }
        if ($event.detail.vitals.spo2) {
            spo2 = Math.round($event.detail.vitals.spo2);
        }
        ">

        <div class="bg-gray-800 flex-1/2 sm:rounded-[3vh] rounded-[4vh] flex justify-center items-center">

            <div class="w-[calc(100%-3vh)] h-[calc(100%-3vh)] flex flex-col">

                <div class="h-[3vh] text-[1.9vh] sm:text-[1.5vh] font-semibold flex items-center text-gray-400 ">
                    <x-circle-icon class='bg-fuchsia-500'>
                        <svg class="h-[calc(100%-1vh)] text-white dark:text-white" aria-hidden="true"
                            xmlns="http://www.w3.org/2000/svg" width="24" height="24" fill="none"
                            viewBox="0 0 24 24">
                            <path stroke="currentColor" stroke-linecap="round" stroke-linejoin="round" stroke-width="3"
                                d="M12.01 6.001C6.5 1 1 8 5.782 13.001L12.011 20l6.23-7C23 8 17.5 1 12.01 6.002Z" />
                        </svg>
                    </x-circle-icon>
                    HR
                </div>

                <div
                    class="mt-[1.6vh] sm:mt-[0.7vh] flex-1 sm:rounded-[2.5vh] rounded-[3vh] flex justify-between items-baseline">
                    <span class="flex-[2.5] text-[8.5vh] sm:text-[7.1vh] sm:leading-[7.1vh] leading-[8.5vh] text-end"
                        x-text="hr"></span>
                    <span
                        class="flex-1 text-[1.9vh] sm:text-[1.5vh]  leading-[1.9vh] sm:leading-[1.5vh] text-gray-400">bpm</span>
                </div>

            </div>
        </div>

        <div class="bg-gray-800 flex-1/2 sm:rounded-[3vh] rounded-[4vh] flex items-center justify-center">

            <div class="w-[calc(100%-3vh)] h-[calc(100%-3vh)] flex flex-col">
                <div class="h-[3vh] text-[1.9vh] sm:text-[1.5vh] font-semibold flex items-center text-gray-400">
                    <x-circle-icon class="bg-cyan-500">
                        <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24"
                            fill="none" stroke="currentColor" stroke-width="3" stroke-linecap="round"
                            stroke-linejoin="round"
                            class="feather feather-droplet h-[calc(100%-1vh)] dark:text-white text-white">
                            <path d="M12 2.69l5.66 5.66a8 8 0 1 1-11.31 0z" />
                        </svg>
                    </x-circle-icon>
                    SPO2
                </div>

                <div
                    class="mt-[1.6vh] sm:mt-[0.7vh] flex-1 sm:rounded-[2.5vh] rounded-[3vh] flex justify-between items-baseline">
                    <span class="flex-[2.5] text-[8.5vh] sm:text-[7.1vh] sm:leading-[7.1vh] leading-[8.5vh] text-end"
                        x-text="spo2"></span>
                    <span
                        class="flex-1 text-[1.9vh] sm:text-[1.5vh]  leading-[1.9vh] sm:leading-[1.5vh] text-gray-400">%</span>
                </div>

            </div>
        </div>

    </div>
</section>
