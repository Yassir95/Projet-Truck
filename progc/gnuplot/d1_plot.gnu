set title 'Drivers with the most different routes'
set xlabel 'Routes Amount'
set ylabel 'Driver Name'
set output './images/d1_image.png'

set terminal pngcairo enhanced font "arial,20" size 1920,1080
set datafile separator ';'
set xrange [0<*:*]

set ytics 1 nomirror
set xtics nomirror

set style line 100 lc rgb "#AAAAAA" lw 0.5
set style fill solid 1.0 border lt -1
set grid ls 100

width = 0.8
offset = 0.5
set offsets 0,0,offset-width/2.,offset

plot './temp/d1_tmp.csv' using (offset*$2):0:(offset*$2):(width/2.):($0+1):ytic(1) with boxxy lc var notitle