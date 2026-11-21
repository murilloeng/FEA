set grid
set key above
set multiplot layout 2, 1
set xlabel "t (s)"

set style line 1 linecolor rgb "#ff0000"
set style line 2 linecolor rgb "#0000ff"

plot\
	'Test/data/Rigid 2D/Double Pendulum/reference.dat' every 40 using ($1) : ($2) with points linestyle 1 pointtype 7 title 'u_1 (m) - Analytic',\
	'Test/data/Rigid 2D/Double Pendulum/reference.dat' every 40 using ($1) : ($3) with points linestyle 2 pointtype 7 title 'v_1 (m) - Analytic',\
	'Test/data/Rigid 2D/Double Pendulum/data.txt' using ($13) : ($1) with lines linestyle 1 title 'u_1 (m) - Numeric',\
	'Test/data/Rigid 2D/Double Pendulum/data.txt' using ($13) : ($4) with lines linestyle 2 title 'v_1 (m) - Numeric'

plot\
	'Test/data/Rigid 2D/Double Pendulum/reference.dat' every 40 using ($1) : ($4) with points linestyle 1 pointtype 7 title 'u_2 (m) - Analytic',\
	'Test/data/Rigid 2D/Double Pendulum/reference.dat' every 40 using ($1) : ($5) with points linestyle 2 pointtype 7 title 'v_2 (m) - Analytic',\
	'Test/data/Rigid 2D/Double Pendulum/data.txt' using ($13) : ($7) with lines linestyle 1 title 'u_2 (m) - Numeric',\
	'Test/data/Rigid 2D/Double Pendulum/data.txt' using ($13) : ($10) with lines linestyle 2 title 'v_2 (m) - Numeric'