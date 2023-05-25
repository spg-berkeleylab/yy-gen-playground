#!/bin/bash
## Install versioned-software in the running container.

# List of versions (see table in README.md file for available versions)
PYTHIA_VERSION="8.245"
HERWIG_VERSION="" # latest via boostrap install
MADGRAPH_VERSION="2.9.5"
SUPERCHIC_VERSION="4.12"
CEPGEN_VERSION="1.1.0"

# Utilities and common settings
SUDO=""
USER=`whoami`
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
export YYGEN_DIR=`dirname ${SCRIPT_DIR}`
INITIAL_DIR=${PWD}
LOGFILE="${INITIAL_DIR}/${install-sw.log}"
rm -f ${LOGFILE}
if [ ${USER} != "root" ]; then
    # if running as non-root user, need to 'sudo'
    SUDO="sudo"
fi
SRC_DIR="/usr/local/src"

# Pythia
echo "===> Installing Pythia ${PYTHIA_VERSION}" | tee -a ${LOGFILE}
export PYTHIA_DIR="${SRC_DIR}/pythia/pythia${PYTHIA_VERSION//./}"
cd ${PYTHIA_DIR}
${SUDO} make install >> ${LOGFILE} 2>&1

# Herwig
#echo "===> Installing latest Herwig"  | tee -a ${LOGFILE}
#HERWIG_DIR=${SRC_DIR}/herwig/
#source ${HERWIG_DIR}/bin/activate

# Madgraph
echo "===> Installing Madgraph ${MADGRAPH_VERSION}"  | tee -a ${LOGFILE}
export MADGRAPH_DIR="${SRC_DIR}/madgraph/MG5_aMC_v${MADGRAPH_VERSION//./_}"
cd ${MADGRAPH_DIR}
export PATH=${MADGRAPH_DIR}/bin:${PATH}
echo "export PATH=${MADGRAPH_DIR}/bin:${PATH}" >> ${HOME}/.bashrc

# Superchic
echo "===> Installing Superchic ${SUPERCHIC_VERSION}"  | tee -a ${LOGFILE}
export SUPERCHIC_DIR="${SRC_DIR}/superchic/superchic${SUPERCHIC_VERSION}"
export PATH=${SUPERCHIC_DIR}/bin:${PATH}
export LD_LIBRARY_PATH=${SUPERCHIC_DIR}/lib:${LD_LIBRARY_PATH}
echo "export PATH=${SUPERCHIC_DIR}/bin:${PATH}" >> ${HOME}/.bashrc
echo "export LD_LIBRARY_PATH=${SUPERCHIC_DIR}/lib:${LD_LIBRARY_PATH}" >> ${HOME}/.bashrc

# CepGen
echo "===> Installing CepGen ${CEPGEN_VERSION}" | tee -a ${LOGFILE}
export CEPGEN_DIR="${SRC_DIR}/cepgen/cepgen"
cd ${CEPGEN_DIR}
#.... check tag, if different from requested version, check it out and compile
cepgen_ver=`git describe --tags`
if [ "${cepgen_ver}" != ${CEPGEN_VERSION} ]; then
    # get needed version and recompile
    echo "Pre-compiled version (${cepgen_ver}) does not match requested one (${CEPGEN_VERSION}). Switching branch/tag and recompiling."  | tee -a >> ${LOGFILE}
    git checkout ${CEPGEN_VERSION} >> ${LOGFILE} 2>&1
    cd build
    rm -rf *
    cmake ../ >> ${LOGFILE} 2>&1
    make -j4 >> ${LOGFILE} 2>&1
    cd ../
fi
cd build
${SUDO} make install >> ${LOGFILE} 2>&1

# Local executables
echo "===> Compiling yy-gen-playground programs in 'source/'"  | tee -a ${LOGFILE}
cd ${YYGEN_DIR}/source
make >> ${LOGFILE} 2>&1
make install >> ${LOGFILE} 2>&1
export PATH=${YYGEN_DIR}/bin:${PATH}


# Back to initial folder
cd ${INITIAL_DIR}