H = 0.1

set grid
set key above
set xlabel "z (m)"
set ylabel "{/Symbol l}" norotate

set style line 1 linecolor rgb "#0000ff" dashtype 1
set style line 2 linecolor rgb "#0000ff" dashtype 2

plot\
	'Test/data/Truss 3D/Pyramid/data.txt' using ($3 + H) : ($5 == 0 ? $4 : NaN) with lines linestyle 2 notitle,\
	'Test/data/Truss 3D/Pyramid/data.txt' using ($3 + H) : ($5 == 1 ? $4 : NaN) with lines linestyle 1 title 'Numeric'