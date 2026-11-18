set grid
set key above
set ylabel "{/Symbol l}" norotate

set style line 1 linecolor rgb "#ff0000" pointtype 7
set style line 2 linecolor rgb "#0000ff" pointtype 7
set style line 3 linecolor rgb "#ff00ff" pointtype 7

P = 50

plot\
	'Test/data/Beam 3D/Elastic/Cantilever Arc/data.txt' using ($1) : ($4) with lines linestyle 1 title 'Numeric - u (m)',\
	'Test/data/Beam 3D/Elastic/Cantilever Arc/data.txt' using ($2) : ($4) with lines linestyle 2 title 'Numeric - v (m)',\
	'Test/data/Beam 3D/Elastic/Cantilever Arc/data.txt' using ($3) : ($4) with lines linestyle 3 title 'Numeric - w (m)',\
	'Test/data/Beam 3D/Elastic/Cantilever Arc/reference-u.dat' using (-$1) : ($2 / P) with points linestyle 1 title 'Reference - u (m)',\
	'Test/data/Beam 3D/Elastic/Cantilever Arc/reference-v.dat' using (+$1) : ($2 / P) with points linestyle 2 title 'Reference - v (m)',\
	'Test/data/Beam 3D/Elastic/Cantilever Arc/reference-w.dat' using (+$1) : ($2 / P) with points linestyle 3 title 'Reference - w (m)'
