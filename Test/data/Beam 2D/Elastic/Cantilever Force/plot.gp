set grid
set key above
set ylabel "{/Symbol l}" norotate

set style line 1 linecolor rgb "#ff0000"
set style line 2 linecolor rgb "#0000ff"
set style line 3 linecolor rgb "#ff00ff"

plot\
	'Test/data/Beam 2D/Cantilever Force/data.txt' using (-$1) : ($4) with lines linestyle 1 title 'u (m) - Numeric',\
	'Test/data/Beam 2D/Cantilever Force/data.txt' using (-$2) : ($4) with lines linestyle 2 title 'v (m) - Numeric',\
	'Test/data/Beam 2D/Cantilever Force/data.txt' using (-$3) : ($4) with lines linestyle 3 title '{/Symbol q} (rad) - Numeric',\
	'Test/data/Beam 2D/Cantilever Force/reference.dat' using (-$3) : ($1) with points linestyle 1 pointtype 7 title 'u (m) - Analytic',\
	'Test/data/Beam 2D/Cantilever Force/reference.dat' using (-$2) : ($1) with points linestyle 2 pointtype 7 title 'v (m) - Analytic',\
	'Test/data/Beam 2D/Cantilever Force/reference.dat' using (-$4) : ($1) with points linestyle 3 pointtype 7 title '{/Symbol q} (rad) - Analytic'