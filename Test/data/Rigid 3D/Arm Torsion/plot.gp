set grid
set key above
set xlabel "{/Symbol l}"

set style line 1 linecolor rgb "#ff0000"
set style line 2 linecolor rgb "#0000ff"

d = 0.50
u2(p) = d * (cos(p) - 1)
u3(p) = d * sin(p)

plot\
	u2(2 * pi * x) with points linestyle 1 pointtype 7 title 'v (m) - Analytic',\
	u3(2 * pi * x) with points linestyle 2 pointtype 7 title 'w (m) - Analytic',\
	'Test/data/Rigid 3D/Arm Torsion/data.txt' using ($3) : ($1) with lines linestyle 1 title 'v (m) - Numeric',\
	'Test/data/Rigid 3D/Arm Torsion/data.txt' using ($3) : ($2) with lines linestyle 2 title 'w (m) - Numeric'