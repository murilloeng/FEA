set grid
set key above
set multiplot layout 2, 1
set xlabel "{/Symbol l}"

set style line 1 linecolor rgb "#ff0000" pointtype 7 pointsize 0.5
set style line 2 linecolor rgb "#0000ff" pointtype 7 pointsize 0.5
set style line 3 linecolor rgb "#ff00ff" pointtype 7 pointsize 0.5

t3(p) = p
u1(p) = (sin(p) / p - 1)
u2(p) = (1 - cos(p)) / p

plot\
	u1(2 * pi * x) with points linestyle 1 title 'u (m) - Reference',\
	u2(2 * pi * x) with points linestyle 2 title 'v (m) - Reference',\
	'Test/data/Beam 2D/Elastic/Cantilever Bending/data.txt' using ($4) : ($1) with lines linestyle 1 title 'u (m) - Numeric',\
	'Test/data/Beam 2D/Elastic/Cantilever Bending/data.txt' using ($4) : ($2) with lines linestyle 2 title 'v (m) - Numeric'

plot\
	t3(2 * pi * x) with points linestyle 3 title '{/Symbol q} (rad) - Reference',\
	'Test/data/Beam 2D/Elastic/Cantilever Bending/data.txt' using ($4) : ($3) with lines linestyle 3 title '{/Symbol q} (rad) - Numeric'