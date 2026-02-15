import "./bootstrap";
import Alpine from "alpinejs";
import Chart from "chart.js/auto";

window.Alpine = Alpine;
window.Chart = Chart;

let ppgChartInstance = null; // Simpan instance chart secara global di file ini

// 1. LISTENER REVERB (Websocket)
window.Echo.connector.pusher.connection.bind("connected", () => {
    console.log("Terhubung ke Reverb!");
});

window.Echo.channel("health-updates").listen("HealthDataReceived", (e) => {
    console.log("Data diterima Echo:", e);

    // Kirim data ke window agar bisa ditangkap Alpine di file mana saja
    window.dispatchEvent(
        new CustomEvent("vitals-updated", {
            detail: e.data,
        }),
    );
});

// 2. LOGIK CHART PPG
window.ppgChart = function () {
    return {
        pointsToShow: 150, // Sesuaikan dengan jumlah data per plot (3 detik @ 50Hz)

        initChart(initialData = []) {
            const ctx = document.getElementById("ppgCanvas");
            if (!ctx) return;

            // Atur ketebalan garis (lebih tebal di mobile agar terlihat jelas)
            const isMobile = window.innerWidth < 768;
            const thickness = isMobile ? 3 : 1.5;

            // Gunakan data dari database untuk tampilan awal (Initial Load)
            let dataAwal =
                initialData && initialData.length > 0
                    ? initialData
                    : Array(this.pointsToShow).fill(null);

            ppgChartInstance = new Chart(ctx, {
                type: "line",
                data: {
                    labels: Array.from(
                        { length: this.pointsToShow },
                        (_, i) => i,
                    ),
                    datasets: [
                        {
                            label: "PPG Signal",
                            data: dataAwal,
                            borderColor: "#ffffff",
                            borderWidth: thickness,
                            pointRadius: 0, // Hilangkan titik-titik data, cukup garis saja
                            fill: false,
                            tension: 0.3, // Membuat garis sedikit melengkung (smooth)
                        },
                    ],
                },
                options: {
                    responsive: true,
                    maintainAspectRatio: false,
                    animation: false, // Wajib false agar update data terasa real-time
                    scales: {
                        x: { display: false }, // Sembunyikan sumbu X
                        y: {
                            display: false, // Sembunyikan sumbu Y agar bersih
                            beginAtZero: false, // Penting karena data PPG ada nilai negatif
                            grace: "20%", // Kasih ruang atas-bawah agar sinyal gak kepotong
                        },
                    },
                    plugins: {
                        legend: { display: false },
                        tooltip: { enabled: false },
                    },
                },
            });

            // Paksa gambar pertama kali
            ppgChartInstance.update();

            // Dengarkan event dari Reverb untuk update live
            window.addEventListener("vitals-updated", (e) => {
                if (e.detail.filtered_ir) {
                    this.updatePlot(e.detail.filtered_ir);
                }
            });
        },

        updatePlot(newSignals) {
            if (!ppgChartInstance || !newSignals) return;

            // Timpa data lama dengan data baru (Mode: Per Plot)
            ppgChartInstance.data.datasets[0].data = newSignals;
            ppgChartInstance.update("none"); // Update instan tanpa animasi
        },
    };
};

Alpine.start();
