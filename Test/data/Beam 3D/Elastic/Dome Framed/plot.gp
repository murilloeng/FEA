set grid
set key above
set xlabel "w (m)"
set ylabel "{/Symbol l}" norotate

set style line 1 linecolor rgb "#ff0000" pointtype 7
set style line 2 linecolor rgb "#0000ff" pointtype 7
set style line 3 linecolor rgb "#ff00ff" pointtype 7

plot\
	'Test/data/Beam 3D/Elastic/Dome Framed/data.txt' using (-$3) : ($4) with lines linestyle 2 title 'Numeric',\
	'Test/data/Beam 3D/Elastic/Dome Framed/reference-2.dat' using ($1) : ($2) with points linestyle 1 title 'Reference - path 2',\
	'Test/data/Beam 3D/Elastic/Dome Framed/reference-1.dat' using ($1) : ($2) with points linestyle 3 title 'Reference - path 1'
