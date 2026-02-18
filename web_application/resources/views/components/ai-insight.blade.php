<section class=" flex-[1.2] flex items-center justify-center">
    <div
        class="bg-gray-800 h-[calc(100%-2vh)]  w-[calc(100%-2vh)] sm:rounded-[3vh] rounded-[4vh] flex justify-center items-center @container-[size]">
        <div class="w-[calc(100%-3vh)] h-[calc(100%-3vh)] flex flex-col">

            <div class="h-[3vh] text-[1.9vh] sm:text-[1.5vh] font-semibold flex items-center text-gray-400">
                <x-circle-icon class="bg-amber-500 text-white">
                    <svg xmlns="http://www.w3.org/2000/svg" height="24px" viewBox="0 -960 960 960" width="24px"
                        stroke="currentColor" stroke-width="3" class="text-white h-[calc(100%-0.5vh)]" fill="#ffffff">
                        <path
                            d="M423.5-103.5Q400-127 400-160h160q0 33-23.5 56.5T480-80q-33 0-56.5-23.5ZM320-200v-80h320v80H320Zm10-120q-69-41-109.5-110T180-580q0-125 87.5-212.5T480-880q125 0 212.5 87.5T780-580q0 81-40.5 150T630-320H330Zm24-80h252q45-32 69.5-79T700-580q0-92-64-156t-156-64q-92 0-156 64t-64 156q0 54 24.5 101t69.5 79Zm126 0Z" />
                    </svg>
                </x-circle-icon>
                Saran Kesehatan
            </div>

            <div
                class=" flex-1 mt-[0.8vh] sm:rounded-[2.5vh] rounded-[3vh] flex gap-[2vh] h-[100cqh] @container-[size]">
                <div x-data x-init="$store.insight.saran = '{{ $latestInsight }}'"
                    class="bg-gray-600 flex-2 sm:rounded-[2.5vh] rounded-[3vh] text-gray-200 opacity-90  px-[1.5vh] py-[1vh] ">
                    <p x-text="$store.insight.saran"
                        class="h-full overflow-y-auto [&::-webkit-scrollbar]:hidden [-ms-overflow-style:none] [scrollbar-width:none] text-[1.9vh] sm:text-[1.5vh] leading-tight sm:leading-snug">
                    </p>
                </div>
            </div>

        </div>
</section>
