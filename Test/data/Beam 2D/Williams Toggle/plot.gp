set grid
set key above
set multiplot layout 2, 1
set ylabel "{/Symbol l}" norotate

set style line 1 linecolor rgb "#ff0000" pointtype 7
set style line 2 linecolor rgb "#0000ff" pointtype 7
set style line 3 linecolor rgb "#ff00ff" pointtype 7

plot\
	'Test/data/Beam 2D/Williams Toggle/data.txt' using (-$1) : ($2) with lines linestyle 2 title 'Numeric',\
	'Test/data/Beam 2D/Williams Toggle/test.txt' using (+$1) : ($2) with points linestyle 1 pointsize 0.5 title 'Reference'