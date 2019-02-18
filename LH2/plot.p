set ylabel 'Momentum loss [MeV/c]'
set nokey  
set term x11 enhanced font 'Arial-Bold'
set termoption font "Arial-Bold,20"
set key on bottom
show key

#set tics font ",18" 
#set tics scale 0.1 



set xdata time
set timefmt '%d/%m/%Y-%H:%M:%S'
#set xtics  "21.09","27.09" 

plot '3-140.txt' using 1:2 title '3mm - 140 MeV/c' with points pointtype 7 ps 1 lc rgb "blue", \
     '6-140.txt' using 1:2 title '6mm - 140 MeV/c' with points pointtype 7 ps 1 lc rgb "#00BFFF"

set yrange [13:GPVAL_DATA_Y_MAX+1.0]
set format x "%d/%m\n%H:%M"
set xrange ['21/09/2017-20:00:00':'27/09/2017-02:00:00']

set label front "MICE online\nLiquid hydrogen filling" at graph 0.05, graph 0.9 #font ",14"
show label

#set font 'Arial,35'
replot
set terminal png size 1024,768  #enhanced font 'Arial-Bold'
set output 'LH2_filling.png'

replot
set terminal pdf
set output 'LH2_filling.pdf'

replot
