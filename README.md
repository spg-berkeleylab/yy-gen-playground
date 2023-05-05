# Docker container for generator tuning and development

Develop a docker container for quickly and easily test solution for common (and new) yy generators.
The docker image is available as `spagan/yy-gen-playground` on dockerhub (see instructions below).

## Getting started
Clone this repository locally, download the docker image and create a run folder to easily share the work across containers:
```
git clone https://github.com/spg-berkeleylab/yy-gen-playground.git
docker pull XXXXX
mkdir run
```

To create a new container based on this image you can simply run:
```bash
docker run -it -v ${PWD}/yy-gen-playground:/work/yy-gen-playground -v ${PWD}/run:/work/run -- spagan/yy-gen-playground:latest /bin/bash
```

Once the container has started, you need to install the specific version of versioned-software. Edit first the `${HOME}/yy-gen-playground/scripts/install-versioned-sw.sh` if you need to adjust the version of the software you'd like to have available. Then execute the script
```bash
. yy-gen-playground/scripts/install-versioned-sw.sh
```
to actually install the software system-wide and make it available in the container. When asked for the user password, use `yygen`.
The script also defines a set of environment variables called `XXX_DIR` that point to the location of the specific version, e.g. `PYTHIA_DIR`, etc..

If you want to run with a different version, the easiest is to create a separate container from the same image. This also allows to keep each setup separately and launch the corresponding container when needed.



## Available Software
The image contains several software packages that are useful for these studies. They can be grouped into two categories:
* pre-installed software: already installed and available with a specific version;
* versioned software: multiple versions (potentially) available and compiled, that needs to be installed when starting a new container from this image (see [this section](#getting-started-container-environment-and-version-setup)).

The *versioned software* is pre-compiled and available in the `/usr/local/src/` folder. The list of available sofotware, versions and the environment variable that will contain the location of the version setup is:

| Type | Name | Versions | Location  |
|------|------|----------|-----------|
| Generator | SuperChic | 4.12(*), 4.2 | `${SUPERCHIC_DIR}`|
| Generator | Madgraph  | 2.9.5(*), 3.5.0.alpha | `${MADGRAPH_DIR}` |
| Parton Shower | Pythia8 | 8.245(*), 8.307, 8.309 | `${PYTHIA_DIR}` |
| Parton Shower | Herwig  | latest | `${HERWIG_DIR}` |

(*) = default version

The list of *pre-installed software* available system-wide is:

| Type | Name | Versions | Source / Note  |
|------|------|----------|--------|
| PDF       | LHAPDF    | 6.5.4 | hepstore/rivet |
| PDF       | APFEL     | 3.0.6 | [web](https://github.com/scarrazza/apfel) |
| EDM       | HepMC     | 3.2.6, 2.06.11 | hepstore/rivet, [web](http://hepmc.web.cern.ch/hepmc/) |
| Analysis      | Rivet   | 3.1.8 | hepstore/rivet |
| Analysis      | Yoda    | 3.1.8 | hepstore/rivet |
| Analysis      | ROOT    | 6.28.02 | hepstore/rivet |
| Analysis      | HighFive | 2.7.1 | hepstore/rivet |
| Analysis      | hepmc2dot | custom | [web](https://github.com/spagangriso/hepmc2dot) |



Several other utility packages are also installed (e.g. python3, latex, general c++ building tools, etc..) and not listed above.

## Utility scripts and programs
A set of utility scripts and program is available in this repository to facilitate generating and analyzing common processes of interest.

* `scripts/install-versioned-sw.sh` make versioned software available system-wide (see instructions above)
* `scripts/create-graph-pdf.sh` read an HepMC file an create a PDF with event visualization (1 event per page)
