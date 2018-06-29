#!/usr/bin/env fish

mkdir -p doc/plot/log
mkdir -p doc/plot/png

for data in Akiyo.QCIF Foreman.CIF Stefan.CIF Table.QCIF
    for search in no three_step orthogonal gradient_descent cross hexagon
        bin/hw2 $argv[1]/$data -f (echo $data | cut -d. -f2) -s $search \
            > doc/plot/log/PSNR_(echo $data | cut -d. -f1)_$search.log
    end
    gnuplot -c doc/plot/gnuplot.gpi (echo $data | cut -d. -f1)
end
