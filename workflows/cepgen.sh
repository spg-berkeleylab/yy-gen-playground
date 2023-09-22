#!/bin/bash
## Generate SuperChic4 yy->ll process, shower with Pythia8

# Settings
CG_CONFIG="sc-yyll-ee.DAT" # input CepGen configuration from configs/ folder
PY8_CONFIG="py8-sc-ee.conf" # pythia8 configuration: elastic

# Automatic Settings
OUT_DIR=$PWD
RUN_PREFIX=`basename $PWD` # used to rename stuff, by default use folder name
RIVET_ANALYSIS_PATH=$PWD
RIVET_DATA_PATH=$PWD

# Settings printout
echo "=========================="
echo "Running SuperChic4 + Pythia8"
echo "=========================="
echo "Run folder: ${SUPERCHIC_DIR}/bin"
echo "Output folder: ${PWD}"
echo "Source folder: ${YYGEN_DIR}"
echo "Config: ${SC_CONFIG}"
echo "=========================="

# Prepare running folder
for pippo in build CepGenAddOns Cards CepGen External CepGenProcesses/; do ln -s ${CEPGEN_DIR}/$pippo .; done
cp ${YYGEN_DIR}/config/${CG_CONFIG} .

# Run
cepgen --config ${CG_CONFIG}
mv test.lhe ${RUN_PREFIX}.lhe

# Run Pythia8
echo "Showering events with Pythia8"
cp ${YYGEN_DIR}/configs/${PY8_CONFIG} .
${YYGEN_DIR}/bin/run-shower-pythia ${RUN_PREFIX}.lhe ${PY8_CONFIG} ${RUN_PREFIX}.hepmc

# Creating rivet plots
echo "Creating Rivet plots"
echo "Compiling Rivet analysis"
cd ${OUT_DIR}
cp ${YYGEN_DIR}/source/RivetAnalysis_yy.cc .
cp ${YYGEN_DIR}/source/RivetAnalysis_yy.plot .
rivet-build RivetAnalysis_yy.so RivetAnalysis_yy.cc
rivet -a RivetAnalysis_yy  ${RUN_PREFIX}.hepmc
rivet-mkhtml Rivet.yoda:${RUN_PREFIX}
echo "End of Rivet analysis!"

# Visualize the first 10 events
echo "Creating event visualization"
hepmc2dot.py ${RUN_PREFIX}.hepmc ${RUN_PREFIX}.hepmc.dot 10
create-graph-pdf.sh ${RUN_PREFIX}.hepmc.dot

echo "All Done!"
