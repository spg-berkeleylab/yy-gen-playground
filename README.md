# Docker container for generator tuning and development

Develop a docker container for quickly and easily test solution for common (and new) yy generators.
The docker image is available as `spagan/yy-gen-playground` on dockerhub (see instructions below).

## Getting started
Clone this repository locally, download the docker image and create a `run` folder to easily share the work across containers:
```
git clone https://github.com/spg-berkeleylab/yy-gen-playground.git
docker pull spagan/yy-gen-playground
mkdir run
```

To create a new container based on this image you can simply run:
```bash
docker run -it -v ${PWD}/yy-gen-playground:/work/yy-gen-playground -v ${PWD}/run:/work/run -- spagan/yy-gen-playground:latest /bin/bash
```

Once the container has started, you need to set up the specific version of versioned-software. Edit `${HOME}/yy-gen-playground/scripts/install-versioned-sw.sh` if you need to adjust the version of the software you'd like to have available. Then execute the script
```bash
. yy-gen-playground/scripts/install-versioned-sw.sh
```
When asked for the user password, use `yygen`.
The script also defines a set of environment variables called `XXX_DIR` that point to the location of the specific version, e.g. `PYTHIA_DIR`, etc.. (see the Table below).

If you want to run with a different version, the easiest is to create a separate container from the same image. This also allows to keep each setup separate and launch the corresponding container when needed.

Note that for technical reasons `Herwig` is not available in this container, but they provide a separate image that you can get with:
```bash
docker pull patrickkirchgaesser/herwig
```
and follow instructions on the [MCNet 2019 Herwig Tutorial](https://phab.hepforge.org/w/mcnet2019/).

## Generating Samples: full chain examples
In this section we give some guidelines/examples on how to generate and analyze samples within the container.
Note: it's ok and sometimes needed to generate some sample in one container, save the output in the `run` folder and then open it again in another container.

A few notes on the structure of the repository:
* Example scripts carrying out specific "workflows" are provided in the `workflow` folder.
* Configuration files are stored in the `configs` folder.
* General utility scripts are in the `scripts` folder.
* Executables built from the `source` folder are installed in the `bin` folder.

These workflow scripts are meant to be either executed as-is, possibly tweaking or changing the configuration files, or just taken as example to produce new workflow or play interactively using similar commands. 

### Superchic + Pythia8
As an example on how to use the software in the container, the `workflow/sc-py8.sh` script
provides a functional example that can be configured/tuned editing the initial part or used as example to execute commands interactively. 

The script will:
* generate SuperChic LHE files for the given process in the configuration file (e.g. `configs/sc-yyll-ee-160.DAT`);
* shower produced events using Pythia8 and the provided configuration (e.g. `configs/py8-sc-ee.conf`);
* run a dedicated rivet routine to get plots that can be compared to other setups;
* run a visualization script that produces a PDF with the structure of the event record for the events produced.

## Available Software
The image contains several software packages that are useful for these studies. They can be grouped into two categories:
* pre-installed software: already installed and available with a specific version;
* versioned software: multiple versions (potentially) available and compiled, that needs to be installed when starting a new container from this image (see [this section](#getting-started-container-environment-and-version-setup)).

The *versioned software* is pre-compiled and available in the `/usr/local/src/` folder. The list of available sofotware, versions and the environment variable that will contain the location of the version setup is:

| Type | Name | Versions | Location  |
|------|------|----------|-----------|
| Generator | SuperChic | 4.12(*), 4.2 | `${SUPERCHIC_DIR}`|
| Generator | Madgraph  | 2.9.5(*), 3.5.0 | `${MADGRAPH_DIR}` |
| Parton Shower | Pythia8 | 8.245(*), 8.307, 8.309 | `${PYTHIA_DIR}` |
| Parton Shower | Herwig  | latest | `patrickkirchgaesser/herwig` image |
| Generator     | CepGen(*)  | 1.1.0  | `${CEPGEN_DIR}` |

(*) = default version

TODO: Look into LPAIR

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

### Available PDFs
PDFs are installed via the LHAPDF package. They are available in the `/usr/local/share/LHAPDF/` folder.
To install a new set in the running container, just do:
```bash
lhapdf install pdfname
```
where `pdfname` is the name of the PDF (use `lhapdf list` to see a list of known PDFs).


## Utility scripts and programs
A set of utility scripts and program is available in this repository to facilitate generating and analyzing common processes of interest.

* `scripts/install-versioned-sw.sh` make versioned software available system-wide (see instructions above)
* `scripts/create-graph-pdf.sh` read an HepMC file an create a PDF with event visualization (1 event per page)

## Building the container
If you need to modify the container, edit the `Dockerfile` and rebuild the container from the main folder containing this file:
```bash
docker build -t yy-gen-playground .
```
