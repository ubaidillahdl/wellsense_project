<x-layout>
    <div class="h-full flex flex-col sm:text-[2vh] text-white">

        <x-navbar></x-navbar>
        <x-ppg :latest-data='$latestData'></x-ppg>
        <x-hr-spo2 :latest-data='$latestData'></x-hr-spo2>
        <x-bp :latest-data='$latestData'></x-bp>
        <x-hb :latest-data='$latestData'></x-hb>

    </div>
</x-layout>
