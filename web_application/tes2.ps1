[Console]::OutputEncoding = [System.Text.Encoding]::UTF8
$PEM_PATH = "D:\_Documents\ubdl-server\ubdl-ssh-key.pem"
$REMOTE_USER = "ubaidillahdl"
$REMOTE_IP = "ubdl.my.id"
$REMOTE_PATH = "/var/www/wellsense.ubdl.my.id"

Write-Host "`n1. BUILDING FRONTEND..." -ForegroundColor Black -BackgroundColor DarkBlue
npm run build | ForEach-Object { Write-Host "`t$_" }

Write-Host "`n2. SINKRONISASI FILE KE VPS..." -ForegroundColor Black -BackgroundColor DarkBlue
# Hapus "database" dari list utama agar tidak terkirim utuh (mencegah overwrite sqlite)
$folders = "app", "config", "public/build", "resources", "routes"

foreach ($folder in $folders) {
    Write-Host "`tSending folder: $folder..."
    scp -r -i "$PEM_PATH" "./$folder" "${REMOTE_USER}@${REMOTE_IP}:${REMOTE_PATH}/" | ForEach-Object { Write-Host "`t$_" }
}

# --- BAGIAN PENGECEUALIAN (DATABASE) ---
Write-Host "`tSending folder: database (migrations & seeders only)..."
# Hanya kirim folder migrations dan seeders agar database.sqlite di VPS tidak tertimpa
scp -r -i "$PEM_PATH" "./database/migrations" "${REMOTE_USER}@${REMOTE_IP}:${REMOTE_PATH}/database/" | ForEach-Object { Write-Host "`t$_" }
scp -r -i "$PEM_PATH" "./database/seeders" "${REMOTE_USER}@${REMOTE_IP}:${REMOTE_PATH}/database/" | ForEach-Object { Write-Host "`t$_" }

Write-Host "`n3. UPDATE KONFIGURASI (.env)..." -ForegroundColor Black -BackgroundColor DarkBlue
scp -i "$PEM_PATH" ./.env.production "${REMOTE_USER}@${REMOTE_IP}:${REMOTE_PATH}/.env" | ForEach-Object { Write-Host "`t$_" }

Write-Host "`n4. MERAPIKAN SERVER DAN DEEP CLEANING CACHE..." -ForegroundColor Black -BackgroundColor DarkBlue
$remoteCmd = "sudo chown -R ubaidillahdl:www-data ${REMOTE_PATH}; " +
"sudo find ${REMOTE_PATH} -type d -exec chmod 775 {} \;; " +
"sudo find ${REMOTE_PATH} -type f -exec chmod 664 {} \;; " +
"sudo chmod -R 775 ${REMOTE_PATH}/storage ${REMOTE_PATH}/bootstrap/cache ${REMOTE_PATH}/database; " +
"sudo rm -f ${REMOTE_PATH}/public/hot; " +
"sudo rm -f ${REMOTE_PATH}/bootstrap/cache/*.php; " +
"cd ${REMOTE_PATH}; " +
"php artisan optimize:clear; " + 
"php artisan cache:clear; " + 
"php artisan config:cache; " + 
"php artisan route:cache; " + 
"php artisan view:cache; " +
"php artisan queue:restart"

ssh -i "$PEM_PATH" "${REMOTE_USER}@${REMOTE_IP}" "$remoteCmd" | ForEach-Object { Write-Host "`t$_" }

Write-Host "`nDEPLOYMENT SELESAI!`n" -ForegroundColor Green