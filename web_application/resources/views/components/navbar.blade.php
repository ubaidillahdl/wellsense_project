<section class="sm:h-[5vh] h-[6vh]  flex items-end justify-center ">
    <nav class="w-[85%] flex justify-between  text-white">
        <a href="" class="font-semibold text-[2.3vh] sm:text-[1.9vh] flex items-center">
            <span class="italic">
                Well
            </span>
            Sense
        </a>

        {{-- Cek: Jika route saat ini BUKAN 'login', maka tampilkan tombol logout --}}
        @if (!Route::is('login') && !Route::is('register'))
            <form action="{{ route('logout') }}" method="POST">
                @csrf
                <button type="submit" class="flex items-center cursor-pointer text-gray-400">
                    <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24"
                        fill="none" stroke="currentColor" stroke-width="2.8" stroke-linecap="round"
                        stroke-linejoin="round"
                        class="feather feather-log-out h-[2.3vh] sm:h-[1.9vh] hover:text-white transition-colors duration-300">
                        <path d="M9 21H5a2 2 0 0 1-2-2V5a2 2 0 0 1 2-2h4" />
                        <polyline points="16 17 21 12 16 7" />
                        <line x1="21" y1="12" x2="9" y2="12" />
                    </svg>
                </button>

            </form>
        @endif
    </nav>
</section>
