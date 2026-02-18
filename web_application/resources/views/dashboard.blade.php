<x-layout title="Wellsense ● Dashboard ">
    <div class="h-full flex flex-col sm:text-[2vh] text-white">

        <x-navbar></x-navbar>
        <x-ppg :latest-data='$latestData' :diff-seconds='$diffSeconds'></x-ppg>
        <x-bp-hb :latest-data='$latestData'></x-bp-hb>
        <x-hr-spo2 :latest-data='$latestData'></x-hr-spo2>
        <x-ai-insight :latest-insight='$latestInsight'></x-ai-insight>

        {{-- <x-bp :latest-data='$latestData'></x-bp> --}}
        {{-- <x-hb :latest-data='$latestData'></x-hb> --}}

    </div>
</x-layout>
