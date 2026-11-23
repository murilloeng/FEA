set grid
set key above
set xlabel "w (m)"
set ylabel "P / P_{cr}"

set style line 1 linecolor rgb "#ff0000"
set style line 2 linecolor rgb "#0000ff"

plot\
	[0:*] 1 with lines linestyle 1 title 'P_{cr} - Timoshenko',\
	'Test/data/Beam 3D/Elastic/Lateral Buckling/data.txt' using ($2) : ($4) with lines linestyle 2 title 'Numeric'
