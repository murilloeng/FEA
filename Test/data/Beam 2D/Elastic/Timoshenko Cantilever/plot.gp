set grid
set key above
set ylabel "{/Symbol l}" norotate

set style line 1 linecolor rgb "#ff0000"
set style line 2 linecolor rgb "#0000ff"
set style line 3 linecolor rgb "#ff00ff"

plot\
	'Test/data/Beam 2D/Elastic/Timoshenko Cantilever/data-bending.txt' using (-$1) : ($2) with linespoints linestyle 1 pointtype 7 title 'v (m) - Shear off',\
	'Test/data/Beam 2D/Elastic/Timoshenko Cantilever/data-shear.txt' using (-$1) : ($2) with linespoints linestyle 2 pointtype 7 title 'v (m) - Shear on'