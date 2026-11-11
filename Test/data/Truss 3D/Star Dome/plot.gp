set grid
set key above
set xlabel "u"
set ylabel "{/Symbol l}" norotate

plot\
	'Test/data/Truss 3D/Star Dome/data.txt' using 1 : 2 with lines linecolor rgb "#0000ff" title 'Numeric',\
	'Test/data/Truss 3D/Star Dome/reference.dat' using 1 : 2 every 3 with points linecolor rgb "#ff0000" pointtype 7 title 'Reference'
