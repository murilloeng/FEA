set grid
set key above
set xlabel "{/Symbol l}"

set style line 1 linecolor rgb "#ff0000"
set style line 2 linecolor rgb "#0000ff"
set style line 3 linecolor rgb "#ff00ff"

plot\
	'Test/data/Beam 2D/Diamond Frame/data-compression.txt' using ($4) : (-$1) with lines linestyle 1 title 'u (m) - Numeric',\
	'Test/data/Beam 2D/Diamond Frame/data-compression.txt' using ($4) : (-$2) with lines linestyle 2 title 'v (m) - Numeric',\
	'Test/data/Beam 2D/Diamond Frame/data-compression.txt' using ($4) : (-$3) with lines linestyle 3 title '{/Symbol q} (rad) - Numeric',\
	'Test/data/Beam 2D/Diamond Frame/reference-compression.dat' using ($1) : ($3) with points linestyle 1 pointtype 7 title 'u (m) - Analytic',\
	'Test/data/Beam 2D/Diamond Frame/reference-compression.dat' using ($1) : ($2) with points linestyle 2 pointtype 7 title 'v (m) - Analytic',\
	'Test/data/Beam 2D/Diamond Frame/reference-compression.dat' using ($1) : (pi / 4 - $4) with points linestyle 3 pointtype 7 title '{/Symbol q} (rad) - Analytic'