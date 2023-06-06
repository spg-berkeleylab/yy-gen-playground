#include "HepMC3/GenEvent.h"
#include "HepMC3/WriterAscii.h"
#include "HepMC3/ReaderAsciiHepMC2.h"

#include <iostream>

int main (int argc, char **argv)
{
  if (argc != 3) {
    std::cout << "Usage: " << argv[0] << " input-hepmc2.hepmc output-hepmc3.hepmc" << std::endl;
    return 1;
  }
  HepMC3::ReaderAsciiHepMC2 inputA (argv[1]);
  if(inputA.failed()) return 1;
  HepMC3::WriterAscii outputA (argv[2]) ;
  if( outputA.failed() ) return 2;
  while ( ! inputA.failed () )
    {
      HepMC3::GenEvent evt ( HepMC3::Units::GEV , HepMC3::Units::MM ) ;
      inputA.read_event ( evt ) ;
      if( inputA.failed () ) { printf("End of file reached . Exit.\n") ; break ;}
      outputA.write_event ( evt );
      evt.clear () ;
    }
  inputA.close () ;
  outputA.close () ;
  return 0;
}
