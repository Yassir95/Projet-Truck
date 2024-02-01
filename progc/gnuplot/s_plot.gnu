set title 'Drivers with the greatest total distance'
set xlabel 'Total Distance'
set ylabel 'Driver Name'
set output './images/s_image.png'

set terminal pngcairo enhanced font "arial,20" size 1920,1080

set datafile separator ";"
set xrange [0<*:*]

set grid xtics mxtics ytics mytics
set log y

plot './temp/s_tmp_sorted.csv' using 1:3:4 with filledcurve lc rgb "#AAAAAACC" notitle, \
    '' using 1:2 with lines lc rgb "#0000FF" notitle