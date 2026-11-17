set grid
set key above
set xlabel "v (m)"
set ylabel "{/Symbol l}" norotate

set style line 1 linecolor rgb "#ff0000" pointtype 7 pointsize 0.5
set style line 2 linecolor rgb "#0000ff" pointtype 7 pointsize 0.5

plot\
	'Test/data/Beam 3D/Inelastic/Cantilever Force/data.txt' using ($2) : ($3) with lines linestyle 2 title 'Numeric',\
	'Test/data/Beam 3D/Inelastic/Cantilever Force/reference.dat' using ($2) : ($1) with points linestyle 1 pointtype 7 title 'Reference'
