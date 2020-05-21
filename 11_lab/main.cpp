#include <cmath>
#include <iostream>
#include <fstream>
#include <gsl/gsl_fft_complex.h>
#include <cstdlib>
#include <ctime>

const int k = 12;
const int N = pow(2, k);
const int stride = 1;

double y_0(double x) {
    double omega = 2 * (2 * M_PI / N);
    return sin(omega*x) + sin(2*omega*x) + sin(3*omega*x);

}

double delta() {
    return 2 * (rand() / (RAND_MAX + 1.f)) - 1;
}

double y(double x) {
    return y_0(x) + delta();
}

double complex_module(double re, double im) {
    return sqrt(pow(re, 2) + pow(im, 2));
}

double calc_y_max(double* data) {
    double max = complex_module(data[0], data[1]);
    for (int i = 0; i < N; i++)
        if (complex_module(data[2*i], data[2*i+1]) > max)
            max = complex_module(data[2*i], data[2*i+1]);
   return max; 
}

int main() {
    //srand(time(nullptr));
    double data[2*N];
    std::ofstream fft_dat;
    std::ofstream y8;
    //fft_dat.open("fft8.dat");
    y8.open("y12.dat");
    // Losowanie tablicy
    for (int i = 0; i < N; i++) {
        data[2*i] = y(i);
        data[2*i+1] = 0;
        std::cout << "[" << 2*i << "]: " << data[2*i] << std::endl;
    }
    // FFT
    gsl_fft_complex_radix2_forward((gsl_complex_packed_array) data, stride, N);
    // Print data after FFT
    for (int i = 0; i < N; i++) {
     //   fft_dat << i << " " << data[2*i] << " " << data[2*i+1] << std::endl;
        y8 << i << " " << data[2*i] << std::endl;
    }

    auto y_max = calc_y_max(data);
    // Denoising
    for (int i = 0; i < N; i++)
        if (complex_module(data[2*i], data[2*i+1]) < (.5 * y_max)) {
            data[2*i] = 0;
            data[2*i+1] = 0;
        }

    // Reverse FFT
    gsl_fft_complex_radix2_backward(data, stride, N);
    for (int i = 0; i < 2*N; i++)
        data[i] /= N;

    y8 << std::endl << std::endl;
    for (int i = 0; i < N; i++)
        y8 << i << " " << data[2*i] << std::endl;

}
