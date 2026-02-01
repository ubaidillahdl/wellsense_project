import "./bootstrap";
import Alpine from "alpinejs";
import Chart from "chart.js/auto";

window.Alpine = Alpine;
window.Chart = Chart;

let ppgChartInstance = null;

window.ppgChart = function () {
    return {
        pointsToShow: 400,

        initChart() {
            const ctx = document.getElementById("ppgCanvas");
            if (!ctx) return;

            // Deteksi apakah layar mobile (lebar < 768px)
            const isMobile = window.innerWidth < 768;
            const thickness = isMobile ? 3 : 1.5; // 3 untuk HP, 1 untuk Laptop

            ppgChartInstance = new Chart(ctx, {
                type: "line",
                data: {
                    labels: Array.from(
                        { length: this.pointsToShow },
                        (_, i) => i
                    ),
                    datasets: [
                        {
                            data: Array(this.pointsToShow).fill(0),
                            borderColor: "#ffff",
                            borderWidth: thickness, // Gunakan variabel thickness di sini
                            pointRadius: 0,
                            fill: false,
                            tension: 0.3,
                        },
                    ],
                },
                options: {
                    responsive: true,
                    maintainAspectRatio: false,
                    animation: false,
                    // Tambahkan spanGaps untuk menangani data kosong jika ada
                    spanGaps: true,
                    scales: {
                        x: {
                            display: false,
                            type: "linear", // Paksa menggunakan linear scale
                        },
                        y: {
                            display: false,
                            min: 0,
                            max: 100,
                            beginAtZero: true,
                        },
                    },
                    plugins: {
                        legend: { display: false },
                        tooltip: { enabled: false },
                    },
                },
            });

            setInterval(() => {
                this.updateWithBuffer();
            }, 1000);
        },

        updateWithBuffer() {
            if (!ppgChartInstance) return;

            const hz = 200;
            const bufferCount = hz * 1;
            let newBuffer = [];

            for (let i = 0; i < bufferCount; i++) {
                let time = Date.now() / 1000 + i / hz;
                // Simulasi sinyal PPG
                let val =
                    50 +
                    20 * Math.sin(2 * Math.PI * 1.2 * time) +
                    8 * Math.sin(4 * Math.PI * 1.2 * time + 1.5);
                newBuffer.push(val);
            }

            const dataset = ppgChartInstance.data.datasets[0].data;
            dataset.push(...newBuffer);

            if (dataset.length > this.pointsToShow) {
                dataset.splice(0, dataset.length - this.pointsToShow);
            }

            ppgChartInstance.update("none");
        },
    };
};

Alpine.start();
