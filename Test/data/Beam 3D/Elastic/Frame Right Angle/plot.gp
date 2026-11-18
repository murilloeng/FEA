set grid
set key above
set xlabel "|w| (mm)"
set ylabel "{/Symbol l}" norotate

set style line 1 linecolor rgb "#ff0000" pointtype 7
set style line 2 linecolor rgb "#0000ff" pointtype 7

M = 626.7

plot\
	'Test/data/Beam 3D/Elastic/Frame Right Angle/data.txt' using (abs($2)) : ($3) with lines linestyle 2 title 'Numeric',\
	'Test/data/Beam 3D/Elastic/Frame Right Angle/reference.dat' using (abs($1)) : ($2 / M) with points linestyle 1 title 'Reference'
