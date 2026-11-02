set grid
set key above

d = 'Test/data/Truss 3D/Space Truss/data.txt'
r = 'Test/data/Truss 3D/Space Truss/reference.dat'

set terminal qt 0 title "F - w"
set xlabel "w / h"
set ylabel "F / EA"
plot\
	d using (-$4) : 5 with lines linecolor rgb "#0000ff" title 'Numeric',\
	r using 3 : 4 every 10 with points linecolor rgb "#ff0000" pointtype 7 title 'Reference'

set terminal qt 1 title "F - v"
set xlabel "v / h"
set ylabel "F / EA"
plot\
	d using (-$2) : 5 with lines linecolor rgb "#0000ff" title 'Numeric',\
	r using 2 : 4 every 10 with points linecolor rgb "#ff0000" pointtype 7 title 'Reference'

set terminal qt 2 title "v - w"
set xlabel "w / h"
set ylabel "v / h"
plot\
	d using (-$4) : (-$2) with lines linecolor rgb "#0000ff" title 'Numeric',\
	r using 3 : 2 every 10 with points linecolor rgb "#ff0000" pointtype 7 title 'Reference'
