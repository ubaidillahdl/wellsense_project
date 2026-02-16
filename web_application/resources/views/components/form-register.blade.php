<section class="flex-1 flex items-center justify-center ">
    <div
        class="bg-gray-800 my-[1vh] w-[calc(100%-2vh)] sm:rounded-[3vh] rounded-[4vh] flex flex-col justify-center items-center">
        <div class="w-[calc(100%-3vh)] sm:my-[1vh] my-[1.5vh] sm:py-[0.5vh] py-[1vh] flex flex-col ">
            <div class=" flex flex-col text-[1.9vh] sm:text-[1.5vh] text-gray-400 w-full">

                <form action="{{ route('register') }}" method="POST">
                    @csrf {{-- Keamanan Token --}}

                    {{-- Nama --}}
                    <div class="sm:pb-[1vh] pb-[1.5vh]">
                        <label class="block sm:mb-[0.3vh] mb-[0.5vh] px-[1vh]">Nama Lengkap</label>
                        <input type="text" name="nama"
                            class="w-full sm:h-[3vh] h-[4vh] bg-slate-900  px-[2vh] rounded-full border-none  focus:ring-[0.15vh] focus:ring-gray-600 text-white text-[1.9vh] sm:text-[1.5vh] text-center"
                            required value="{{ old('nama') }}">

                        @error('nama')
                            <span class="text-red-500 text-sm">{{ $message }}</span>
                        @enderror
                    </div>

                    {{-- Email --}}
                    <div class="sm:pb-[1vh] pb-[1.5vh]">
                        <label class="block sm:mb-[0.3vh] mb-[0.5vh] px-[1vh]">Email</label>
                        <input type="email" name="email"
                            class="w-full sm:h-[3vh] h-[4vh] bg-slate-900  px-[2vh] rounded-full border-none  focus:ring-[0.15vh] focus:ring-gray-600 text-white text-[1.9vh] sm:text-[1.5vh] text-center"
                            required value="{{ old('email') }}">

                        @error('email')
                            <span class="text-red-500 text-sm">{{ $message }}</span>
                        @enderror
                    </div>

                    {{-- Password --}}
                    <div class="sm:pb-[1vh] pb-[1.5vh]">
                        <label class="block sm:mb-[0.3vh] mb-[0.5vh] px-[1vh]">Kata Sandi</label>
                        <input type="password" name="password"
                            class="w-full sm:h-[3vh] h-[4vh] rounded-full bg-slate-900 border-none focus:ring-[0.15vh] focus:ring-gray-600 px-[2vh] text-white text-[1.9vh] sm:text-[1.5vh] text-center"
                            required>

                        @error('password')
                            <span class="text-red-500 text-sm">{{ $message }}</span>
                        @enderror
                    </div>

                    {{-- Konfirmasi Password --}}
                    <div class="sm:pb-[1vh] pb-[1.5vh]">
                        <label class="block sm:mb-[0.3vh] mb-[0.5vh] px-[1vh]">Konfirmasi Kata Sandi</label>
                        <input type="password" name="password_confirmation"
                            class="w-full sm:h-[3vh] h-[4vh] rounded-full bg-slate-900 border-none focus:ring-[0.15vh] focus:ring-gray-600 px-[2vh] text-white text-[1.9vh] sm:text-[1.5vh] text-center"
                            required>
                    </div>

                    {{-- Tombol Aksi --}}
                    <div class="w-full flex justify-center ">
                        <div class="w-[80%] flex gap-3 sm:pt-[1.5vh] pt-[2vh]">

                            <a href="/login"
                                class="w-1/2 border-gray-600 border-[0.2vh] text-gray-400 py-2 hover:bg-gray-700 transition-colors duration-300 text-center flex items-center justify-center rounded-full sm:h-[3vh] h-[4vh]">
                                Masuk
                            </a>
                            <button type="submit"
                                class=" bg-gray-600 text-white py-2 rounded-full  hover:bg-gray-700 transition-colors duration-300 w-1/2 sm:h-[3vh] h-[4vh] flex items-center justify-center cursor-pointer">
                                Daftar
                            </button>
                        </div>
                    </div>
                </form>

            </div>
        </div>
    </div>
</section>
