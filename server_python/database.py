import mysql.connector
from mysql.connector import Error
import json

class WellSenseDB:
    def __init__(self):
        self.config = {
            'host': '127.0.0.1',
            'user': 'root',
            'password': '',
            'database': 'wellsense_db'
        }
        self.db = mysql.connector.connect(**self.config)

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

    def save_health_data(self, device_info, data_vitals, raw_signals):
        """Menyimpan hasil olahan ANN ke database"""
        try:
            cursor = self.db.cursor()
            query = """
                INSERT INTO data_kesehatan 
                (pengguna_id, device_id, filtered_ir, filtered_red, hr, spo2, sbp, dbp, hb, created_at, updated_at) 
                VALUES (%s, %s, %s, %s, %s, %s, %s, %s, %s, NOW(), NOW())
            """

            # Kita konversi list signal ke JSON string agar masuk ke longText
            values = (
                device_info['pengguna_id'],
                device_info['id'],
                json.dumps(raw_signals.get('ir', [])),
                json.dumps(raw_signals.get('red', [])),
                data_vitals.get('hr'),
                data_vitals.get('spo2'),
                data_vitals.get('sbp'),
                data_vitals.get('dbp'),
                data_vitals.get('hb')
            )

            cursor.execute(query, values)
            self.db.commit()
            cursor.close()
            print("[+] Data berhasil disimpan ke Database !")
        except Exception as e:
            print(f"[!] Gagal simpan data: {e}")