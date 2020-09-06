#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <functional>
#include <cmath>

const long int mod2 = pow(2, 32) - 5;

double rand_norm() {
    return (rand() + 1.) / (RAND_MAX + 1.);
}

std::vector<double> box_miller(std::vector<double> u) {
    double z1 = sqrt(-2*log(u[0]))*cos(2*M_PI*u[1]);
    double z2 = sqrt(-2*log(u[0]))*sin(2*M_PI*u[1]);
    return {z1, z2};
}

double calc_ci(double x1, double x2, double y1, double y2, double x20) {
    return 1/(sqrt(pow(x1-(x2+x20), 2) + pow(y1-y2, 2)));
}


int main() {

    std::ofstream result("out.dat");
	for (int n = 1e2; n <= 1e5; n*=10) {

		for (double x20 = 0; x20 <= 6.f; x20 += .1) {

            double sum = 0;
            double sum2 = 0;
            for (int i = 0; i < n; i++) {
                auto x = box_miller({rand_norm(), rand_norm()});
                auto y = box_miller({rand_norm(), rand_norm()});
                double ci = calc_ci(x[0], x[1], y[0], y[1], x20);
                sum += ci;
                sum2 += ci*ci;
            }
            double Vn = sum/n;
            double var = (sum2-(sum*sum)/n)/n;
            double eps = sqrt(var/n);



            result << std::fixed << std::setprecision(8) << x20 << " " << Vn << " " << eps << std::endl;
		}
        result << "\n\n";
	}
	return 0;
}
