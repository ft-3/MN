#include <cmath>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "nr/nrutil.h"
#include "nr/nrutil.c"
#include "nr/gauher.c"
#include "nr/bessi0.c"

const double sigma = 1.f/sqrt(2);

double calc_r0(double R10_1, double R10_2, double R20_1, double R20_2) {
    return sqrt(R10_1 * R10_1 + R10_2 * R10_2) + sqrt(R20_1 * R20_1 + R20_2 * R20_2);
}

double calc_Vdok(double r0) {
    double sigma2 = pow(sigma, 2);
    return pow((2 * M_PI), 2) * pow(sigma, 4) * sqrt(M_PI)/(2 * sigma) * exp(-(r0 * r0) / (8 * sigma2)) * bessi0((pow(r0, 2)) / (8 * sigma2));
}

float* init_R10() {
    float* R10 = vector(1, 2);
    R10[1] = 0;
    R10[2] = 0;
    return R10;
}

float* init_R20(float x20) {
    float* R20 = vector(1, 2);
    R20[1] = x20;
    R20[2] = 0;
    return R20;
}

void free_vectors(float* R10, float* R20, float* x1, float* x2, float* w1, float* w2, int n) {
			free_vector(R10, 1, 2);
			free_vector(R20, 1, 2);
			free_vector(x1, 1, n);
			free_vector(x2, 1, n+1);
			free_vector(w1, 1, n);
			free_vector(w2, 1, n+1);
}

int main() {

    std::ofstream result("out.dat");
	for (int n = 5; n <= 35; n+=5) {
		for (double x20 = .1; x20 <= 6.0001; x20 += .1) {

            auto R10 = init_R10();
            auto R20 = init_R20(x20);

			double r0 = calc_r0(R10[1], R10[2], R20[1], R20[2]);
			double Vdok = calc_Vdok(r0);

			float* x1 = vector(1, n);
			float* w1 = vector(1, n);

			float* x2 = vector(1, n+1);
			float* w2 = vector(1, n+1);

			gauher(x1, w1, n);
			gauher(x2, w2, n+1);

			double Vnum = 0.f;
			for (int i = 1; i <= n; i++)
				for (int j = 1; j <= n+1; j++)
					for (int k = 1; k <= n; k++)
						for (int l = 1; l <= n+1; l++) {
                            double up = (w1[i] * w2[j] * w1[k] * w2[l]);
							Vnum += up / sqrt(pow(x1[i] - x2[j] + x20, 2) + pow(x1[k] - x2[l], 2));				
                        }

			double eps = fabs((Vdok-Vnum)/Vdok);
			
            result << std::fixed << std::setprecision(8) << x20 << " " << eps << std::endl;
            free_vectors(R10, R20, x1, x2, w1, w2, n);
		}
        result << "\n\n";
	}
	return 0;
}
