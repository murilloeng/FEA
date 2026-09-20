set grid
set key above
set ylabel "{/Symbol l}" norotate

set style line 1 linecolor rgb "#ff0000" pointtype 7
set style line 2 linecolor rgb "#0000ff" pointtype 7
set style line 3 linecolor rgb "#ff00ff" pointtype 7

plot\
	'Test/data/Beam 3D/Elastic/Lee Frame/data.txt' using (+$1) : ($3) with lines linestyle 1 title 'Numeric - u (m)',\
	'Test/data/Beam 3D/Elastic/Lee Frame/data.txt' using (-$2) : ($3) with lines linestyle 2 title 'Numeric - v (m)',\
	'Test/data/Beam 3D/Elastic/Lee Frame/reference-u.dat' using ($1) : ($2) with points linestyle 1 title 'Reference - u (m)',\
	'Test/data/Beam 3D/Elastic/Lee Frame/reference-v.dat' using ($1) : ($2) with points linestyle 2 title 'Reference - v (m)'