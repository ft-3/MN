#include <vector>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iomanip>

long int N = 2000;
const long int mod1 = pow(2, 13) - 1;
const long int mod2 = pow(2, 32) - 5;

long int rand_norm(std::vector<long int> xb, std::vector<int> a, long int mod) {
    long int sum = 0;
    for (long unsigned int i = 0; i < xb.size(); i++)
        sum += (xb[i]*a[i] % mod);
    return sum % mod;
}

std::vector<double> us(double* U, int i) {
    return {U[3+4*i], U[3+4*i+1], U[3+4*i+2], U[3+4*i+3]};
}

void normalize(double* U, int len, long int m) {
    for (int i = 0; i < len; i++) {
        U[i] = U[i]/(m + 1);
    }
}

void gen_for_sphere(double* U) {
    for (int i = 3; i < 5*N+6; i++) {
        U[i] = rand_norm({(long) U[i-3],(long) U[i-2],(long) U[i-1]}, {1776, 1476, 1176}, mod2);
    }
    normalize(U, 5*N+6, mod2);
}

void normalize_vec(std::vector<double>& vec) {
    double ri = sqrt(pow(vec[0], 2)+pow(vec[1], 2)+pow(vec[2],2));
    for (auto& el : vec) {
        el = el / ri;
    }
}

int main() {
    double U1[N+1]; U1[0] = 10;
    double U2[N+1]; U2[0] = 10;
    double U3[N+6]; U3[0] = U3[1] = U3[2] = 10;

    std::ofstream u1("u1.dat");
    std::ofstream u2("u2.dat");
    std::ofstream u3("u3.dat");
    std::ofstream sp("3d.dat");
    std::ofstream hist("hist3.dat");

    for (int i = 1; i < N+1; i++) {
        U1[i] = rand_norm({(long int)U1[i-1]}, {17}, mod1);
        U2[i] = rand_norm({(long int)U2[i-1]}, {85}, mod1);
    }

    for (int i = 3; i < N+6; i++) {
        U3[i] = rand_norm({(long) U3[i-3],(long) U3[i-2],(long) U3[i-1]}, {1776, 1476, 1176}, mod2);
    }

    // Normalize
    normalize(U1, N+1, mod1);
    normalize(U2, N+1, mod1);
    normalize(U3, N+6, mod2);

    for (int i = 1; i < N+1-3; i++) {
        u1 << std::fixed << std::setprecision(8) <<U1[i] << " " << U1[i+1] << " " << U1[i+2] << " " << U1[i+3] << std::endl;
        u2 << std::fixed << std::setprecision(8) << U2[i] << " " << U2[i+1] << " " << U2[i+2] << " " << U2[i+3] << std::endl;
    }
    for (int i = 3; i < N; i++)
        u3 << std::fixed << std::setprecision(8) << U3[i] << " " << U3[i+1] << " " << U3[i+2] << " " << U3[i+3] << std::endl;

    N = pow(10, 4);
    // Sphere
    double U[5*N+6]; U[0] = U[1] = U[2] = 10;
    gen_for_sphere(U);

    std::vector<double> vecs[N];
    for (int i = 0; i < N; i++) {
        auto u = us(U, i);
        double xi = sqrt(-2*log(1-u[0]))*cos(2*M_PI*u[1]);
        double yi = sqrt(-2*log(1-u[0]))*sin(2*M_PI*u[1]);
        double zi = sqrt(-2*log(1-u[2]))*cos(2*M_PI*u[3]);
        vecs[i] = {xi, yi, zi};
        normalize_vec(vecs[i]);
    }

    for (int i = 0; i < N; i++)
        sp << std::fixed << std::setprecision(8) << vecs[i][0] << " " << vecs[i][1] << " " << vecs[i][2] << std::endl;

    for (int i = 0; i < N; i++) {
        double si = pow(U[3+4*N+i],(1.0/3));
        for (auto& el : vecs[i])
            el *= si;
    }
    for (int i = 0; i < N; i++)
        sp << std::fixed << std::setprecision(8) << vecs[i][0] << " " << vecs[i][1] << " " << vecs[i][2] << std::endl;

    const int K = 10;
    const double delta = 1.0/K;
    int quant[10] {0};
    for (int i = 0; i < N; i++) {
        double ri = sqrt(pow(vecs[i][0], 2)+pow(vecs[i][1], 2)+pow(vecs[i][2],2));
        quant[(int)(ri/delta)]++;
    }

    for(int i = 0; i < K; i++) {
    	double Rj = delta * (i+1);
        double Rj1 = delta * (i);
        double Vj = (4.0 / 3.0) * M_PI * pow(Rj, 3);
        double Vj1 = (4.0 / 3.0) * M_PI * pow(Rj1, 3);
        double gj = quant[i] / (Vj - Vj1);

        hist << (Rj+Rj1)/2 << " " << quant[i] << " " << gj << std:: endl;
    }



    
}
