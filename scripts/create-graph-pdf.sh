#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

for pippo in $@; do

    #check if we need to produce dot files first
    if ! [[ $pippo == *.dot ]]; then
        #convert HEPMC to dot file
        input_dot="${pippo}.dot"
        echo "$pippo (via ${input_dot})  => ${input_dot}.pdf"
        ${SCRIPT_DIR}/hepmc2dot.py $pippo ${input_dot}
    else
        #input is already a dot input
        input_dot=$pippo
        echo "$pippo => ${input_dot}.pdf"
    fi

    #now produce graphics
    dot -Grankdir="LR" -Tpdf -O ${input_dot}

    #now glue PDF together
    list_pdfs=`ls -1v ${input_dot}.*pdf`
    pdfunite ${list_pdfs} ${input_dot}-combined.pdf

    #clean up
    rm ${input_dot}.*pdf
    mv ${input_dot}-combined.pdf ${input_dot}.pdf

done

