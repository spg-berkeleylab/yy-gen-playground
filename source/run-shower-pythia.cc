/** Run showering of LHE files with Pythia8.
*
* Simple interface with Pythia8 that reads an LHE file, reads and execute a dedicated configuration file
* and showers the events. The output is stored in HepMC3 format.
* Code based on:
* - pythia examples/main11.cc (process LHE files)
* - pythia examples/main41.cc (output HepMC files)
* - https://github.com/CMS-SUS-XPAG/GenLHEfiles/blob/master/DecayStep/PythiaScripts/main20.cc
*/

#include "Pythia8/Pythia.h"
#include "Pythia8Plugins/HepMC2.h"

#include <iostream>
#include <string>

using namespace Pythia8;

int main(int argc, char *argv[]) {
    // Get inputs
    if ((argc != 4) and (argc != 5)) {
        std::cout << "Usage: " << argv[0] << "inputLHE config outputHepMC [nevents]" << std::endl;
        return 1;
    }
    std::string infilename(argv[1]);
    std::string cfg(argv[2]);
    std::string outfilename(argv[3]);
    int nEvents=-1;
    if (argc == 5) nEvents = atoi(argv[4]);

    std::cout << "Input file: " << infilename << std::endl;
    std::cout << "Input config: " << cfg << std::endl;
    std::cout << "Output file: " << outfilename << std::endl;
    std::cout << "Max number of events: " << nEvents << std::endl;

    // Interface for conversion from Pythia8::Event to HepMC event.
    HepMC::Pythia8ToHepMC ToHepMC;
    // Specify file where HepMC events will be stored.
    HepMC::IO_GenEvent ascii_io(outfilename.c_str(), std::ios::out);

    // Generator.
    Pythia pythia;

    // Set input file
    pythia.readString("Beams:frameType = 4");
    pythia.readString(std::string("Beams:LHEF = ")+infilename);

    // Read config file and initialize Pythia
    pythia.readFile( cfg, 0);
    pythia.init();

    // Run Pythia
    int nAbort = 10; // Allow for possibility of a few faulty events.
    int iAbort = 0;

    for (int iEvent = 0; ; ++iEvent) {
        if ( (nEvents >= 0) and (iEvent >= nEvents) ) break;

        // run generator
        if (!pythia.next()) {
            if (pythia.info.atEndOfFile()) break;
            if (++iAbort < nAbort) continue;
            break;
        }

        // Store output.
        // Construct new empty HepMC event and fill it.
        // Units will be as chosen for HepMC build; but can be changed
        // by arguments, e.g. GenEvt( HepMC::Units::GEV, HepMC::Units::MM)
        HepMC::GenEvent* hepmcevt = new HepMC::GenEvent();
        ToHepMC.fill_next_event( pythia, hepmcevt );

        // Write the HepMC event to file. Done with it.
        ascii_io << hepmcevt;
        delete hepmcevt;

    }

    // Print stats.
    pythia.stat();

    // All Done
    return 0;
}