import "./bootstrap";
import Alpine from "alpinejs";
import Chart from "chart.js/auto";

window.Alpine = Alpine;
window.Chart = Chart;

document.addEventListener("alpine:init", () => {
    // --- KONEKSI REVERB/PUSHER ---
    window.Echo.connector.pusher.connection.bind("connected", () => {
        console.log("Koneksi Reverb Berhasil !");
    });

    // --- LISTENER DATA DARI SERVER (SIM800C -> LARAVEL -> ECHO) ---
    window.Echo.channel("health-updates").listen("HealthDataReceived", (e) => {
        const res = e.data ? e.data : e;

        if (res.is_update_insight) {
            // Sebar event khusus untuk update teks saran AI
            window.dispatchEvent(
                new CustomEvent("ai-insight-update", {
                    detail: res.ai_insight,
                }),
            );
            console.log("Insight AI: ", res);
        } else {
            // Sebar event untuk data vital & grafik PPG
            window.dispatchEvent(
                new CustomEvent("vitals-updated", {
                    detail: res,
                }),
            );
            console.log("Data Vital: ", res);
        }
    });

    // --- STORE: INSIGHT AI ---
    Alpine.store("insight", {
        saran: "",
        init() {
            window.addEventListener(
                "ai-insight-update",
                (e) => (this.saran = e.detail),
            );
        },
    });

    // --- STORE: TIMER DATA TERAKHIR ---
    Alpine.store("timer", {
        detikLalu: 0,

        init() {
            setInterval(() => {
                this.detikLalu++;
            }, 1000);

            // Reset timer ke 0 kalau ada data vital baru masuk
            window.addEventListener("vitals-updated", () => {
                this.detikLalu = 0;
            });
        },

        get formatTeks() {
            let d = Math.floor(this.detikLalu);
            if (d < 5) return " Baru saja";
            if (d < 60) return `${d} detik lalu`;
            if (d < 3600) return `${Math.floor(d / 60)} menit lalu`;
            return `${Math.floor(d / 3600)} jam lalu`;
        },

        setAwal(detik) {
            this.detikLalu = detik;
        },
    });

    // --- KOMPONEN: GRAFIK PPG ---
    Alpine.data("ppgChart", (initialData = []) => {
        let chartInstance = null; // Private variable agar tidak crash (stack overflow)

        return {
            pointsToShow: 150, // Jumlah titik yang tampil di layar

            init() {
                const ctx = document.getElementById("ppgCanvas");
                if (!ctx) return;

                const isMobile = window.innerWidth < 768;
                const thickness = isMobile ? 3 : 1.5;

                chartInstance = new Chart(ctx, {
                    type: "line",
                    data: {
                        // Membuat label sumbu X otomatis (0-149)
                        labels: Array.from(
                            { length: this.pointsToShow },
                            (_, i) => i,
                        ),
                        datasets: [
                            {
                                label: "PPG Signal",
                                data: initialData,
                                borderColor: "#ffffff",
                                borderWidth: thickness,
                                pointRadius: 0, // Sembunyikan titik dot
                                fill: false,
                                tension: 0.3, // Bikin garis agak melengkung (smooth)
                            },
                        ],
                    },
                    options: {
                        responsive: true,
                        maintainAspectRatio: false,
                        animation: false, // Matikan animasi biar ringan (real-time)
                        scales: {
                            x: { display: false }, // Sembunyikan angka sumbu X
                            y: {
                                display: false,
                                beginAtZero: false,
                                grace: "20%", // Kasih jarak atas bawah biar sinyal gak kepotong
                            },
                        },
                        plugins: {
                            legend: { display: false },
                            tooltip: { enabled: false }, // Matikan tooltip biar gak berat
                        },
                    },
                });

                chartInstance.update();

                // Dengerin sinyal baru untuk di-plot ke grafik
                window.addEventListener("vitals-updated", (e) => {
                    let signal = e.detail.filtered_ir;
                    if (signal) this.updatePlot(signal);
                });
            },

            updatePlot(newSignals) {
                if (!chartInstance || !Array.isArray(newSignals)) return;

                // Timpa data lama dengan array sinyal baru (spread operator untuk reaktivitas)
                chartInstance.data.datasets[0].data = [...newSignals];
                // Update grafik tanpa animasi ("none") untuk performa maksimal
                chartInstance.update("none");
            },
        };
    });
});

Alpine.start();
