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

t3(p) = k(1.5 * My * p) * L
u1(p) = t3(p) == 0 ? 0 : L * (sin(t3(p)) / t3(p) - 1)
u2(p) = t3(p) == 0 ? 0 : L * (1 - cos(t3(p))) / t3(p)

set multiplot layout 2, 1

plot sample\
	[t=0:0.995] '+' using (u1(t)) : (t) with points linestyle 1 title 'u (m) - Reference',\
	[t=0:0.995] '+' using (u2(t)) : (t) with points linestyle 2 title 'v (m) - Reference',\
	'Test/data/Beam 2D/Inelastic/Cantilever Bending/data.txt' using ($1) : ($4) with lines linestyle 1 title 'u (m) - Numeric',\
	'Test/data/Beam 2D/Inelastic/Cantilever Bending/data.txt' using ($2) : ($4) with lines linestyle 2 title 'v (m) - Numeric'

plot sample\
	[t=0:0.995] '+' using (t3(t)) : (t) with points linestyle 3 title '{/Symbol q} (rad) - Reference',\
	'Test/data/Beam 2D/Inelastic/Cantilever Bending/data.txt' using ($3) : ($4) with lines linestyle 3 title '{/Symbol q} (rad) - Numeric'
