## Makefile for programs that require compilation

.PHONY: all install

all: run-shower-pythia

install: run-shower-pythia
	mkdir -pv ${YYGEN_DIR}/bin
	mv $^ ${YYGEN_DIR}/bin/

run-shower-pythia: run-shower-pythia.cc
	g++ -O2 -ansi -pedantic -W -Wall -Wshadow $< -o $@  -lpythia8 -lLHAPDF -lHepMC -ldl
#g++ -O2 -ansi -pedantic -W -Wall -Wshadow -I$(PYTHIA_DIR)/include main20.cc -o main20.exe  -L$(PYTHIA_DIR)/lib -lpythia8 -lHepMC -lLHAPDF -ldl

