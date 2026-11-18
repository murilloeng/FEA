set grid
set key above
set xlabel "{/Symbol q} (rad)"
set ylabel "{/Symbol l}" norotate

set style line 1 linecolor rgb "#ff0000" pointtype 7
set style line 2 linecolor rgb "#0000ff" pointtype 7

#rotation recorded in [-pi, pi]: unwrapped
t0 = 0
k = 0
unwrap(t) = (d = t - t0, k = k + (d > pi ? -1 : (d < -pi ? 1 : 0)), t0 = t, t + 2 * pi * k)

T = 0.25

plot\
	'Test/data/Beam 3D/Elastic/Hockling Cable/data.txt' using (unwrap($2)) : ($3) with lines linestyle 2 title 'Numeric',\
	'Test/data/Beam 3D/Elastic/Hockling Cable/reference.dat' using ($1) : ($2 / 1e3 / T) with points linestyle 1 title 'Reference'
