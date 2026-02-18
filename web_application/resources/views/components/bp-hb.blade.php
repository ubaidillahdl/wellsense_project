<section class=" flex-1 flex items-center justify-center">
    <div class=" h-[calc(100%-2vh)]  w-[calc(100%-2vh)] sm:rounded-[3vh] rounded-[4vh] flex gap-[2vh] @container-[size]"
        x-data="{
            sbp: {{ round($latestData->sbp) }},
            dbp: {{ round($latestData->dbp) }},
            hb: {{ round($latestData->hb) }},
        }"
        @vitals-updated.window="
        sbp = Math.round($event.detail.vitals.sbp);
        dbp = Math.round($event.detail.vitals.dbp);
        hb = Math.round($event.detail.vitals.hb);">

        <div class="bg-gray-800 flex-[1.2] sm:rounded-[3vh] rounded-[4vh] flex justify-center items-center">
            <div class="w-[calc(100%-3vh)] h-[calc(100%-20cqh)] sm:h-[calc(100%-20cqh)] flex flex-col ">

                {{-- Label BP --}}
                <div class="h-[3vh] text-[1.9vh] sm:text-[1.5vh] font-semibold flex  items-center text-gray-400 ">
                    <x-circle-icon class='bg-fuchsia-500'>
                        <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24"
                            fill="none" stroke="currentColor" stroke-width="3" stroke-linecap="round"
                            stroke-linejoin="round" class="feather feather-thermometer  h-[calc(100%-1vh)] text-white">
                            <path d="M14 14.76V3.5a2.5 2.5 0 0 0-5 0v11.26a4.5 4.5 0 1 0 5 0z" />
                        </svg>
                    </x-circle-icon>
                    <span>BP <span class=" text-[1.5vh] sm:text-[1.25vh] opacity-50">(mmHg)</span></span>
                </div>

                {{-- Value BP --}}
                <div
                    class="mt-[1.6vh] sm:mt-[0.7vh] flex-1 sm:rounded-[2.5vh] rounded-[3vh] flex justify-center items-center h-[calc(100%-20cqh)] sm:h-[calc(100%-25cqh)] @container-[size]">

                    <div class="flex items-center text-white h-[100cqh]">
                        <span class="text-[60cqh] sm:text-[60cqh]  -translate-y-[13cqh] sm:-translate-y-[13cqh]"
                            x-text="sbp"></span>
                        <span class="text-[40cqh] text-gray-600 font-light">/</span>
                        <span class="text-[55cqh] sm:text-[65cqh] translate-y-[13cqh] sm:translate-y-[18cqh]"
                            x-text="dbp"></span>
                    </div>
                </div>

            </div>
        </div>

        <div class="bg-gray-800 flex-1 sm:rounded-[3vh] rounded-[4vh] flex items-center justify-center">
            <div class="w-[calc(100%-3vh)] h-[calc(100%-3vh)] flex flex-col">

                {{-- Label HB --}}
                <div class="h-[3vh] text-[1.9vh] sm:text-[1.5vh] font-semibold flex items-center text-gray-400 ">
                    <x-circle-icon class='bg-cyan-500'>
                        <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="none" stroke="currentColor"
                            stroke-width="3" stroke-linecap="round" stroke-linejoin="round"
                            class="text-white  h-[calc(100%-1vh)]">
                            <path d="M12 22s8-4 8-10V5l-8-3-8 3v7c0 6 8 10 8 10z" />
                        </svg>
                    </x-circle-icon>
                    <span>HB <span class=" text-[1.5vh] sm:text-[1.25vh] opacity-50">(g/L)</span></span>
                </div>

                {{-- Value HB --}}
                <div
                    class="bg-gray-800 flex-1 sm:rounded-[2.5vh] rounded-[3vh] flex flex-col justify-center items-center h-[calc(100%-20cqh)] sm:h-[calc(100%-25cqh)] @container-[size]">
                    <div class="flex items-center text-white h-[100cqh]">
                        <span class="text-[55cqh] sm:text-[65cqh] " x-text="hb"></span>
                    </div>
                </div>
            </div>
        </div>

    </div>
</section>
