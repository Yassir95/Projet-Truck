set title 'Top of the most visited cities'
set xlabel 'City'
set output './images/t_image.png'

set terminal pngcairo enhanced font "arial,14" size 1920,1080
set datafile separator ';'

set yrange [0<*<500:*]

# set a tick on x every 1 units
set ytics 500

set style line 100 lc rgb "grey" lw 0.5
set grid ls 100

set style fill solid 1.0 border lt -1

set boxwidth 0.25 relativ

plot './temp/t_tmp_sorted.csv' u ($0 - 0.125):2:xticlabel(1) with boxes title "Visited", \
    '' u ($0 + 0.125):3 with boxes title "Starting Point"

