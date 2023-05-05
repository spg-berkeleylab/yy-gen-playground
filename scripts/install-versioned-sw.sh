#!/bin/bash
## Install versioned-software in the running container.

# List of versions (see table in README.md file for available versions)
PYTHIA_VERSION="8.245"
HERWIG_VERSION="" # latest via boostrap install
MADGRAPH_VERSION="2.9.5"
SUPERCHIC_VERSION="4.12"

# Utilities and common settings
SUDO=""
USER=`whoami`
if [ ${USER} != "root" ]; then
    # if running as non-root user, need to 'sudo'
    SUDO="sudo"
fi
SRC_DIR="/usr/local/src"

# Pythia
echo "===> Installing Pythia ${PYTHIA_VERSION}"
PYTHIA_DIR="${SRC_DIR}/pythia${PYTHIA_VERSION//./}"
cd ${PYTHIA_DIR}
${SUDO} make install

# Herwig
echo "===> Installing latest Herwig"
HERWIG_DIR=${SRC_DIR}/herwig/
source ${HERWIG_DIR}/bin/activate

# Madgraph
echo "===> Installing Madgraph ${MADGRAPH_VERSION}"
MADGRAPH_DIR="${SRC_DIR}/MG5_aMC_v${MADGRAPH_VERSION//./_}"
cd ${MADGRAPH_DIR}
export PATH=${MADGRAPH_DIR}/bin:${PATH}
echo "export PATH=${MADGRAPH_DIR}/bin:${PATH}" >> ${HOME}/.bashrc

# Superchic
echo "===> Installing Superchic ${SUPERCHIC_VERSION}"
SUPERCHIC_DIR="${SRC_DIR}/superchic${SUPERCHIC_VERSION}"
export PATH=${SUPERCHIC_DIR}/bin:${PATH}
export LD_LIBRARY_PATH=${SUPERCHIC_DIR}/LIB:${LD_LIBRARY_PATH}
echo "export PATH=${SUPERCHIC_DIR}/bin:${PATH}" >> ${HOME}/.bashrc
echo "export LD_LIBRARY_PATH=${SUPERCHIC_DIR}/LIB:${LD_LIBRARY_PATH}" >> ${HOME}/.bashrc
