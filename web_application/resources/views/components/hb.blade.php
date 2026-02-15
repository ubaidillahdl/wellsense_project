<section class=" flex-1 flex justify-center items-center">
    <div
        class="bg-gray-800 h-[calc(100%-2vh)]  w-[calc(100%-2vh)] sm:rounded-[3vh] rounded-[4vh] flex justify-center items-center">

        <div class="w-[calc(100%-3vh)] h-[calc(100%-3vh)] flex flex-col">
            <div class="h-[3vh] text-[1.9vh] sm:text-[1.5vh] font-semibold flex items-center text-gray-400">
                <x-circle-icon class="bg-gray-200 text-gray-800">
                    <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none"
                        stroke="currentColor" stroke-width="3" stroke-linecap="round" stroke-linejoin="round"
                        class="feather feather-thermometer  h-[calc(100%-1vh)]">
                        <path d="M14 14.76V3.5a2.5 2.5 0 0 0-5 0v11.26a4.5 4.5 0 1 0 5 0z" />
                    </svg>
                </x-circle-icon>
                HB
            </div>

            <div class=" flex-1 mt-[0.8vh] sm:rounded-[2.5vh] rounded-[3vh] flex gap-[2vh]">
                <div class="bg-gray-600 flex-2 sm:rounded-[2.5vh] rounded-[3vh] flex items-center"
                    x-data="{
                        hb: {{ round($latestData->hb) }},
                    }"
                    @vitals-updated.window="
                    if ($event.detail.vitals.hr) {
                        hb = Math.round($event.detail.vitals.hb);
                    }
                ">
                    <div class="flex-1 flex justify-between items-baseline">
                        <span
                            class="flex-[2.5] text-[8.5vh] sm:text-[7.1vh] sm:leading-[7.1vh] leading-[8.5vh] text-end"
                            x-text="hb.toString().replace('.', ',')"></span>
                        <span
                            class="flex-1
                            text-[1.9vh] sm:text-[1.5vh] leading-[1.9vh] sm:leading-[1.5vh] text-gray-400">g/L</span>
                    </div>
                </div>

                <div
                    class="bg-gray-600 flex-1 sm:rounded-[2.5vh] rounded-[3vh] flex flex-col justify-center items-center">
                    <a href=""
                        class="flex flex-col justify-center items-center text-[1.5vh] sm:text-[1.25vh] text-gray-400">
                        <svg class="h-[8vh] w-[8vh]  sm:h-[6vh] sm:w-[6vh] " aria-hidden="true"
                            xmlns="http://www.w3.org/2000/svg" width="24" height="24" fill="none"
                            viewBox="0 0 24 24">
                            <path stroke="currentColor" stroke-linecap="round" stroke-linejoin="round"
                                stroke-width="1.5"
                                d="M12 8v4l3 3M3.22302 14C4.13247 18.008 7.71683 21 12 21c4.9706 0 9-4.0294 9-9 0-4.97056-4.0294-9-9-9-3.72916 0-6.92858 2.26806-8.29409 5.5M7 9H3V5" />
                        </svg>
                        History
                    </a>
                </div>

            </div>
        </div>
</section>
