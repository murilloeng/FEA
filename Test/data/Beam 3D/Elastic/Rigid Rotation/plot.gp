set grid
set key above
set logscale y
set format y "10^{%L}"
set xlabel "t"

set style line 1 linecolor rgb "#ff0000"
set style line 2 linecolor rgb "#0000ff"
set style line 3 linecolor rgb "#00ff00"

plot\
	'Test/data/Beam 3D/Elastic/Rigid Rotation/data.txt' using ($1) : (abs($2) + 1e-300) with lines linestyle 1 title 'U / (EI / L)',\
	'Test/data/Beam 3D/Elastic/Rigid Rotation/data.txt' using ($1) : (abs($3) + 1e-300) with lines linestyle 2 title '|x - x_{rigid}| / L',\
	'Test/data/Beam 3D/Elastic/Rigid Rotation/data.txt' using ($1) : (abs($4) + 1e-300) with lines linestyle 3 title '|R - R_{rigid}|'
