set grid
set key above
set samples 20
set multiplot layout 2, 1
set xlabel '{/Symbol q} (rad)'
set ylabel '{/Symbol l}' norotate

set style line 1 linecolor rgb "#0000ff"
set style line 2 linecolor rgb "#ff0000"

a = 1.00e-02

plot\
	'Test/data/Rigid 2D/Spring Buckling/data.txt' using ($1) : ($4) with lines linestyle 1 title 'Numeric',\
	x / sin(a + x) with points linestyle 2  pointtype 7 title 'Analytic'

set ylabel ""

plot\
	'Test/data/Rigid 2D/Spring Buckling/data.txt' using ($1) : ($2) with lines linestyle 1 title 'Numeric (u)',\
	'Test/data/Rigid 2D/Spring Buckling/data.txt' using ($1) : ($3) with lines linestyle 2 title 'Numeric (v)',\
	-(1 - cos(x)) * cos(a) - sin(x) * sin(a) with points linestyle 1 pointtype 7 title 'Analytic (u)',\
	-(1 - cos(x)) * sin(a) + sin(x) * cos(a) with points linestyle 2 pointtype 7 title 'Analytic (v)'