set grid
set key above
set view equal xyz
set xlabel "x (m)"
set ylabel "y (m)"
set zlabel "z (m)"

set style line 1 linecolor rgb "#ff0000"
set style line 2 linecolor rgb "#0000ff"
set style line 3 linecolor rgb "#000000"

splot\
	'Test/data/Beam 3D/Elastic/Path Independence/deformed-a.txt' using ($1) : ($3) : ($5) with lines linestyle 3 title 'Undeformed',\
	'Test/data/Beam 3D/Elastic/Path Independence/deformed-a.txt' using ($2) : ($4) : ($6) with linespoints linestyle 1 pointtype 7 title 'Deformed - P_2 then P_3',\
	'Test/data/Beam 3D/Elastic/Path Independence/deformed-b.txt' using ($2) : ($4) : ($6) with lines linestyle 2 linewidth 2 title 'Deformed - P_3 then P_2',\
	'Test/data/Beam 3D/Elastic/Path Independence/data-a.txt' using (1 + $1) : ($2) : ($3) with lines linestyle 1 dashtype 2 title 'Tip path - P_2 then P_3',\
	'Test/data/Beam 3D/Elastic/Path Independence/data-b.txt' using (1 + $1) : ($2) : ($3) with lines linestyle 2 dashtype 2 title 'Tip path - P_3 then P_2'

pause mouse close
