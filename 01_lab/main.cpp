#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <fstream>

#include "nr/nrutil.h"
#include "nr/nrutil.c"
#include "nr/gaussj.c"

#define N 400 

int main(void) {
	double a = 1.f;
	double h = 0.1;
	double w = 1.f;

	double v0 = 0;
	double wh = pow(w, 2)*pow(h, 2)-2;
	float **A,**b;
    std::ofstream f("result.dat");

	A = matrix(1, N, 1, N);
	b = matrix(1, N, 1, 1);
	
	for(int i=1; i<=N; ++i){
		b[i][1] = 0.0;
		for(int j=1; j<=N; ++j)
			A[i][j] = 0.0;
	}

	A[2][1] = -1.f;
	b[1][1] = a;
	b[2][1] = v0 * h;
	
	for(int i=1; i<=N; ++i)
		A[i][i] = 1.0;

	for(int i=3; i<=N; ++i) {
		A[i][i-1] = wh;
		A[i][i-2] = 1.0;
	}
	
	gaussj(A,N,b,1);


	for(int i=1; i<=N; ++i)
		f << (i-1)*h << " " <<  b[i][1] << std::endl;
    
	free_matrix(A,1,N,1,N);
	free_matrix(b,1,N,1,1);    
	
	return 0;
}

