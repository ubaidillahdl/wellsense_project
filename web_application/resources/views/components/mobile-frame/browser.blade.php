<div class="sm:flex justify-between h-[60%] items-center px-[2vh]">

    <div class=" w-[10%] flex justify-center">
        <svg class="w-[2.5vh] h-[2.5vh] text-gray-800 dark:text-white" aria-hidden="true"
            xmlns="http://www.w3.org/2000/svg" width="24" height="24" fill="none" viewBox="0 0 24 24">
            <path stroke="currentColor" stroke-linecap="round" stroke-linejoin="round" stroke-width="1.5"
                d="m4 12 8-8 8 8M6 10.5V19a1 1 0 0 0 1 1h3v-3a1 1 0 0 1 1-1h2a1 1 0 0 1 1 1v3h3a1 1 0 0 0 1-1v-8.5" />
        </svg>
    </div>

    <div class="w-[70%]" x-data="{ currentUrl: window.location.origin }">
        <div class="bg-slate-600 mx-[1vh] rounded-full px-[2vh] h-[3vh] text-[1.7vh] flex items-center overflow-hidden">
            <span class=" inline-block align-middle font-semibold text-white" x-text="currentUrl">Loading...</span>
        </div>
    </div>

    <div class=" w-[20%]  items-center flex justify-between">
        <svg class="w-[2.5vh] h-[2.5vh] text-gray-800 dark:text-white" aria-hidden="true"
            xmlns="http://www.w3.org/2000/svg" width="24" height="24" fill="none" viewBox="0 0 24 24">
            <path stroke="currentColor" stroke-linecap="round" stroke-linejoin="round" stroke-width="2"
                d="M5 12h14m-7 7V5" />
        </svg>

        <svg class="w-[2.5vh] h-[2.5vh] text-gray-800 dark:text-white" aria-hidden="true"
            xmlns="http://www.w3.org/2000/svg" width="24" height="24" fill="none" viewBox="0 0 24 24">
            <path stroke="currentColor" stroke-linecap="round" stroke-width="2" d="M5 7h14M5 12h14M5 17h14" />
        </svg>

    </div>
</div>
