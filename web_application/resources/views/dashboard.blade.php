<x-layout>
    <div class="h-full flex flex-col sm:text-[2vh] text-white">
        <form action="{{ route('logout') }}" method="POST">
            @csrf
            <button type="submit" class="bg-red-500 text-white px-4 py-2 rounded">
                Keluar
            </button>
        </form>

        @if (session('success'))
            <div x-data="{ show: true }" x-init="setTimeout(() => show = false, 3000)" x-show="show" x-transition.duration.500ms
                class="fixed top-5 right-5 bg-green-500 text-white px-6 py-3 rounded-lg shadow-xl z-50 flex items-center">
                <svg class="w-6 h-6 mr-2" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                    <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M5 13l4 4L19 7"></path>
                </svg>

                <span>{{ session('success') }}</span>
            </div>
        @endif

        <x-navbar></x-navbar>
        <x-ppg :latest-data='$latestData'></x-ppg>
        <x-hr-spo2 :latest-data='$latestData'></x-hr-spo2>
        <x-bp :latest-data='$latestData'></x-bp>
        <x-hb :latest-data='$latestData'></x-hb>


    </div>
</x-layout>
