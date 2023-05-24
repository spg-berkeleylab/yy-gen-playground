#!/bin/bash
## Generate SuperChic4 yy->ll process, shower with Pythia8

# Settings
SC_CONFIG="sc-yyll-ee-160.DAT" # input SuperChic configuration from configs/ folder

# Automatic Settings
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
YYGEN_DIR=`dirname ${SCRIPT_DIR}`

OUT_DIR=$PWD

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
echo "Preparing to run"
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
mv evrecs ${OUT_DIR}/

# Now apply patch to SC event record
echo "Patching SuperChic event record"

# Run Pythia8
echo "Showering events with Pythia8"

echo "All Done!"
