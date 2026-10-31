w = 0.2
h = 0.4
L = 3.0
E = 2.1e11
sy = 2.5e8
l = L / 2
I = w * h**3 / 12
My = sy * w * h**2 / 6
ky = 2 * sy / E / h

set grid
set key above
set ylabel "{/Symbol l}" norotate
set samples 400

set style line 2 linecolor rgb "#0000ff" pointtype 7 pointsize 0.5

Q(p) = 1.5 * My * p / l
a(p) = Q(p) / My
ry(p) = 1 / a(p) < l ? 1 / a(p) : l
s(p) = 3 - 2 * a(p) * l > 0 ? 3 - 2 * a(p) * l : 0
u2(p) = p == 0 ? 0 : -(Q(p) * ry(p)**3 / 3 / E / I + (ry(p) < l ? ky / 4 / a(p)**2 * (16.0 / 3 - 6 * sqrt(s(p)) + 2 * s(p) * sqrt(s(p)) / 3) : 0))

plot sample\
	[t=0:1] '+' using (u2(t)) : (t) with points linestyle 2 title 'v (m) - Reference',\
	'Test/data/Beam 2D/Inelastic/Pinned Force/data.txt' using ($1) : ($2) with lines linestyle 2 title 'v (m) - Numeric'
