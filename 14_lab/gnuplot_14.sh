set terminal post enhanced colour solid font 25
reset

# SKRYPT GENERUJE WYKRESY NA PODSTAWIE TRZECH PLIKOW:
# 1. PLIK uniform.dat zawiera 4 kolumny: x_i, x_{i+1}, x_{i+2}, x_{i+3}
#       - W pierwszej serii danych: dla 1. rozkladu U1(0, 1).
#       - W drugiej serii danych (po dwoch pustych liniach): dla 2. rozkladu U2(0, 1).
#       - W trzeciej serii danych: dla 3. rozkladu U3(0, 1).
#     Uwaga: te wyniki skrypt narysuje w formie trzech "potrojnych" wykresow.
#     Mozna je zamienic na 9 osobnych wykresow -- nizej zaznaczono, co trzeba odkomentowac/zakomentowac.

# 2. PLIK 3D.dat zawiera 3 kolumny: x_i, y_i, z_i
#       - W pierwszej serii danych: dla powierzchni sfery (zad. 1.2.1).
#       - W drugiej serii danych: w kuli 3D (zad. 1.2.2).

# 3. PLIK hist.dat zawiera 3 kolumny: (R_j+R_{j-1})/2, n_j, g_j
#       - W pierwszej serii danych: dla N = 2000.
#       - W drugiej serii danych: dla N = 10000.
#       - W trzeciej serii danych: dla N = 10000000.


reset
set terminal post enhanced colour solid font 25
k_max = 10.
set key outside top center horizontal
x_min = 0.
x_max = 1.
dx = (x_max-x_min)/k_max
set ylabel "n_j, g_j"
set xlabel "r"
a = (x_max-x_min)/k_max/2.
set xtics out font ",20" offset 0, 0.5 x_min, dx, x_max
do for [t=1:11] {
  set arrow t from x_min+2*t*a, graph 0 to x_min+2*t*a, graph 1 nohead lt 0 lw 2 lc rgb "gray30"
}
do for [i=0:2]{
  set output "hist".(i+1).".eps"
  plot [x_min:x_max][0:] "hist.dat" i i u 1:2 w boxes lc rgb "dark-spring-green" fs pattern 1 t "n_j", \
    "" i i u 1:3 w boxes lc rgb "red" lw 5 fs fill empty t "g_j"
}
