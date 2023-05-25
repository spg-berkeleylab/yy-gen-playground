#!/bin/bash

for pippo in $@; do

    echo $pippo

    #check if we need to produce dot files first
    if ! [ "$pippo" = "*.dot" ]; then
	    #convert HEPMC to dot file
	    input_dot=${pippo/hepmc/dot}
        /usr/local/src/hepmc2dot/hepmc2dot.py $pippo ${input_dot}
    else
	    #input is already a dot input
	    input_dot = $pippo
    fi

    #now produce graphics
    dot -Grankdir="LR" -Tpdf -O ${input_dot}

    #now glue PDF together
    pdfunite ${input_dot}.*pdf ${input_dot}-combined.pdf

    #clean up
    rm ${input_dot}.*pdf
    mv ${input_dot}-combined.pdf ${input_dot}.pdf

done

