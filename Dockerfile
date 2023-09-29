# Docker build settings
######### 
# Decide if to setup the container as a normal user ('yyfriend') or as 'root' user.
# Possible values: 'user', 'root'
ARG MAIN_USER=user

# General software
######### 
FROM hepstore/rivet:3.1.8-hepmc3 AS yygenplayground-base

# Python virtual environment
#RUN apt -y install python3-pip python3-venv && \
# python3 -m venv /home/ubuntu/py-venv && \
# chown -R ubuntu:ubuntu /home/ubuntu/py-venv && \
# echo '# Setup python3 virtual environment' >> /home/ubuntu/.bashrc && \
# echo 'source /home/ubuntu/py-env/bin/activate' >> /home/ubuntu/.bashrc
RUN apt-get update && apt -y install libgsl-dev poppler-utils graphviz libglu1-mesa libzip-dev vim

# Get specific software with fixed versions
#########
# APFEL
RUN mkdir /code && cd /code && \
 git clone https://github.com/scarrazza/apfel.git && cd apfel && \
 git checkout tags/3.0.6 && \
 ./configure && make && make install && \
 cd ${HOME} && rm -rf /code

# HepMC2 (HepMC3 alrady in base image)
 RUN mkdir /code && cd /code && \
  wget http://hepmc.web.cern.ch/hepmc/releases/hepmc2.06.11.tgz && tar xzf hepmc2.06.11.tgz && \
  mkdir build && cd build && cmake -DCMAKE_INSTALL_PREFIX=/usr/local -Dmomentum:STRING=GEV -Dlength:STRING=MM ../HepMC-2.06.11/ && \
  make -j 4 && make install && \
  cd ${HOME} && rm -rf /code

# hepmc2dot (visualization)
RUN cd /usr/local/src/ && git clone https://github.com/spagangriso/hepmc2dot.git && \
 cd hepmc2dot
# && pip3 install -r requirements.txt


# Get and compile versioned software. Will install when launching the container.
#########

# Pythia 
RUN mkdir -pv /usr/local/src/pythia
RUN cd /usr/local/src/pythia && \
 wget https://pythia.org/download/pythia82/pythia8245.tgz && tar xzf pythia8245.tgz && \
 cd pythia8245 && ./configure --prefix=/usr/local --with-hepmc3 --with-lhapdf6 && make -j4
RUN cd /usr/local/src/pythia && \
 wget https://pythia.org/download/pythia83/pythia8306.tgz && tar xzf pythia8306.tgz && \
 cd pythia8306 && ./configure --prefix=/usr/local --with-hepmc3 --with-lhapdf6 && make -j4
RUN cd /usr/local/src/pythia && \
 wget https://pythia.org/download/pythia83/pythia8310.tgz && tar xzf pythia8310.tgz && \
 cd pythia8310 && ./configure --prefix=/usr/local --with-hepmc3 --with-lhapdf6 && make -j4

# Herwig
#RUN mkdir -pv /usr/local/src/herwig
#ENV LHAPDF_DATA_PATH="/usr/local/share/LHAPDF"
#RUN cd /usr/local/src/herwig && \
# wget https://herwig.hepforge.org/downloads/herwig-bootstrap && chmod +x herwig-bootstrap 
# && \
# ./herwig-bootstrap -j 4 --without-madgraph --with-hepmc=/usr/local --with-lhapdf=`lhapdf-config --prefix` --with-yoda=`yoda-config --prefix` --with-rivet=`rivet-config --prefix`  /usr/local/src/herwig/

# Madgraph
RUN mkdir -pv /usr/local/src/madgraph
RUN cd /usr/local/src/madgraph && \
 wget https://launchpad.net/mg5amcnlo/lts/2.9.x/+download/MG5_aMC_v2.9.5.tar.gz && tar xzf MG5_aMC_v2.9.5.tar.gz
RUN cd /usr/local/src/madgraph && \
 wget https://launchpad.net/mg5amcnlo/3.0/3.5.x/+download/MG5_aMC_v3.5.1.tar.gz && tar xzf MG5_aMC_v3.5.1.tar.gz

# Superchic
RUN mkdir -pv /usr/local/src/superchic
RUN cd /usr/local/src/superchic && \
 wget https://superchic.hepforge.org/superchic4.12.tar.gz && tar xzf superchic4.12.tar.gz && \
 cd superchic4.12 && make
RUN cd /usr/local/src/superchic && \
 wget https://superchic.hepforge.org/superchic4.2.tar.gz && tar xzf superchic4.2.tar.gz && \
 cd superchic4.2 && make

# CepGen
RUN mkdir -pv /usr/local/src/cepgen
RUN cd /usr/local/src/cepgen && \
 git clone https://github.com/cepgen/cepgen.git && \
 cd cepgen && git checkout master && \
 CEPGEN_SOURCES=`pwd -P` && \
 mkdir $CEPGEN_SOURCES/build && cd $CEPGEN_SOURCES/build && \
 cmake $CEPGEN_SOURCES && make -j4

# Sherpa (master branch)
RUN mkdir -pv /usr/local/src/sherpa
RUN cd /usr/local/src/sherpa && \
 git clone https://gitlab.com/sherpa-team/sherpa.git && \
 cd sherpa && mkdir build && cd build && \
 cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr/local -DSHERPA_ENABLE_RIVET=ON -DSHERPA_ENABLE_HEPMC3=ON -S ../ -B . && \
 cmake --build . -j 4

# System-wide settings
######### 
# LHAPDF sets, including superchic SF_MSHT20qed_nnlo
RUN lhapdf install \
  MMHT2014lo68cl \
  MSTW2008nnlo68cl MSTW2008nlo68cl MSTW2008lo68cl \
  LUXlep-NNPDF31_nlo_as_0118_luxqed LUXqed17_plus_PDF4LHC15_nnlo_100 LUXqed17_plus_PDF4LHC15_nnlo_30 LUXqed_plus_PDF4LHC15_nnlo_100 \
  CT14qed_proton CT14nnlo CT14nlo CT14llo \
  cteq6l1 cteq61 cteq66 \
  MMHT2015qed_nlo_elastic MMHT2015qed_nlo_inelastic MMHT2015qed_nnlo_elastic MMHT2015qed_nnlo_inelastic \
  NNPDF23_lo_as_0119_qed NNPDF23_nlo_as_0119 NNPDF23_nlo_as_0119_qed NNPDF23_nnlo_as_0118 NNPDF23_nnlo_as_0118_qed NNPDF23_nnlo_as_0119 NNPDF23_nnlo_as_0119_qed
RUN mkdir -pv /pdfin && cd /pdfin && \
 wget https://superchic.hepforge.org/SF_MSHT20qed_nnlo.tar.gz && \
 cd /usr/local/share/LHAPDF && \
 tar xzf /pdfin/SF_MSHT20qed_nnlo.tar.gz && \
 rm -rf /pdfin

# Now build an image supporting a user
######### 
FROM yygenplayground-base AS yygenplayground-user
# sudo, set user password as well for sudo
RUN apt -y install sudo && \
 useradd -Ms /bin/bash yyfriend && \
 usermod -aG sudo yyfriend && \
 chown -R yyfriend:yyfriend /work && \
 chown -R yyfriend:yyfriend /usr/local/src/ && \
 chown -R yyfriend:yyfriend /usr/local/share/ && \
 echo "yyfriend:yygen" | chpasswd

# set user stuff
RUN mkdir /home/yyfriend
RUN echo 'alias ll="ls -ltrhF --color=auto"' >> /home/yyfriend/.bashrc
RUN echo "export PATH=/usr/local/src/hepmc2dot/:${PATH}" >> /home/yyfriend/.bashrc
RUN chown -R yyfriend:yyfriend /home/yyfriend

# Switch to ubuntu user
USER yyfriend

# Instead, if we want just the root user...
######### 
FROM yygenplayground-base AS yygenplayground-root

# Finally, choose the final stage based on the build arguments
######### 
FROM yygenplayground-${MAIN_USER} AS yygenplayground-final
