#!/usr/bin/env fish

for data in Akiyo.QCIF Foreman.CIF Stefan.CIF Table.QCIF
    for search in no three_step orthogonal gradient_descent
        bin/hw2 data/$data -f (echo $data | cut -d. -f2) -s $search \
            > doc/plot/log/PSNR_(echo $data | cut -d. -f1)_$search.log
    end
    gnuplot -c doc/plot/gnuplot.gpi (echo $data | cut -d. -f1)
end
