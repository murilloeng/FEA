set grid
set key above
set ylabel "{/Symbol l}" norotate

set style line 1 linecolor rgb "#ff0000"
set style line 2 linecolor rgb "#0000ff"
set style line 3 linecolor rgb "#ff00ff"

plot\
	'Test/data/Beam 3D/Elastic/Cantilever Force/data.txt' using (-$1) : ($3) with lines linestyle 1 title 'u (m) - Numeric',\
	'Test/data/Beam 3D/Elastic/Cantilever Force/data.txt' using (-$2) : ($3) with lines linestyle 2 title 'v (m) - Numeric',\
	'Test/data/Beam 3D/Elastic/Cantilever Force/reference.dat' using (-$3) : ($1) with points linestyle 1 pointtype 7 title 'u (m) - Analytic',\
	'Test/data/Beam 3D/Elastic/Cantilever Force/reference.dat' using (-$2) : ($1) with points linestyle 2 pointtype 7 title 'v (m) - Analytic'