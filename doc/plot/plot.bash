#!/usr/bin/env bash

mkdir -p doc/plot/log
mkdir -p doc/plot/png

for data in Akiyo.QCIF Foreman.CIF Stefan.CIF Table.QCIF
do
    for search in no three_step orthogonal gradient_descent cross hexagon
    do
        bin/hw2 $1/$data -f ${data##*.} -s $search \
            > doc/plot/log/PSNR_${data%.*}_$search.log
    done
    gnuplot -c doc/plot/gnuplot.gpi ${data%.*}
done
