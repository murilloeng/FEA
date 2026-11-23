set grid
set key above
set xlabel "{/Symbol l}"

set style line 1 linecolor rgb "#ff0000"
set style line 2 linecolor rgb "#0000ff"
set style line 3 linecolor rgb "#00ff00"

L = 1.00
nt = 3
d1 = 1. / 3; d2 = 2. / 3; d3 = 2. / 3
m1 = 2. / 3; m2 = -2. / 3; m3 = 1. / 3
n1 = m2 * d3 - m3 * d2; n2 = m3 * d1 - m1 * d3; n3 = m1 * d2 - m2 * d1
ud(p) = p == 0 ? 0 : L * (sin(p) / p - 1)
un(p) = p == 0 ? 0 : L * (1 - cos(p)) / p

plot\
	ud(2 * pi * nt * x) with points linestyle 1 pointtype 7 title 'u . d (m) - Analytic',\
	un(2 * pi * nt * x) with points linestyle 2 pointtype 7 title 'u . n (m) - Analytic',\
	'Test/data/Beam 3D/Elastic/Roll Up/data.txt' using ($7) : ($1 * d1 + $2 * d2 + $3 * d3) with lines linestyle 1 title 'u . d (m) - Numeric',\
	'Test/data/Beam 3D/Elastic/Roll Up/data.txt' using ($7) : ($1 * n1 + $2 * n2 + $3 * n3) with lines linestyle 2 title 'u . n (m) - Numeric',\
	'Test/data/Beam 3D/Elastic/Roll Up/data.txt' using ($7) : ($1 * m1 + $2 * m2 + $3 * m3) with lines linestyle 3 title 'u . m (m) - Numeric'
