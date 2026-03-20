[Console]::OutputEncoding = [System.Text.Encoding]::UTF8
$PEM_PATH = "D:\_Documents\ubdl-server\ubdl-ssh-key.pem"
$REMOTE_USER = "ubaidillahdl"
$REMOTE_IP = "ubdl.my.id"
$REMOTE_PATH = "/var/www/wellsense"

Write-Host "`n2. SINKRONISASI FILE TOTAL (FIRST TIME)..." -ForegroundColor Black -BackgroundColor DarkBlue
# Tambahkan file root penting: artisan, composer.json, package.json
$files = "artisan", "composer.json", "composer.lock", "package.json", "vite.config.js"
foreach ($f in $files) {
    scp -i "$PEM_PATH" "./$f" "${REMOTE_USER}@${REMOTE_IP}:${REMOTE_PATH}/" | Out-Null
}

# Tambahkan folder 'bootstrap' (tanpa cache) untuk inisialisasi awal
$folders = "app", "config", "public", "resources", "routes", "bootstrap", "database", "storage"
foreach ($folder in $folders) {
    Write-Host "`tSending folder: $folder..."
    scp -r -i "$PEM_PATH" "./$folder" "${REMOTE_USER}@${REMOTE_IP}:${REMOTE_PATH}/" | Out-Null
}

Write-Host "`n3. INITIALIZING SERVER ENVIRONMENT..." -ForegroundColor Black -BackgroundColor DarkBlue
$remoteCmd = "cd ${REMOTE_PATH} && " +
"sudo chown -R ubaidillahdl:www-data . && " +
"sudo chmod -R 775 storage bootstrap/cache database && " +
"composer install --no-dev --optimize-autoloader && " + # Download library di server
"php artisan key:generate && " + # Buat APP_KEY pertama kali
"php artisan storage:link && " + # Link folder storage ke public
"touch database/database.sqlite && " + # Buat file database jika blm ada
"php artisan migrate --seed && " + # Buat tabel & data awal
"php artisan optimize"

ssh -i "$PEM_PATH" "${REMOTE_USER}@${REMOTE_IP}" "$remoteCmd"