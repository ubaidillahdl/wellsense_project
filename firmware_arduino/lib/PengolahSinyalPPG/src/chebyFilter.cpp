#include "chebyFilter.h"

// Koefisien SOS dari hasil MATLAB Anda
// Format per baris: {b0, b1, b2, a1, a2}
static const float sosCoeffs[4][5] = {
    //  Rs:40, LP:0.5, HP:8.0, Orde:4
    // {1.0000000000f, -0.8474135721f, 1.0000000000f, -1.6294757062f,
    // 0.6806832852f},
    // {1.0000000000f, -1.7177300221f, 1.0000000000f, -1.7948290282f,
    // 0.8728101180f},
    // {1.0000000000f, -1.9998392417f, 1.0000000000f, -1.9003070624f,
    // 0.9043081573f},
    // {1.0000000000f, -1.9991430930f, 1.0000000000f, -1.9710279257f,
    // 0.9740465299f}};
    // static const float globalGain = 0.0107612698;

    //  Rs:40, LP:0.3, HP:5.0, Orde:4
    //     {1.0000000000, -1.4647441125, 1.0000000000, -1.7659087800,
    //     0.7868814204},
    //     {1.0000000000, -1.8876304518, 1.0000000000, -1.8875399551,
    //     0.9181567090},
    //     {1.0000000000, -1.9999423021, 1.0000000018, -1.9396651001,
    //     0.9411301470},
    //     {1.0000000000, -1.9996916420, 0.9999999982, -1.9830711424,
    //     0.9841700153}};
    // static const float globalGain = 0.0107612698;

    //  Rs:40, LP:0.5, HP:10.0, Orde:4
    //     {1.0000000000, -0.4052300198, 1.0000000000, -1.5158564677,
    //     0.5953982374},
    //     {1.0000000000, -1.5650295768, 1.0000000000, -1.7212869453,
    //     0.8371855015},
    //     {1.0000000000, -1.9998428318, 1.0000000000, -1.8974966719,
    //     0.9013679762},
    //     {1.0000000000, -1.9991460470, 1.0000000000, -1.9685939899,
    //     0.9717459636}};
    // static const float globalGain = 0.0120824983;

    //  Rs:40, LP:0.5, HP:8.0, Orde:4
    {1.0000000000, -1.6435820318, 1.0000000000, -1.8143171106, 0.8279373498},
    {1.0000000000, -1.9278243189, 1.0000000000, -1.9146468734, 0.9345251605},
    {1.0000000000, -1.9999597559, 1.0000000092, -1.9500730364, 0.9511001235},
    {1.0000000000, -1.9997856969, 0.9999999908, -1.9862582796, 0.9870156962}};
static const float globalGain = 0.0094909218;

void chebyInit(ChebyFilter *f) {
  for (int i = 0; i < 4; i++) {
    f->sections[i].w1 = 0.0f;
    f->sections[i].w2 = 0.0f;
  }

  // Inisialisasi DC Remover (alpha 0.99)
  // Panggil fungsi init dari dcRemoval.h
  dcRemoverInit(&(f->dc), 0.99f);
}

float chebyProcess(ChebyFilter *f, float input) {
  // 1. Buang DC Offset (16384) terlebih dahulu
  // Panggil fungsi process dari dcRemoval.h
  float acInput = dcRemoverProcess(&(f->dc), input);
  // float acInput = input - 16384.0f;

  float v = acInput * globalGain;

  for (int i = 0; i < 4; i++) {
    // Implementasi Direct Form II
    // w[n] = x[n] - a1*w[n-1] - a2*w[n-2]
    float w0 = v - (sosCoeffs[i][3] * f->sections[i].w1) -
               (sosCoeffs[i][4] * f->sections[i].w2);

    // y[n] = b0*w[n] + b1*w[n-1] + b2*w[n-2]
    float out = (sosCoeffs[i][0] * w0) + (sosCoeffs[i][1] * f->sections[i].w1) +
                (sosCoeffs[i][2] * f->sections[i].w2);

    // Update state history
    f->sections[i].w2 = f->sections[i].w1;
    f->sections[i].w1 = w0;

    // Output section ini menjadi input section berikutnya
    v = out;
  }

  return v;
}