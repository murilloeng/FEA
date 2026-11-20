set grid
set key above
set xlabel "w (m)"
set ylabel "{/Symbol l}"

set style line 1 linecolor rgb "#0000ff"

plot\
	'Test/data/Rigid 3D/Kirigami/data.txt' using ($1) : ($2) with lines linestyle 1 title 'Inner circle - Numeric'