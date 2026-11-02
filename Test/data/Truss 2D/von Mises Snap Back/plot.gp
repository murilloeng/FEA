set grid
set key above
set xlabel "u (m)"
set ylabel "{/Symbol l}" norotate

H = 0.1
p(u) = -(1 + u / H) * (2 + u / H) * u / H

plot\
	'Test/data/Truss 2D/von Mises Snap Back/data.txt' using 1 : 3 with lines linecolor rgb "#0000ff" title 'Apex',\
	'Test/data/Truss 2D/von Mises Snap Back/data.txt' using 2 : 3 with lines linecolor rgb "#00aa00" title 'Loaded node',\
	'Test/data/Truss 2D/von Mises Snap Back/data.txt' using 1 : (p($1)) every 5 with points linecolor rgb "#ff0000" pointtype 7 title 'Exact'
