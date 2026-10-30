set grid
set key above
set logscale y
set xlabel "{/Symbol q} (rad)"

set style line 1 linecolor rgb "#ff0000"
set style line 2 linecolor rgb "#0000ff"

plot\
	'Test/data/Beam 2D/Elastic/Rigid Rotation/data.txt' using ($1) : ($2 + 1e-30) with lines linestyle 1 title 'U / (EI / L)',\
	'Test/data/Beam 2D/Elastic/Rigid Rotation/data.txt' using ($1) : ($3 + 1e-30) with lines linestyle 2 title '|f| / (EI / L^2)'