set grid
set key above
set xlabel "{/Symbol q} (rad)"
set ylabel "{/Symbol l}" norotate

set style line 1 linecolor rgb "#ff0000" pointtype 7
set style line 2 linecolor rgb "#0000ff" pointtype 7
set style line 3 linecolor rgb "#ff00ff" pointtype 7

#rotation recorded in [-pi, pi]: unwrapped
t0 = 0
k = 0
unwrap(t) = (d = t - t0, k = k + (d > pi ? -1 : (d < -pi ? 1 : 0)), t0 = t, t + 2 * pi * k)

plot\
	'Test/data/Beam 3D/Elastic/Deployable Ring/data.txt' using (unwrap(-$1)) : ($2) with lines linestyle 2 title 'Numeric',\
	'Test/data/Beam 3D/Elastic/Deployable Ring/reference-beam.dat' using ($1) : (100 * $2) with points linestyle 1 title 'Reference - beam',\
	'Test/data/Beam 3D/Elastic/Deployable Ring/reference-shell.dat' using ($1) : (100 * $2) with points linestyle 3 title 'Reference - shell'
