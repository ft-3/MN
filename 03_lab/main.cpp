#include <cmath>
#include <fstream>
#include <iostream>
#include <time.h>

#include "nr/nrutil.h"
#include "nr/nrutil.c"
#include "nr/gaussj.c"


const int N = 1e4;
const double m = 5.f;

// -------------------------------------------------------------------------


double** init_matrix() {
    double** A = new double*[N];
    for (int i = 0; i < N; i++) {
        A[i] = new double[N];
        for (int j = 0; j < N; j++) {
            A[i][j] = abs(i-j) <= m ? 1.f/(1.f+abs(i-j)) : 0.f;
        }
    }
    return A;
}

double* init_vector() {
    double* b = new double[N];
    for (int i = 0; i < N; i++)
        b[i] = (double)i + 1.f;
    return b;
}

double* init_x(double val) {
    double* x = new double[N];
    for (int i = 0; i < N; i++)
        x[i] = val;
    return x;
}

// -------------------------------------------------------------------------

double* matrix_times_vec(double** M, double* v) {
    double* result = new double[N];
    for (int i = 0; i < N; i++) {
        result[i] = 0;
        for (int j = 0; j < N; j++) 
            result[i] += M[i][j]*v[j];
    }
    return result;
}

double* vec_min_vec(double* a, double* b) {
    double* result = new double[N];
    for (int i = 0; i < N; i++)
        result[i] = a[i] - b[i];
    return result;
}

double* vec_plus_vec(double* a, double* b) {
    double* result = new double[N];
    for (int i = 0; i < N; i++)
        result[i] = a[i] + b[i];
    return result;
}

double vec_times_vec(double* a, double* b) {
    double result = 0;
    for (int i = 0; i < N; i++)
        result += a[i] * b[i];
    return result;
}

double* vec_times_num(double* r, double alpha) {
    double* result = new double[N];
    for (int i = 0; i < N; i++) result[i] = alpha*r[i];
    return result;
}

// -------------------------------------------------------------------------


double norm(double* a) {
    return sqrt(vec_times_vec(a,a));
}



int main() {
    std::ofstream zada("zad_a.dat");
    std::ofstream zadb("zad_b.dat");
    double** A = init_matrix();
    double* b = init_vector();
    double* x = init_x(0.f);
    double* r;
    double alpha;

    int k = 0;
    clock_t t1 = clock();
    do {
        r = vec_min_vec(b, matrix_times_vec(A, x));
        alpha = vec_times_vec(r, r)/vec_times_vec(r, matrix_times_vec(A, r));
        zada << k++ << " " << norm(r) << " " << alpha << " " << norm(x) << std::endl;
        x = vec_plus_vec(x, vec_times_num(r, alpha));
    } while (norm(r) > 1e-6);

    clock_t t2 = clock();
    double dur1 = 1000.f*(t2-t1)/CLOCKS_PER_SEC;
    std::cout << "Trwanie wstegowej (x=0): " << dur1 << " ms" << std::endl;

    x = init_x(1.f);
    k = 0;
    t1 = clock();
    do {
        r = vec_min_vec(b, matrix_times_vec(A, x));
        alpha = vec_times_vec(r, r)/vec_times_vec(r, matrix_times_vec(A, r));
        zadb << k++ << " " << norm(r) << " " << alpha << " " << norm(x) << std::endl;
        x = vec_plus_vec(x, vec_times_num(r, alpha));
    } while (norm(r) > 1e-6);
    t2 = clock();
    dur1 = 1000.f*(t2-t1)/CLOCKS_PER_SEC;
    std::cout << "Trwanie wstegowej (x=1): " << dur1 << " ms" << std::endl;
    

    float** M = matrix(1, N, 1, N);
    float** by = matrix(1, N, 1, 1);

    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <=N; j++) {
            float u = abs(i-j) <= m ? abs(i-j) : 0;
            M[i][j] = 1 / 1 + u;
        }
    }
    for (int i = 1; i <= N; i++)
        by[i][1] = i + 1;

    t1 = clock();
    gaussj(M, N, by, 1);
    t2 = clock();
    double dur2 = 1000.f*(t2-t1)/CLOCKS_PER_SEC;
    std::cout << "Trwanie Gaussa: " << dur2 << " ms" << std::endl;
}
