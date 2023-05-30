#!/bin/bash
# first argument is path to the hepmc file

OUT_DIR=$PWD
export RIVET_ANALYSIS_PATH=$(pwd)
export RIVET_DATA_PATH=$(pwd)

cp ${YYGEN_DIR}/source/RivetAnalysis_yy.cc $OUT_DIR
rivet-build RivetAnalysis_yy.so RivetAnalysis_yy.cc
echo "hepmc file used is $1"
name=$1
rivet -a RivetAnalysis_yy  ${name}
rivet-mkhtml Rivet.yoda:${name/.hepmc/}

