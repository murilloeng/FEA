set grid
set key above
set xlabel "t (s)"

set style line 1 linecolor rgb "#ff0000"
set style line 2 linecolor rgb "#0000ff"

plot\
	'Test/data/Rigid 2D/Single Pendulum/reference.dat' every 40 using ($1) : ($2) with points linestyle 1 pointtype 7 title 'u (m) - Analytic',\
	'Test/data/Rigid 2D/Single Pendulum/reference.dat' every 40 using ($1) : ($3) with points linestyle 2 pointtype 7 title 'v (m) - Analytic',\
	'Test/data/Rigid 2D/Single Pendulum/data.txt' using ($7) : ($1) with lines linestyle 1 title 'u (m) - Numeric',\
	'Test/data/Rigid 2D/Single Pendulum/data.txt' using ($7) : ($4) with lines linestyle 2 title 'v (m) - Numeric'