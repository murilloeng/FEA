R = 100

set grid
set key above
set ylabel "P R^2 / EI" norotate

set style line 1 linecolor rgb "#ff0000"
set style line 2 linecolor rgb "#0000ff"
set style line 3 linecolor rgb "#ff00ff"

plot\
	8.97 with lines linestyle 3 dashtype 2 title 'Reference Limit load',\
	'Test/data/Beam 2D/Elastic/Deep Arch/data.txt' using (+$1 / R) : ($3) with lines linestyle 1 title 'u / R - Numeric',\
	'Test/data/Beam 2D/Elastic/Deep Arch/data.txt' using (-$2 / R) : ($3) with lines linestyle 2 title 'v / R - Numeric'