set grid
set key above
set multiplot layout 2, 1
set xlabel "{/Symbol l}"

set style line 1 linecolor rgb "#ff0000"
set style line 2 linecolor rgb "#0000ff"
set style line 3 linecolor rgb "#ff00ff"

plot\
	'Test/data/Beam 2D/cantilever bending/data.txt' using ($4) : ($1) with lines linestyle 1 title 'u (m)',\
	'Test/data/Beam 2D/cantilever bending/data.txt' using ($4) : ($2) with lines linestyle 2 title 'v (m)'

plot\
	'Test/data/Beam 2D/cantilever bending/data.txt' using ($4) : ($3) with lines linestyle 3 title '{/Symbol q} (rad)'