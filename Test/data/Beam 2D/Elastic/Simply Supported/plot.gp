set grid
set key above
set ylabel "P L^2 / EI" norotate

set style line 1 linecolor rgb "#ff0000"
set style line 2 linecolor rgb "#0000ff"
set style line 3 linecolor rgb "#ff00ff"

plot\
	'Test/data/Beam 2D/Elastic/Simply Supported/data.txt' using (-$1) : ($4) with lines linestyle 1 title 'v / L - Numeric',\
	'Test/data/Beam 2D/Elastic/Simply Supported/data.txt' using (-$2) : ($4) with lines linestyle 2 title 'u / L - Numeric',\
	'Test/data/Beam 2D/Elastic/Simply Supported/data.txt' using (-$3) : ($4) with lines linestyle 3 title '{/Symbol q} (rad) - Numeric',\
	'Test/data/Beam 2D/Elastic/Simply Supported/reference.dat' using ($2) : ($1) with points linestyle 1 pointtype 7 title 'v / L - Analytic',\
	'Test/data/Beam 2D/Elastic/Simply Supported/reference.dat' using ($3) : ($1) with points linestyle 2 pointtype 7 title 'u / L - Analytic',\
	'Test/data/Beam 2D/Elastic/Simply Supported/reference.dat' using ($4) : ($1) with points linestyle 3 pointtype 7 title '{/Symbol q} (rad) - Analytic'