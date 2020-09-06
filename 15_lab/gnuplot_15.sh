set terminal post enhanced colour solid font 20 
reset

# SKRYPT GENERUJE WYKRESY DLA PLIKU out.dat Z TRZEMA KOLUMNAMI:
#     x20, V_n, blad (wzor (11))
#     Wyniki sa zapisane w czterech seriach danych:
#        - 1. seria danych: dla n = 100.
#        - 2. seria danych (po dwoch pustych liniach): dla n = 1000.
#        - 3. seria danych: dla n = 10000.
#        - 4. seria danych: dla n = 100000.

set style line 1 lw 2 lt rgb "black"
set style line 2 lw 2 pt 7 lc rgb "red"

# Funkcja pomocnicza dla funkcji Bessela:
y(x) = (abs(x) < 3.75) ? (x/3.75)**2 : 3.75/x
# Modyfikowana funkcja Bessela I_0(x):
bessi0(x) = (  (abs(x) < 3.75) ?  \
    (1.0+y(x)*(3.5156229+y(x)*(3.0899424+y(x)*(1.2067492+y(x)*(0.2659732+y(x)*(0.360768e-1+y(x)*0.45813e-2)))))) : \
    ((exp(x)/sqrt(x))*(0.39894228+y(x)*(0.1328592e-1+y(x)*(0.225319e-2+y(x)*(-0.157565e-2+y(x)*(0.916281e-2+y(x)*\
    (-0.2057706e-1+y(x)*(0.2635537e-1+y(x)*(-0.1647633e-1+y(x)*0.392377e-2))))))))))

sigma = 1.0
# Dokladna wartosc calki:
V_dok(x20) = sqrt(pi)/(2.*sigma) * exp(-x20**2/(8.*sigma**2)) * bessi0(x20**2/(8.*sigma**2))

set key spacing 2
set ylabel "V(x_{20})"
set xlabel "x_{20}"
do for [i=0:3]{
    n = 10**(i+2)
    set output "V_".n.".eps" 
    plot [][] V_dok(x) ls 1 t "V_{dok}",\
     "out.dat" i i u 1:2:3 w yerrorbars ls 2 t "V_{".n."}"
}
