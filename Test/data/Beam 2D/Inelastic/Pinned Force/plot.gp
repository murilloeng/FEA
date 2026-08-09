set grid
set key above
set xlabel 'v (m)'
set ylabel "{/Symbol l}"

set style line 1 linecolor rgb "#0000ff"

plot\
	'Test/data/Beam 2D/Inelastic/Pinned Force/data.txt' using (-$1) : ($2) with lines linestyle 1 notitle