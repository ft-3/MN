#include "nr/nrutil.c"
#include "nr/nrutil.h"
#include "nr/ludcmp.c"
#include "nr/lubksb.c"
#include <fstream>
#include <iostream>

const int N = 4;

float** alloc_matrix() {
    float** mat = matrix(1, N, 1, N);
    for (int i = 1; i <= N; i++)
        for (int j = 1; j <= N; j++)
            mat[i][j] = 1.f/(i+j);
    return mat;
}

float calc_det(float** LU) {
    float sum = 1;
    for (int i = 1; i <= N; i++)
        sum *= LU[i][i];
    return sum;
}

void print_diag(float** LU) {
    std::cout << "Elementy diagonalne U: ";
    for (int i = 1; i <= N; i++)
        std::cout << LU[i][i] << " ";
    std::cout << std::endl;
}

float* alloc_b() {
    float* b = vector(1, N);
    b[1] = 1;
    b[2] = b[3] = b[4] = 0;
    return b;
}


int main() {
    float** A = alloc_matrix();
    int* indx = ivector(1, N);
    float d;

    ludcmp(A, N, indx, &d);
    print_diag(A);
    float det = calc_det(A) * d;
    std::cout << "Wyznacznik A: " << det << std::endl;
    float* b = alloc_b();
    lubksb(A, N, indx, b);
}
