#!/bin/bash

export RIVET_ANALYSIS_PATH=$(pwd)
export RIVET_DATA_PATH=$(pwd)

rivet-build RivetAnalysis_yy.so RivetAnalysis_yy.cc
rivet -a RivetAnalysis_yy  SC_yyll_ee_160.hepmc
rivet-mkhtml Rivet.yoda:SuperChicPy

