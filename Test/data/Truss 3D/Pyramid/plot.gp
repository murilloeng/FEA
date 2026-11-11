set grid
set key above
set samples 20
set xlabel "z (m)"
set ylabel "{/Symbol l}" norotate

set style line 1 linecolor rgb "#ff0000"
set style line 2 linecolor rgb "#0000ff" dashtype 1
set style line 3 linecolor rgb "#0000ff" dashtype 2

H = 0.1
p(u) = -(1 + u / H) * (2 + u / H) * u / H

plot\
	'Test/data/Truss 3D/Pyramid/data.txt' using ($1 + H) : ($6 == 0 ? $2 : NaN) with lines linestyle 3 notitle,\
	'Test/data/Truss 3D/Pyramid/data.txt' using ($1 + H) : ($6 == 1 ? $2 : NaN) with lines linestyle 2 title 'Numeric',\
	p(x - H) with points linestyle 1 pointtype 7 title 'Exact'