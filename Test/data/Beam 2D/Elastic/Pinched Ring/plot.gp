R = 1
pc = 10
pt = 20

set grid
set key above
set ylabel "P R^2 / EI" norotate

set style line 1 linecolor rgb "#ff0000"
set style line 2 linecolor rgb "#0000ff"

set terminal qt 0 title "Compression"

plot\
	'Test/data/Beam 2D/Elastic/Pinched Ring/data-compression.txt' using (-$1 / R) : (pc * $5) with lines linestyle 1 title 'v / R - Numeric',\
	'Test/data/Beam 2D/Elastic/Pinched Ring/data-compression.txt' using (+$2 / R) : (pc * $5) with lines linestyle 2 title 'u / R - Numeric',\
	'Test/data/Beam 2D/Elastic/Pinched Ring/reference-compression.dat' using ($2) : ($1) with points linestyle 1 pointtype 7 title 'v / R - Analytic',\
	'Test/data/Beam 2D/Elastic/Pinched Ring/reference-compression.dat' using ($3) : ($1) with points linestyle 2 pointtype 7 title 'u / R - Analytic'

set terminal qt 1 title "Tension"

plot\
	'Test/data/Beam 2D/Elastic/Pinched Ring/data-tension.txt' using (+$1 / R) : (pt * $5) with lines linestyle 1 title 'v / R - Numeric',\
	'Test/data/Beam 2D/Elastic/Pinched Ring/data-tension.txt' using (-$2 / R) : (pt * $5) with lines linestyle 2 title 'u / R - Numeric',\
	'Test/data/Beam 2D/Elastic/Pinched Ring/reference-tension.dat' using ($2) : ($1) with points linestyle 1 pointtype 7 title 'v / R - Analytic',\
	'Test/data/Beam 2D/Elastic/Pinched Ring/reference-tension.dat' using ($3) : ($1) with points linestyle 2 pointtype 7 title 'u / R - Analytic'
