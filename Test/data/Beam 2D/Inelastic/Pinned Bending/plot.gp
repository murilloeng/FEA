w = 0.2
h = 0.4
L = 3.0
E = 2.1e11
sy = 2.5e8
I = w * h**3 / 12
ky = 2 * sy / E / h
My = sy * w * h**2 / 6
k(M) = M <= My ? M / E / I : ky / sqrt(3 - 2 * M / My)

set grid
set key above
set parametric
set samples 100
set ylabel "{/Symbol l}" norotate

set style line 1 linecolor rgb "#ff0000" pointtype 7 pointsize 0.5
set style line 2 linecolor rgb "#0000ff" pointtype 7 pointsize 0.5

t3(p) = k(1.5 * My * p) * L / 2

set trange [0:0.995]

plot\
	'Test/data/Beam 2D/Inelastic/Pinned Bending/data.txt' using ($1) : ($2) with lines linestyle 2 title '{/Symbol q} (rad) - Numeric',\
	t3(t), t with points linestyle 1 title '{/Symbol q} (rad) - Reference'