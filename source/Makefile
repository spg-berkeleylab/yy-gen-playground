## Makefile for programs that require compilation

.PHONY: all install

all: run-shower-pythia RivetAnalysis_yy.so convert-hepmc-2to3

install: run-shower-pythia RivetAnalysis_yy.so convert-hepmc-2to3
	mkdir -pv ${YYGEN_DIR}/bin
	mv $^ ${YYGEN_DIR}/bin/

run-shower-pythia: run-shower-pythia.cc
	g++ -std=c++11 -O2 -pedantic -W -Wall -Wshadow $< -o $@  -lpythia8 -lLHAPDF -lHepMC -ldl

convert-hepmc-2to3: convert-hepmc-2to3.cc
	g++ -O2 $< -o $@ -lHepMC3 -lHepMC

RivetAnalysis_yy.so: RivetAnalysis_yy.cc
	rivet-build $@ $<
