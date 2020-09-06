#include <fstream>
#include <iostream>
#include <iomanip>
#include <cmath>

const double xmin = -5.f;
const double xmax = 5.f;

double Wn(double *xm, double *ym, int n, double x) {

	double result = 0.f;
	for (int j = 0; j <= n; j++) {
		double product = 1.0;
		for (int k = 0; k <= n; k++) {
            if (k == j) continue;
			product *= (x - xm[k])/(xm[j] - xm[k]);
		}
		result += ym[j] * product;
	}

	return result;
}

double f(double x) { return exp(-pow(x, 2)); }

double czeb_w(double m, double n) { return ((xmin - xmax) * cos(M_PI * (2.0 * m + 1.0) / (2.0 * n + 2.0)) + (xmin + xmax)) / 2.0; }

int main() {

    std::ofstream czeb("zad_2.dat");
    std::ofstream xmdat("zad_1.dat");
	for (int n = 5; n <= 20; n += 5) {
		double step = (xmax - xmin) / n;

        // Czebyszew
        double x[n+1];
        // Rownodlegle
		double xm[n+1];
        //Wartosci
        double ym[n+1];
        //Wartosci czebyszew
        double ymc[n+1];

        for (int i = 0; i <= n; i++) {
            xm[i] = xmin + (step * i);
            ym[i] = f(xm[i]);
			x[i] = czeb_w(i, n);
            ymc[i] = f(x[i]);
        }


		for (double i = xmin; i <= xmax; i+=.01) {
            xmdat << std::setprecision(10) << i << " " << std::setprecision(10) << Wn(xm, ym, n, i) << std::endl;
            czeb << std::setprecision(10) << i << " " << std::setprecision(10) << Wn(x, ymc, n, i) << std::endl;
		}
        xmdat << std::endl << std::endl;
        czeb << std::endl << std::endl;

	}
}

