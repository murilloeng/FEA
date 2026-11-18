set grid
set key above
set xlabel "|w| (mm)"
set ylabel "{/Symbol l}" norotate

set style line 1 linecolor rgb "#ff0000" pointtype 7
set style line 2 linecolor rgb "#0000ff" pointtype 7

plot\
	'Test/data/Beam 3D/Elastic/Cantilever Right Angle/data.txt' using (1e3 * abs($3)) : ($4) with lines linestyle 2 title 'Numeric',\
	'Test/data/Beam 3D/Elastic/Cantilever Right Angle/reference.dat' using ($1) : ($2) with points linestyle 1 title 'Reference'
