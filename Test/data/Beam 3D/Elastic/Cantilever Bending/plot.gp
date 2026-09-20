set grid
set key above
set xlabel "{/Symbol l}"

set style line 1 linecolor rgb "#ff0000"
set style line 2 linecolor rgb "#0000ff"
set style line 3 linecolor rgb "#ff00ff"
set style line 3 linecolor rgb "#00ff00"

L = 1.00
u1(p) = p == 0 ? 0 : L * (sin(p) / p - 1)
u2(p) = p == 0 ? 0 : L * (1 - cos(p)) / p

plot\
	u1(2 * pi * x) with points linestyle 1 title 'u (m) - Analytic',\
	u2(2 * pi * x) with points linestyle 2 title 'v (m) - Analytic',\
	'Test/data/Beam 3D/Elastic/Cantilever Bending/data.txt' using ($3) : ($1) with lines linestyle 1 title 'u (m) - Numeric',\
	'Test/data/Beam 3D/Elastic/Cantilever Bending/data.txt' using ($3) : ($2) with lines linestyle 2 title 'v (m) - Numeric'