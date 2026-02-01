import numpy as np
from processor import Processor


class VitalEngine:
    def __init__(self):
        self.processor = Processor()
        self.fs = 400
        self.feedback_str = ""
        self.last_result = None
        self.new_data_available = False  # Penanda untuk Arduino (Feedback)
        self.graph_ready = False  # Penanda untuk Plotter (Grafik)

    def process_package(self, red_data, ir_data):
        """Alur: Pre-processing -> Gatekeeper -> ANN Analysis"""
        if not ir_data:
            return None

        # --- 1. PRE-PROCESSING (Sinyal Mentah) ---
        ac_ir, v_dc_ir = self.processor.dc_remover(ir_data)
        ppg_ir = self.processor.apply_spline(
            self.processor.butter_filter(self.processor.moving_average(ac_ir)),
            target_hz=self.fs,
        )

        ac_red, v_dc_red = self.processor.dc_remover(red_data)
        ppg_red = self.processor.apply_spline(
            self.processor.butter_filter(self.processor.moving_average(ac_red)),
            target_hz=self.fs,
        )

        t_ppg = np.linspace(0, len(ppg_ir) / self.fs, len(ppg_ir))
        std_val = np.std(ir_data)

        # Inisialisasi output dasar
        output = {
            "status": "IDLE",
            "ppg_ir": ppg_ir,
            "t_ppg": t_ppg,
            "best_cycle": None,
            "vitals_display": None,
        }

        # Simpan state awal agar plotter tetap jalan
        self.last_result = output
        self.graph_ready = True

        # --- 2. GATEKEEPER (Cek Sensor) ---
        if std_val < 5:
            self.processor.reset_all_filters()
            output["status"] = "SENSOR_OFF"
            self.last_result = output
            return output

        if std_val > 100:
            output["status"] = "NOISY"
            self.last_result = output
            return output

        # --- 3. ANALISIS SIKLUS & ANN ---
        v_ac_ir = np.max(ppg_ir) - np.min(ppg_ir)
        v_ac_red = np.max(ppg_red) - np.min(ppg_red)
        vpg = np.gradient(ppg_ir, 1 / self.fs)
        valid_cycles = self.processor._extract_valid_cycles(vpg)

        if valid_cycles:
            best_cycle = self.processor.get_best_cycle_by_sqi(ppg_ir, valid_cycles)
            if best_cycle:
                start, end = best_cycle["z1"], best_cycle["z3"]
                cycle = self.processor.straighten_cycle(ppg_ir[start:end])
                t_cycle = np.arange(len(cycle)) / self.fs
                w_out = self.processor.extract_w1_w2(cycle, t_cycle, self.fs)

                if w_out:
                    # Hitung parameter vital
                    spo2 = self.processor.calculate_spo2(
                        v_ac_ir, v_dc_ir, v_ac_red, v_dc_red
                    )
                    sbp = self.processor.model_sbp.calculate(
                        [
                            w_out["sysTime"],
                            w_out["w1W2Ratio"],
                            w_out["heartRate"],
                            w_out["diasTime"],
                            w_out["cycleDuration"],
                        ]
                    )
                    dbp = self.processor.model_dbp.calculate(
                        [
                            w_out["w1W2Ratio"],
                            w_out["sysTime"],
                            w_out["relWidthW1"],
                            w_out["heartRate"],
                            w_out["diasTime"],
                        ]
                    )
                    hb = self.processor.model_hb.calculate(
                        [
                            w_out["sysTime"],
                            w_out["w1W2Ratio"],
                            w_out["diasTime"],
                            w_out["relWidthW2"],
                            w_out["widthW2"],
                        ]
                    )

                    # Filter angka agar stabil
                    s_hr = self.processor.hr_filter.get_stable_value(w_out["heartRate"])
                    s_spo2 = self.processor.spo2_filter.get_stable_value(spo2)
                    s_sbp = self.processor.sbp_filter.get_stable_value(sbp)
                    s_dbp = self.processor.dbp_filter.get_stable_value(dbp)
                    s_hb = self.processor.hb_filter.get_stable_value(hb)

                    # Siapkan feedback untuk Arduino via Listener
                    self.feedback_str = f"*{s_hr:.1f};{s_spo2:.1f};{s_sbp:.0f};{s_dbp:.0f};{s_hb:.2f}#\n"
                    self.new_data_available = True

                    # Update output lengkap untuk Plotter
                    output["status"] = "VALID"
                    output["vitals_display"] = [s_hr, s_spo2, s_sbp, s_dbp, s_hb]
                    output["best_cycle"] = {
                        "signal": cycle,
                        "time": t_cycle,
                        "w_out": w_out,
                        "z_points": {
                            "x": [
                                t_cycle[best_cycle["z1"] - start],
                                t_cycle[best_cycle["z2"] - start],
                                t_cycle[best_cycle["z3"] - start - 1],
                            ],
                            "y": [
                                cycle[best_cycle["z1"] - start],
                                cycle[best_cycle["z2"] - start],
                                cycle[best_cycle["z3"] - start - 1],
                            ],
                        },
                        "global_markers": {
                            "x": [
                                t_ppg[best_cycle["z1"]],
                                t_ppg[best_cycle["z2"]],
                                t_ppg[best_cycle["z3"]],
                            ],
                            "y": [
                                ppg_ir[best_cycle["z1"]],
                                ppg_ir[best_cycle["z2"]],
                                ppg_ir[best_cycle["z3"]],
                            ],
                        },
                        "skew": best_cycle["skew"],
                    }

        # Simpan hasil akhir & nyalakan lampu hijau buat Plotter
        self.last_result = output
        self.graph_ready = True
        return output

    def get_feedback(self):
        """Diambil oleh Listener untuk feedback ke hardware"""
        if self.new_data_available:
            self.new_data_available = False
            return self.feedback_str
        return None

    def get_last_result(self):
        """Diambil oleh Plotter untuk update visualisasi"""
        if self.graph_ready:
            self.graph_ready = False
            return self.last_result
        return None

    def _print_table(self, v):
        """Mencetak data vital ke terminal dengan format tabel ASCII"""
        print("\n┌───────────────────────────────────────────┐")
        print("│      ANALISA VITAL SIGN REAL-TIME         │")
        print("├─────────────────────┬─────────────────────┤")
        print(f"│ HEART RATE          │  {v[0]:>8.2f} bpm       │")
        print(f"│ SPO2 (OXYGEN)       │  {v[1]:>8.2f} %         │")
        print("├─────────────────────┼─────────────────────┤")
        print(f"│ SYSTOLIC PRESSURE   │  {v[2]:>8.4f} mmHg      │")
        print(f"│ DIASTOLIC PRESSURE  │  {v[3]:>8.4f} mmHg      │")
        print("├─────────────────────┼─────────────────────┤")
        print(f"│ HEMOGLOBIN (Hb)     │  {v[4]:>8.4f} g/dL      │")
        print("└─────────────────────┴─────────────────────┘")
