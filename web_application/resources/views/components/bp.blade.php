        <section class=" flex-1 flex items-center justify-center">
            <div
                class="bg-gray-800 h-[calc(100%-2vh)]  w-[calc(100%-2vh)] sm:rounded-[3vh] rounded-[4vh] flex justify-center items-center">
                <div class="w-[calc(100%-3vh)] h-[calc(100%-3vh)] flex flex-col">
                    <div class="h-[3vh] text-[1.9vh] sm:text-[1.5vh] font-semibold flex items-center text-gray-400">
                        <x-circle-icon class="bg-gray-200 text-gray-800">
                            <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24"
                                fill="none" stroke="currentColor" stroke-width="3" stroke-linecap="round"
                                stroke-linejoin="round" class="feather feather-shield  h-[calc(100%-1vh)]">
                                <path d="M12 22s8-4 8-10V5l-8-3-8 3v7c0 6 8 10 8 10z" />
                            </svg>
                        </x-circle-icon>
                        BP
                    </div>
                    <div class=" flex-1 mt-[0.8vh] sm:rounded-[2.5vh] rounded-[3vh] flex gap-[2vh]">
                        <div class="bg-gray-600 flex-2 sm:rounded-[2.5vh] rounded-[3vh] flex items-center"
                            x-data="{
                                sbp: {{ round($latestData->sbp) }},
                                dbp: {{ round($latestData->dbp) }},
                            }"
                            @vitals-updated.window="
                            if ($event.detail.vitals.hr) {
                                sbp = Math.round($event.detail.vitals.sbp);
                            }
                            if ($event.detail.vitals.spo2) {
                                dbp = Math.round($event.detail.vitals.dbp);
                            }
                            ">

                            <div class="flex-3
                            flex flex-col ps-[3vh] text-gray-400">
                                <span
                                    class=" w-[50%] text-[5.9vh] sm:text-[4.9vh] sm:leading-[4.9vh] leading-[5.9vh] 
                                    text-center text-white"
                                    x-text="sbp"></span>
                                <div class="flex justify-center">

                                    /<span
                                        class="ms-[1.5vh] text-[5.9vh] sm:text-[4.9vh] sm:leading-[4.9vh] leading-[5.9vh] text-end text-white"
                                        x-text="dbp"></span>
                                    <span class=" flex items-end text-[1.9vh] sm:text-[1.5vh]">mmHg</span>
                                </div>
                            </div>
                        </div>

                        <div
                            class="bg-gray-600 flex-1 sm:rounded-[2.5vh] rounded-[3vh] flex flex-col justify-center items-center">
                            <a href=""
                                class="flex flex-col justify-center items-center text-gray-400 text-[1.5vh] sm:text-[1.25vh]">

                                <svg class="h-[8vh] w-[8vh] sm:h-[6vh] sm:w-[6vh]  " aria-hidden="true"
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
