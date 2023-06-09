#!/bin/bash
## Generate SuperChic4 yy->ll process, shower with Pythia8

# Settings
SC_CONFIG="sc-yyll-ee.DAT" # input SuperChic configuration from configs/ folder
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

# Prepare folders
echo "Preparing to run SuperChic"
cd ${SUPERCHIC_DIR}/bin
mkdir -pv outputs inputs evrecs
cp ${YYGEN_DIR}/configs/${SC_CONFIG} ./

# Running init
echo "Running SuperChic initialization"
${SUPERCHIC_DIR}/bin/init < ${SC_CONFIG}

# Running event generation
echo "Running SuperChic event generation"
${SUPERCHIC_DIR}/bin/superchic < ${SC_CONFIG}

# Move output
echo "Moving SuperChic outputs and configuration to ${OUT_DIR}"
mv ${SC_CONFIG} ${OUT_DIR}/
mv outputs ${OUT_DIR}/
mv inputs ${OUT_DIR}/
mv evrecs/evrecout.dat evrecs/${RUN_PREFIX}.lhe
mv evrecs ${OUT_DIR}/
cd ${OUT_DIR}


# Now apply patch to SC event record
#echo "Patching SuperChic event record"

# Run Pythia8
echo "Showering events with Pythia8"
cp ${YYGEN_DIR}/configs/${PY8_CONFIG} .
${YYGEN_DIR}/bin/run-shower-pythia evrecs/${RUN_PREFIX}.lhe ${PY8_CONFIG} ${RUN_PREFIX}.hepmc

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
