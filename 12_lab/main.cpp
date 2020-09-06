#include <cmath>
#include <gsl/gsl_linalg.h>
#include <iostream>
#include <iomanip>

const int n = 9;
const int a = 0;
const int b = 1;


double f(double x) {
    return log(pow(x, 3) + 3*pow(x, 2) + x + .1)*sin(18*x);
}

void fill_matrix_trap(gsl_matrix* m) {
    for (int w = 1; w < n; w++) {
        for (int k = 1; k <= w; k++) {
            double d1 = gsl_matrix_get(m, w, k-1);
            double d2 = gsl_matrix_get(m, w-1, k-1);
            gsl_matrix_set(m, w, k, (pow(4, k) * d1 - d2)/(pow(4, k) - 1));
        }
    }
}

void fill_matrix_3(gsl_matrix* m) {
    for (int w = 1; w < n; w++) {
        for (int k = 1; k <= w; k++) {
            double d1 = gsl_matrix_get(m, w, k-1);
            double d2 = gsl_matrix_get(m, w-1, k-1);
            gsl_matrix_set(m, w, k, (pow(4, k) * d1 - d2)/(pow(4, k) - 1));
        }
    }
}

int main() {
    // Metoda trapezow
    gsl_matrix* richardson_matrix = gsl_matrix_alloc(n, n);
    for (int w = 0; w < n; w++) {
        double N     = pow(2, w);
        double width = (b - a) / N;
        double sum   = 0;

        for (int i = 0; i < N; i++) {
            double result = (width / 2) * (f(a + i*width) + f(a + (i+1)*width));
            sum += result;
        }
        gsl_matrix_set(richardson_matrix, w, 0, sum);
    }
    fill_matrix_trap(richardson_matrix);
    std::cout << "===== Metoda trapezow =====" << std::endl;
    for (int w = 0; w < n; w++) {
        std::cout << std::fixed << std::setprecision(10) << gsl_matrix_get(richardson_matrix, w, 0) << " " << gsl_matrix_get(richardson_matrix, w, w) << std::endl;
    }

    // Metoda 3/8
    richardson_matrix = gsl_matrix_alloc(n, n);
    for (int w = 0; w < n; w++) {
        double N     = 3*pow(2, w);
        double width = (b - a) / N;
        double sum   = 0;

        for (int i = 0; i < N/3; i++) {
            double f0 = f(a + 3*i*width);
            double f1 = f(a + (3*i+1)*width);
            double f2 = f(a + (3*i+2)*width);
            double f3 = f(a + (3*i+3)*width);
            sum += (3*width)/8 * (f0 + 3*f1 + 3*f2+ f3);
        }
        gsl_matrix_set(richardson_matrix, w, 0, sum);
    }
    fill_matrix_trap(richardson_matrix);
    std::cout << "===== Metoda 3/8 =====" << std::endl;
    for (int w = 0; w < n; w++) {
        std::cout << std::fixed << std::setprecision(10) << gsl_matrix_get(richardson_matrix, w, 0) << " " << gsl_matrix_get(richardson_matrix, w, w) << std::endl;
    }
}
