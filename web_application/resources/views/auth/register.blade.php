<!DOCTYPE html>
<html>

<head>
    <title>Daftar Wellsense</title>
    <script src="https://cdn.tailwindcss.com"></script>
</head>

<body class="bg-gray-100 flex justify-center items-center h-screen">
    <div class="bg-white p-8 rounded shadow-md w-96">
        <h2 class="text-2xl font-bold mb-4 text-center">Daftar Akun</h2>

        <form action="{{ route('register') }}" method="POST">
            @csrf
            <div class="mb-4">
                <label>Nama Lengkap</label>
                <input type="text" name="nama" class="w-full border p-2 rounded" value="{{ old('nama') }}">
                @error('nama')
                    <span class="text-red-500 text-sm">{{ $message }}</span>
                @enderror
            </div>

            <div class="mb-4">
                <label>Email</label>
                <input type="email" name="email" class="w-full border p-2 rounded" value="{{ old('email') }}">
                @error('email')
                    <span class="text-red-500 text-sm">{{ $message }}</span>
                @enderror
            </div>

            <div class="mb-4">
                <label>Password</label>
                <input type="password" name="password" class="w-full border p-2 rounded">
                @error('password')
                    <span class="text-red-500 text-sm">{{ $message }}</span>
                @enderror
            </div>

            <div class="mb-6">
                <label>Konfirmasi Password</label>
                <input type="password" name="password_confirmation" class="w-full border p-2 rounded">
            </div>

            <button type="submit" class="w-full bg-green-500 text-white p-2 rounded hover:bg-green-600">Daftar
                Sekarang</button>
        </form>

        <p class="mt-4 text-center text-sm">
            Sudah punya akun? <a href="{{ route('login') }}" class="text-blue-500">Login di sini</a>
        </p>
    </div>
</body>

</html>
