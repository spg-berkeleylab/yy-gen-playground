FROM hepstore/rivet:3.1.8-hepmc3

# General software
######### 
# Python virtual environment
#RUN apt -y install python3-pip python3-venv && \
# python3 -m venv /home/ubuntu/py-venv && \
# chown -R ubuntu:ubuntu /home/ubuntu/py-venv && \
# echo '# Setup python3 virtual environment' >> /home/ubuntu/.bashrc && \
# echo 'source /home/ubuntu/py-env/bin/activate' >> /home/ubuntu/.bashrc
RUN apt -y install libgsl-dev

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

# hepm2dot (visualization)
RUN cd /usr/local/src/ && git clone https://github.com/spagangriso/hepmc2dot.git && \
 cd hepmc2dot
# && pip3 install -r requirements.txt


# Get and compile versioned software. Will install when launching the container.
#########

# Pythia 
RUN mkdir -pv /usr/local/src/pythia
RUN cd /usr/local/src/pythia && \
 wget https://pythia.org/download/pythia83/pythia8307.tgz && tar xzf pythia8307.tgz && \
 cd pythia8307 && ./configure && make -j4
RUN cd /usr/local/src/pythia && \
 wget https://pythia.org/download/pythia82/pythia8245.tgz && tar xzf pythia8245.tgz && \
 cd pythia8245 && ./configure && make -j4
RUN cd /usr/local/src/pythia && \
 wget https://pythia.org/download/pythia83/pythia8309.tgz && tar xzf pythia8309.tgz && \
 cd pythia8309 && ./configure && make -j4

# Herwig
RUN mkdir -pv /usr/local/src/herwig
ENV LHAPDF_DATA_PATH="/usr/local/share/LHAPDF"
RUN cd /usr/local/src/herwig && \
 wget https://herwig.hepforge.org/downloads/herwig-bootstrap && chmod +x herwig-bootstrap 
# && \
# ./herwig-bootstrap -j 4 --without-madgraph --with-hepmc=/usr/local --with-lhapdf=`lhapdf-config --prefix` --with-yoda=`yoda-config --prefix` --with-rivet=`rivet-config --prefix`  /usr/local/src/herwig/

# Madgraph
RUN mkdir -pv /usr/local/src/madgraph
RUN cd /usr/local/src/madgraph && \
 wget https://launchpad.net/mg5amcnlo/lts/2.9.x/+download/MG5_aMC_v2.9.5.tar.gz && tar xzf MG5_aMC_v2.9.5.tar.gz
RUN cd /usr/local/src/madgraph && \
 wget https://launchpad.net/mg5amcnlo/3.0/3.5.x/+download/MG5_aMC_v3.5.0.tar.gz && tar xzf MG5_aMC_v3.5.0.tar.gz

# Superchic
RUN mkdir -pv /usr/local/src/superchic
RUN cd /usr/local/src/superchic && \
 wget https://superchic.hepforge.org/superchic4.12.tar.gz && tar xzf superchic4.12.tar.gz && \
 cd superchic4.12 && make
RUN cd /usr/local/src/superchic && \
 wget https://superchic.hepforge.org/superchic4.2.tar.gz && tar xzf superchic4.2.tar.gz && \
 cd superchic4.2 && make

# CepGen
#RUN mkdir -pv /usr/local/src/cepgen
#RUN cd /usr/local/src/cepgen && \
# wget https://cepgen.hepforge.org/downloads?f=cepgen-1.0.1.tar.gz && tar xzf cepgen-1.0.1.tar.gz && \
# cd cepgen

# System-wide settings
######### 
# sudo, set user password as well for sudo
RUN apt -y install sudo && \
 useradd -Ms /bin/bash yyfriend && \
 usermod -aG sudo yyfriend && \
 echo "yyfriend:yygen" | chpasswd

# Switch to ubuntu user
USER yyfriend
