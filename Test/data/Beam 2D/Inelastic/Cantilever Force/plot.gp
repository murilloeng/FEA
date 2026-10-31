w = 0.2
h = 0.4
L = 3.0
E = 2.1e11
sy = 2.5e8
I = w * h**3 / 12
My = sy * w * h**2 / 6
ky = 2 * sy / E / h
k(M) = M <= My ? M / E / I : ky / sqrt(3 - 2 * M / My)

set grid
set key above
set ylabel "{/Symbol l}" norotate
set samples 400

set style line 1 linecolor rgb "#ff0000" pointtype 7 pointsize 0.5
set style line 2 linecolor rgb "#0000ff" pointtype 7 pointsize 0.5
set style line 3 linecolor rgb "#ff00ff" pointtype 7 pointsize 0.5

P(p) = 1.5 * My * p / L
a(p) = P(p) / My
ry(p) = 1 / a(p) < L ? 1 / a(p) : L
s(p) = 3 - 2 * a(p) * L > 0 ? 3 - 2 * a(p) * L : 0
t3(p) = p == 0 ? 0 : -(P(p) * ry(p)**2 / 2 / E / I + (ry(p) < L ? ky / a(p) * (1 - sqrt(s(p))) : 0))
u2(p) = p == 0 ? 0 : -(P(p) * ry(p)**3 / 3 / E / I + (ry(p) < L ? ky / 4 / a(p)**2 * (16.0 / 3 - 6 * sqrt(s(p)) + 2 * s(p) * sqrt(s(p)) / 3) : 0))

set multiplot layout 2, 1

plot sample\
	[t=0:1] '+' using (u2(t)) : (t) with points linestyle 2 title 'v (m) - Reference',\
	'Test/data/Beam 2D/Inelastic/Cantilever Force/data.txt' using ($2) : ($4) with lines linestyle 2 title 'v (m) - Numeric'

plot sample\
	[t=0:1] '+' using (t3(t)) : (t) with points linestyle 3 title '{/Symbol q} (rad) - Reference',\
	'Test/data/Beam 2D/Inelastic/Cantilever Force/data.txt' using ($3) : ($4) with lines linestyle 3 title '{/Symbol q} (rad) - Numeric'
