## Makefile for programs that require compilation

.PHONY: all install

all: run-shower-pythia RivetAnalysis_yy.so

install: run-shower-pythia RivetAnalysis_yy.so
	mkdir -pv ${YYGEN_DIR}/bin
	mv $^ ${YYGEN_DIR}/bin/

run-shower-pythia: run-shower-pythia.cc
	g++ -std=c++11 -O2 -pedantic -W -Wall -Wshadow $< -o $@  -lpythia8 -lLHAPDF -lHepMC -ldl

RivetAnalysis_yy.so: RivetAnalysis_yy.cc
	rivet-build $@ $<
