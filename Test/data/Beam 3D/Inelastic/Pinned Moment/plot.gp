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
set samples 33
set xlabel "u (m)"
set ylabel "{/Symbol l}" norotate

set style line 1 linecolor rgb "#ff0000" pointtype 7 pointsize 0.5
set style line 2 linecolor rgb "#0000ff" pointtype 7 pointsize 0.5

p3(t) = 2 * t / L <= ky ? E * I * 2 * t / L / (1.5 * My) : (3 - (ky * L / 2 / t)**2) / 3

plot\
	'Test/data/Beam 3D/Inelastic/Pinned Moment/data.txt' using ($1) : ($2) with lines linestyle 2 title 'Numeric',\
	[t=0:k(1.5 * My * 0.994) * L / 2] '+' using (t == 0 ? 0 : L * (sin(t) / t - 1)) : (p3(t)) with points linestyle 1 pointtype 7 title 'Reference'