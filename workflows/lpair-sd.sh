# Run Rivet Analysis on LPAIR SD
echo "Running Rivet on LPAIR SD 60 <= mll < 200 GeV"
rivet -a RivetAnalysis_yy -o LPair-SD-60_200.yoda  /work/photonfusion2023/LPAIR/363699_HepMCAsciiEG_Lpair_BSY_SDiss_ggTOmumu_60M200_LeptonFilter.hepmc.gz >& rivet-lpair-1.log
echo "Running Rivet on LPAIR SD mll >= 200 GeV"
rivet -a RivetAnalysis_yy -o LPair-SD-200.yoda  /work/photonfusion2023/LPAIR/363700_HepMCAsciiEG_Lpair_BSY_SDiss_ggTOmumu_200M_LeptonFilter.hepmc.gz >& rivet-lpair-1.log
echo "Merging"
rivet-merge -o LPair-SD-merged.yoda LPair-SD-60_200.yoda:=0.582 LPair-SD-200.yoda:=0.02628
