set grid
set key above
set xlabel "u (m)"
set ylabel "{/Symbol l}" norotate

H = 0.1
Ls = 1.0
k = 0.5 * Ls / H
s(u) = 1 + u / Ls
p(u) = -(1 + u / H) * (2 + u / H) * u / H - k * s(u) * (s(u) * s(u) - 1) / 2

plot\
	'Test/data/Truss 2D/von Mises Snap Through/data.txt' using 1 : 2 with lines linecolor rgb "#0000ff" title 'Numeric',\
	'Test/data/Truss 2D/von Mises Snap Through/data.txt' using 1 : (p($1)) every 5 with points linecolor rgb "#ff0000" pointtype 7 title 'Exact'
