set grid
set key above
set xlabel "u (m)"
set ylabel "{/Symbol l}" norotate

s(x) = 1 + x
f1(x) = 1 * (s(x) - 1)
f2(x) = 1 * s(x) * (s(x) * s(x) - 1) / 2
f3(x) = 1 * log(s(x)) / s(x)

plot\
	'Test/data/Truss 2D/Single/Linear Tension.txt' using 1 : 2 with lines linecolor 1 title 'Linear',\
	'Test/data/Truss 2D/Single/Linear Compression.txt' using 1 : 2 with lines linecolor 1 notitle,\
	'Test/data/Truss 2D/Single/Linear Tension.txt' using 1 : (f1($1)) every 10 with points linecolor 1 pointtype 7 notitle,\
	'Test/data/Truss 2D/Single/Linear Compression.txt' using 1 : (f1($1)) every 10 with points linecolor 1 pointtype 7 notitle,\
	'Test/data/Truss 2D/Single/Quadratic Tension.txt' using 1 : 2 with lines linecolor 2 title 'Quadratic',\
	'Test/data/Truss 2D/Single/Quadratic Compression.txt' using 1 : 2 with lines linecolor 2 notitle,\
	'Test/data/Truss 2D/Single/Quadratic Tension.txt' using 1 : (f2($1)) every 10 with points linecolor 2 pointtype 7 notitle,\
	'Test/data/Truss 2D/Single/Quadratic Compression.txt' using 1 : (f2($1)) every 10 with points linecolor 2 pointtype 7 notitle,\
	'Test/data/Truss 2D/Single/Logarithmic Tension.txt' using 1 : 2 with lines linecolor 3 title 'Logarithmic',\
	'Test/data/Truss 2D/Single/Logarithmic Compression.txt' using 1 : 2 with lines linecolor 3 notitle,\
	'Test/data/Truss 2D/Single/Logarithmic Tension.txt' using 1 : (f3($1)) every 10 with points linecolor 3 pointtype 7 notitle,\
	'Test/data/Truss 2D/Single/Logarithmic Compression.txt' using 1 : (f3($1)) every 10 with points linecolor 3 pointtype 7 notitle