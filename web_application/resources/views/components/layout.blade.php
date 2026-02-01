<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    @vite(['resources/css/app.css', 'resources/js/app.js'])
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>Document</title>
</head>

<body class="h-dvh w-dvw  sm:bg-linear-50 sm:from-slate-950 sm:to-slate-800 flex items-center justify-center sm:p-4">
    <div
        class="h-full w-full sm:h-[calc(100vh-4rem)]  sm:w-[calc((100vh-4rem)*6/13)]  sm:rounded-[5vh] sm:border-[0.5vh] sm:border-slate-700 sm:shadow-lg ">
        <div
            class="h-full w-full sm:rounded-[4.5vh] sm:pb-0  bg-gray-900  sm:border-[1.5vh] sm:border-gray-950 sm flex flex-col overflow-hidden">

            <header class="bg-slate-800 hidden sm:h-[8vh] sm:block text-gray-300 overflow-hidden">
                <x-mobile-frame.device></x-mobile-frame.device>
                <x-mobile-frame.browser></x-mobile-frame.browser>
            </header>

            <main class="flex-1"> {{ $slot }} </main>

        </div>
    </div>
    <script src="https://cdn.jsdelivr.net/npm/flowbite@4.0.1/dist/flowbite.min.js"></script>
</body>

</html>
