set grid
set key above
set multiplot layout 2, 1
set ylabel "{/Symbol l}" norotate

set style line 1 linecolor rgb "#ff0000" pointtype 7
set style line 2 linecolor rgb "#0000ff" pointtype 7
set style line 3 linecolor rgb "#ff00ff" pointtype 7

plot\
	'Test/data/Beam 2D/Lee Frame/data.txt' using (+$1) : ($4) with lines linestyle 1 title 'Numeric - u (m)',\
	'Test/data/Beam 2D/Lee Frame/data.txt' using (-$2) : ($4) with lines linestyle 2 title 'Numeric - v (m)',\
	'Test/data/Beam 2D/Lee Frame/reference-u.dat' using ($1) : ($2) with points linestyle 1 title 'Reference - u (m)',\
	'Test/data/Beam 2D/Lee Frame/reference-v.dat' using ($1) : ($2) with points linestyle 2 title 'Reference - v (m)'

plot\
	'Test/data/Beam 2D/Lee Frame/data.txt' using (+$3) : ($4) with lines linestyle 3 title '{/Symbol q} (rad)'