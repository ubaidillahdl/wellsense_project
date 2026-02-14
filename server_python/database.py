import mysql.connector
import json
import requests


class WellSenseDB:
    def __init__(self):
        self.config = {
            "host": "127.0.0.1",
            "user": "root",
            "password": "",
            "database": "wellsense_db",
        }
        try:
            self.db = mysql.connector.connect(**self.config)
        except Exception as e:
            print(f"[!] Database Connection Error: {e}")

    def triger_dashboard(self, token):
        """Mengirim sinyal ke Laravel API untuk update dashboard"""
        url = "http://192.168.0.105/api/v1/send-health-data"
        payload = {"device_token": str(token)}
        headers = {"Content-Type": "application/json", "Accept": "application/json"}

        try:
            response = requests.post(url, json=payload, headers=headers)
            if response.status_code == 200:
                print("[+] Sinyal pemicu dashboard berhasil dikirim")
            else:
                print(f"[!] Web server merespon status {response.status_code}")
        except Exception as e:
            print(f"[!] Web server error (Koneksi): {e}")

    def get_device_info(self, token):
        try:
            cursor = self.db.cursor(dictionary=True)
            query = "SELECT id, pengguna_id FROM devices WHERE device_token = %s"
            cursor.execute(query, (token,))
            result = cursor.fetchone()
            cursor.close()
            return result
        except Exception as e:
            print(f"[!] Lookup Error: {e}")
            return None

    def save_health_data(
        self, device_info, device_token, data_vitals, signals, features
    ):
        """Menyimpan hasil olahan ANN ke database"""
        try:
            # Pastikan koneksi tidak timeout
            if not self.db.is_connected():
                self.db.reconnect(attempts=3, delay=1)

            cursor = self.db.cursor()
            query = """
                INSERT INTO data_kesehatan 
                (
                    pengguna_id, device_id, 
                    raw_ir, raw_red, 
                    filtered_ir, filtered_red, 
                    features,
                    hr, spo2, sbp, dbp, hb, 
                    created_at, updated_at
                ) 
                VALUES (%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, NOW(), NOW())
                """

            # Kita konversi list signal ke JSON string agar masuk ke longText
            values = (
                device_info["pengguna_id"],
                device_info["id"],
                json.dumps(signals.get("raw_ir", [])),
                json.dumps(signals.get("raw_red", [])),
                json.dumps(signals.get("filtered_ir", [])),
                json.dumps(signals.get("filtered_red", [])),
                json.dumps(features),
                data_vitals.get("hr"),
                data_vitals.get("spo2"),
                data_vitals.get("sbp"),
                data_vitals.get("dbp"),
                data_vitals.get("hb"),
            )

            cursor.execute(query, values)
            self.db.commit()
            cursor.close()
            print("[+] Data berhasil disimpan ke Database !")

            # --- EKSEKUSI PATH 2 ---
            # Dipanggil otomatis tepat setelah data masuk database
            self.triger_dashboard(device_token)

        except Exception as e:
            print(f"[!] Gagal simpan data: {e}")
