set grid
set key above
set ylabel "{/Symbol l}"
set xlabel '{/Symbol q} (rad)'

set style line 1 linecolor rgb "#0000ff"

plot 'Test/data/Beam 2D/Inelastic/Pinned Bending/data.txt' using ($1) : ($2) with lines linestyle 1 notitle 