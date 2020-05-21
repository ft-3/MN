#include <cmath>
#include <iostream>
#include <gsl/gsl_linalg.h>
#include <cstdlib>
#include <ctime>

const int N = 11;
const double x_0 = 2.f;
const double delta = 4.f;
const double alp = 4.f;

double d(double x, double alpha) {
    double U = (double) rand() / ((double) RAND_MAX);
    return alpha * (U - .5);
}

double g(double x, double alpha) {
    double a_0 = -pow(x_0, 2)/2/ pow(delta, 2);
    double a_1 = x_0 / pow(delta, 2);
    //double a_2 = (1/2)/pow(delta, 2);
    double a_2 = -0.03125;
    std::cout << a_0 << "+ " << a_1 << "* x + " << a_2 <<"* x^2" << std::endl;
    return exp(a_0 + a_1 * x + a_2 * pow(x, 2)) * (1 + d(x, alpha));
}

gsl_vector* alloc_r(gsl_vector* f_values, gsl_vector* x_axis) {
    gsl_vector* r_vector = gsl_vector_alloc(4);
    for (int i = 0; i < 4; i++) {
        double val = 0.f;
        for (int j = 0; j < N; j++) {
            val += gsl_vector_get(f_values, j) * pow(gsl_vector_get(x_axis, j), i);
        }
        gsl_vector_set(r_vector, i, val);
    }
    return r_vector;
}

gsl_matrix* alloc_g(gsl_vector* x_axis) {
    gsl_matrix* g_matrix = gsl_matrix_alloc(4, 4);
    for (int i = 0; i < 4; i++) {
        for (int k = 0; k < 4; k++) {
            double sum = 0.f;
            for (int j = 0; j < N; j++) {
                sum += pow(gsl_vector_get(x_axis, j), i+k);
            }
            gsl_matrix_set(g_matrix, i, k, sum);
        }
    }

    return g_matrix;
            
}

int main() {
    srand(time(nullptr));

    double x_max = 3.f*delta + x_0; double x_min = -3.f*delta + x_0;
    double dist = (x_max - x_min) / (N - 1);

    gsl_vector* x_axis = gsl_vector_alloc(N);
    for (int i = 0; i < N; i++) {
        gsl_vector_set(x_axis, i, x_min + dist * i);
    }

    gsl_vector* g_values = gsl_vector_alloc(N);
    for (int i = 0; i < N; i++) {
        double x = gsl_vector_get(x_axis, i);
        gsl_vector_set(g_values, i, g(x, 0.f));
    }

    for (int i = 0; i < N; i++) {
        std::cout << "g("<< i << "): " << gsl_vector_get(g_values, i) << std::endl;
    }
    std::cout << std::endl;

    gsl_vector* f_values = gsl_vector_calloc(N);
    for (int i = 0; i < N; i++) {
        double g_j = gsl_vector_get(g_values, i);
        gsl_vector_set(f_values, i, log(g_j));
    }

    for (int i = 0; i < N; i++) {
        std::cout << "f(" << i << "): " << gsl_vector_get(f_values, i) << std::endl;
    }
    std::cout << std::endl;

    gsl_vector* r_vector = alloc_r(f_values, x_axis);
    gsl_matrix* g_matrix = alloc_g(x_axis);

    for (int i = 0; i < 4; i++) {
        std::cout << i << ": " << gsl_vector_get(r_vector, i) << std::endl;
    }

    gsl_linalg_HH_svx(g_matrix, r_vector);

    for (int i = 0; i < 4; i++) {
        std::cout << i << ": " << gsl_vector_get(r_vector, i) << std::endl;
    }



}
