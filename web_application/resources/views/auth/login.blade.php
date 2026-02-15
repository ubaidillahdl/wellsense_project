<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Login - Wellsense</title>
    <script src="https://cdn.tailwindcss.com"></script>
</head>

<body class="bg-gray-100 h-screen flex items-center justify-center">

    <div class="bg-white p-8 rounded-lg shadow-md w-96">
        <h2 class="text-2xl font-bold mb-6 text-center">Login Wellsense</h2>

        @if ($errors->any())
            <div class="bg-red-100 text-red-600 p-3 rounded mb-4 text-sm">
                {{ $errors->first() }}
            </div>
        @endif

        <form action="/login" method="POST">
            @csrf

            <div class="mb-4">
                <label class="block text-sm font-semibold mb-1">Email</label>
                <input type="email" name="email" class="w-full border rounded p-2" required
                    value="{{ old('email') }}">
            </div>

            <div class="mb-6">
                <label class="block text-sm font-semibold mb-1">Password</label>
                <input type="password" name="password" class="w-full border rounded p-2" required>
            </div>

            <button type="submit" class="w-full bg-blue-600 text-white py-2 rounded font-bold hover:bg-blue-700">
                Login
            </button>
        </form>
    </div>

</body>

</html>
