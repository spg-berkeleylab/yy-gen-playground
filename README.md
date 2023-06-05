# Docker container for generator tuning and development

Develop a docker container for quickly and easily test solution for common (and new) yy generators.
The docker image is available as `spagan/yy-gen-playground` on dockerhub (see instructions below).

Pre-requisites: git, [docker](https://docs.docker.com/get-docker/)

See also the section below ([Docker Installation](#docker-installation)) on installing the docker software for a few useful hints.

This container was developed in the context of the [Workshop on the modeling of photon-induced processes](https://conference.ippp.dur.ac.uk/event/1193/)

## Getting started
Clone this repository locally, download the docker image and create a `run` folder to easily share the work across containers:
```
git clone https://github.com/spg-berkeleylab/yy-gen-playground.git
docker pull spagan/yy-gen-playground:0.6
mkdir run
```

To create a new container based on this image you can simply run:
```bash
docker run -it -u `id -u $USER`:`id -g` -v ${PWD}/yy-gen-playground:/work/yy-gen-playground -v ${PWD}/run:/work/run -- spagan/yy-gen-playground:0.6 /bin/bash
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
docker pull herwigcollaboration/herwig-7.3:7.3.0
```
and follow the instructions on the dedicated [Herwig Tutorial](https://phab.hepforge.org/w/herwigtutorial/) and the additional instructions specific for this workshop [here](https://phab.hepforge.org/w/herwigtutorial/photoninduced/) (credit to: Aidin Masouminia).

To have a shell, you can also do:
```bash
docker run -it -u `id -u $USER`:`id -g` --rm -v ${PWD}/yy-gen-playground:/work/yy-gen-playground -v ${PWD}/run:/work/run -- herwigcollaboration/herwig-7.3:7.3.0 /bin/bash
```
and then execute, e.g.
```bash
source activate
cd /work
```
but we advise to follow the instructions in the tutorial links above instead.

## Generating Samples: full chain examples
In this section we give some guidelines/examples on how to generate and analyze samples within the container.
Note: it's ok and sometimes needed to generate some sample in one container, save the output in the `run` folder and then open it again in another container.

A few notes on the structure of the repository:
* Example scripts carrying out specific "workflows" are provided in the `workflow` folder.
* Configuration files are stored in the `configs` folder.
* General utility scripts are in the `scripts` folder.
* Executables built from the `source` folder are installed in the `bin` folder.

These workflow scripts are meant to be either executed as-is, possibly tweaking or changing the configuration files, or just taken as example to produce new workflow or play interactively using similar commands. 

### Workflows 

**Superchic + Pythia8**

As an example on how to use the software in the container, the `workflow/sc-py8.sh` script
provides a functional example that can be configured/tuned editing the initial part or used as example to execute commands interactively. 

The script will:
* generate SuperChic LHE files for the given process in the configuration file (e.g. `configs/sc-yyll-ee-160.DAT`);
* shower produced events using Pythia8 and the provided configuration (e.g. `configs/py8-sc-ee.conf`);
* run a dedicated rivet routine to get plots that can be compared to other setups;
* run a visualization script that produces a PDF with the structure of the event record for the events produced.

**Madgraph + Pythia8**
An example workflow for Madgraph and Pythia8 is in the `workflow/mg-py8.sh` script.
The script is analogous to the Superchic + Pythia workflow in terms of features.

### Configurations
A quick guide to available configurations in the `config/` folder:
* `sc-yyll-*` are SuperChic configuration files for the $\gamma\gamma\to ll$ process (ee=elastic; sd,ds=single-diffractive; dd=double-diffractive).
* `mg-yyll-*` are Madgraph configuration files for the $\gamma\gamma\to ll$ process (ee=elastic; sd,ds=single-diffractive; dd=double-diffractive).
* `sherpa-yyll-*` are Sherpa configuration files for the $\gamma\gamma\to ll$ process.
* `py8-sc-*` are pythia8 configurations to shower SuperChic LHE events.
* `py8-mg-*` are pythia8 configurations to shower MadGraph LHE events.

### Analysis

A simple rivet analysis script is available in the `source` directory. The analysis `RivetAnalysis_yy.cc` contains basic plots and selction for $\gamma\gamma\to ll$ process. To run only the rivet analysis one can use `run_analysis.sh` script in the `scripts` directory, providing the hepmc file as an argument:
```bash
sh scripts/run_analysis.sh filename.hepmc
```
After any change is made in the analysis script `RivetAnalysis_yy.cc` you need to compile it:
```bash
rivet-build RivetAnalysis_yy.so RivetAnalysis_yy.cc
```
Then to run the analysis on your favorite hepmc file run:
```bash
rivet -a RivetAnalysis_yy  filename.hepmc
```
To create plots execute:
```bash
rivet-mkhtml Rivet.yoda:filename
```
Now, plots in png and pdf formats are created, you can also see all of them by opening `RivetAnalysis_yy/index.html`.


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
| Parton Shower | Herwig  | 7.3.0 | `herwigcollaboration/herwig-7.3:7.3.0` image |
| Generator     | CepGen  | 1.1.0(*)  | `${CEPGEN_DIR}` |
| Generator     | Sherpa  | master(*) | `${SHERPA_DIR}` |

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
* `create-graph-pdf.sh` and `hepmc2dot.py` (available within $PATH, from /usr/local/src/hepmc2dot) read an HepMC file an create a PDF with event visualization (1 event per page); the `hepmc2dot.py` just creates the `.dot` file that can be used as input to `create-graph-pdf.sh` instead of the hepmc file if more control is needed (e.g. skipping some number of events or limiting the number of max events visualized)

## Attaching to a previously-created container
You can see the list of containers you've created with:
```bash
docker ps -a
```

to start a container that has exited you can use
```bash
docker start container_id
```
where `container_id` is the container ID (you can get the ID of the latest container that exited with `docker -l -q`). Once started it will show in the list when you type `docker ps` and you can start a shell inside the container by doing 
```bash
docker attach container_id
```

## Building the container
If you need to modify the container, edit the `Dockerfile` and rebuild the container from the main folder containing this file:
```bash
docker build -t yy-gen-playground .
```

## Available pre-generated samples
A few samples have been already generated (either unshowered or showered) for easing comparisons.
HepMC files and yoda files from the Rivet routine can be found on this cernbox folder (each folder can be viewed and downloaded by anyone): [https://cernbox.cern.ch/s/ue21j3KK5eQq27i](https://cernbox.cern.ch/s/ue21j3KK5eQq27i)

## Docker installation
I prefer installing aplain docker engine instead of the full-suite of docker desktop, but this is up to you.

Make sure in any case to add your user to the `docker` group, e.g. in a linux system: 
```bash
sudo groupadd docker #in case this group does not exist yet
sudo usermod -aG docker $USER
```

### Troubleshootings
See also slides [here](https://docs.google.com/presentation/d/1SOlk3JS_UsUXbVb3GXviu_eCliy7r_afsvWin740KqI/edit?usp=sharing).

#### Permission issues - solution 1
Symptoms: files created within the container are visible outside the container but are not readable outside the container since they are from a different user (id).

Work-around:
* I’ve produced a new version of the container, called: `spagan/yy-gen-playground:0.6-root`
* This will give-up having a dedicated user inside the container and when you start the container you’ll be the root user.
* Every file created from inside the container will have root as user also outside the container. 
* When outside the container, manually change file permissions to match your user with:
`sudo chown -R yourusername:yourgroup folder`
replacing `yourusername` and `yourgroup` to the user name and group used in your system outside the container. 

#### Permission issues - solution 2
Then we need to make sure to map the user id in the container to your current user id correctly.
You can find your user id in a terminal by doing:
```bash
$ id -u
1000
$ id -g
1000
```
In this case (and most cases for single-user laptops) my user ID is `1000` and my group ID is `1000`.
You will then need to edit (with root privilegies) the files `/etc/subid` and `/etc/subgid` to be as they appear below, but replacing `spagan` with your username on your computer.
```bash
$ cat /etc/subuid
spagan:100000:65536
$ cat /etc/subgid
spagan:100000:65536
```
and then you need to enable user re-mapping in docker.
see also [here](https://docs.docker.com/engine/security/userns-remap/) for more information on this topic.

Other methods are possible especially given that the `Dockerfile` is available in the resporitory and are described in the docker documentation [here](https://faun.pub/set-current-host-user-for-docker-container-4e521cef9ffc).
