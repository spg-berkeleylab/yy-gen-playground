#!/bin/bash
## Generate SuperChic4 yy->ll process, shower with Pythia8

# Settings
MG_CONFIG_PROCCARD="mg-yyll-ee-160-proc_card.conf" # input Madgraph commands
MG_CONFIG_RUNCARD="mg-yyll-ee-160-run_card.dat" # input Madgraph settings
PY8_CONFIG="py8-mg-ee.conf" # pythia8 configuration: elastic

# Automatic Settings
OUT_DIR=$PWD
RUN_PREFIX=`basename $PWD` # used to rename stuff, by default use folder name

# Settings printout
echo "=========================="
echo "Running Madgraph + Pythia8"
echo "=========================="
echo "Run folder: ${OUT_DIR}"
echo "Output folder: ${OUT_DIR}"
echo "Source folder: ${YYGEN_DIR}"
echo "MG Config: ${MG_CONFIG}"
echo "Py8 Config: ${PY8_CONFIG}"
echo "=========================="

# Prepare folders and create process folder
echo "Running madgraph"
cd ${OUT_DIR}
cp ${YYGEN_DIR}/configs/${MG_CONFIG_PROCCARD} ./
mg5_aMC -f ${MG_CONFIG}
cp ${YYGEN_DIR}/configs/${MG_CONFIG_RUNCARD} ./PROC_sm_0/Cards/run_card.dat
cp ${YYGEN_DIR}/configs/mg-param_card-atlas.conf ./PROC_sm_0/Cards/param_card.dat

# Running MG
echo "launch PROC_sm_0" | mg5_aMC

# Run Pythia8
echo "Showering events with Pythia8"
cp ${YYGEN_DIR}/configs/${PY8_CONFIG} .
gzip -d PROC_sm_0/Events/run_01/unweighted_events.lhe.gz
${YYGEN_DIR}/bin/run-shower-pythia PROC_sm_0/Events/run_01/unweighted_events.lhe ${PY8_CONFIG} ${RUN_PREFIX}.hepmc

# Creating rivet plots
echo "Creating Rivet plots"
## TODO..

# Visualize a few events
echo "Creating event visualization"
${YYGEN_DIR}/scripts/create-graph-pdf.sh ${RUN_PREFIX}.hepmc

echo "All Done!"

