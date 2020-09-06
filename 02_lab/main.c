#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "nr/nrutil.h"
#include "nr/nrutil.c"
#include "nr/gaussj.c"
#include "nr/ludcmp.c"
#include "nr/lubksb.c"

#define N 4

void print_matrix(float **A, int n) {
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            printf("%f ", A[i][j]);
        }
        printf("\n");
    }
}

float** pop_matrix() {
   float** A;
   A = matrix(1, N, 1, N);
   for (int i = 1; i <= N; i++) {
       for (int j = 1; j <= N; j++) {
           A[i][j] = 1./(i+j);
        }
    }
   return A;
}

float M_norm(float** A) {
    float max = fabs(A[1][1]);
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            if (max < fabs(A[i][j])) {
                max = fabs(A[i][j]);
            }
        }
    }
    return max;
}



int main() {
   float **A, **B, **Ai;
   int *indx;
   float d;
   double detA;

   // Alloc vector for ludcmp func
   indx = ivector(1, N);

   // Alloc the matrix with given values
   A = pop_matrix();
   B = pop_matrix();

   ludcmp(A, N, indx, &d);

   // Print L diagonal
   for (int i = 1; i <= N; i++) {
       printf("L[%d][%d] = %f\n", i, i, A[i][i]);
    }
   printf("\n");

   // Print det(A)
   detA = (double) d;
   for (int i = 1; i <= N; i++) {
       detA *= (double) A[i][i];
    }
   printf("det(A) = %F\n", detA);
   printf("\n");

   // Inverse
   float *bt = vector(1, N);
   Ai = matrix(1, N, 1, N);
   
   for (int i = 1; i <= N; i++) {
       bt[1] = bt[2] = bt[3] = bt[4] = 0;
       bt[i] = 1;
       lubksb(A, N, indx, bt);
       for (int j = 1; j <= N; j++) {
           Ai[j][i] = bt[j];
        }
    }
   printf("A' = (\n");
   print_matrix(Ai, N);
   printf(")\n\n");


   // A*Ai
   float** C;
   C = matrix(1, N, 1, N);
   for (int i = 1; i <= N; i++) {
       for (int j = 1; j <= N; j++) {
           C[i][j] = 0;
           for (int k = 1; k <= N; k++) {
                C[i][j] += B[i][k] * Ai[k][j];
            }
        }
    }
   printf("C = (\n");
   print_matrix(C, N);
   printf(")\n\n");


   // Norm
   float a = M_norm(B);
   float b = M_norm(Ai);
   
   printf("||A||  = %f\n", a);
   printf("||Ai|| = %f\n", b);
   printf("k(A) = %f\n", a*b);

   free_matrix(A, 1, N, 1, N);
   free_matrix(B, 1, N, 1, N);
   free_matrix(Ai, 1, N, 1, N);
   free_ivector(indx, 1, N);
   free_vector(bt, 1, N);
}
