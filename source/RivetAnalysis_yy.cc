// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/DressedLeptons.hh"
#include "Rivet/Projections/MissingMomentum.hh"
#include "Rivet/Projections/DirectFinalState.hh"
#include "Rivet/Projections/VetoedFinalState.hh"

namespace Rivet {


  /// @brief Add a short analysis description here
  class RivetAnalysis_yy : public Analysis {
  public:

    /// Constructor
    RIVET_DEFAULT_ANALYSIS_CTOR(RivetAnalysis_yy);


    /// @name Analysis methods
    /// @{

    /// Book histograms and initialise projections before the run
    void init() {

      // Initialise and register projections

      // The basic final-state projection:
      // all final-state particles within
      // the given eta acceptance
      const FinalState fs((Cuts::abseta < 15.0) && (Cuts::pT > 100*MeV));
      declare(fs, "FS");

      // FinalState of direct photons and bare muons and electrons in the event
      DirectFinalState photons(Cuts::abspid == PID::PHOTON);
      DirectFinalState bare_leps(Cuts::abspid == PID::MUON || Cuts::abspid == PID::ELECTRON);

      // Dress the bare direct leptons with direct photons within dR < 0.1,
      // and apply some fiducial cuts on the dressed leptons
      Cut lepton_cuts = ((Cuts::abseta < 2.5) && (Cuts::pT > 5*GeV));
      DressedLeptons dressed_leps(photons, bare_leps, 0.1, lepton_cuts);
      declare(dressed_leps, "leptons");

      // Remove taus
      VetoedFinalState bare_leps_no_tau(bare_leps);
      bare_leps_no_tau.addDecayProductsVeto(PID::TAU);
      bare_leps_no_tau.addDecayProductsVeto(-PID::TAU);
      declare(bare_leps_no_tau, "leps_no_tau");

      // DressedLeptons dressed_leps(photons, bare_leps, 0.1, lepton_cuts);
      // declare(dressed_leps, "leptons");
      DressedLeptons dressed_leps_no_tau(photons, bare_leps_no_tau, 0.1, lepton_cuts);
      declare(dressed_leps_no_tau, "dressed_leptons_no_tau");

      //Charged particles
      const ChargedFinalState cfs(fs);
      declare(cfs, "CFS");

      //Veto charged particle tracks which belong to the leptons
      VetoedFinalState cfs_nolep(cfs); 
      cfs_nolep.addVetoPair(PID::ELECTRON, lepton_cuts); 
      cfs_nolep.addVetoPair(PID::MUON, lepton_cuts);
      declare(cfs_nolep, "CFS_NL");

      // The final-state particles declared above are clustered using FastJet with
      // the anti-kT algorithm and a jet-radius parameter 0.4
      // muons and neutrinos are excluded from the clustering
      FastJets jetfs(fs, FastJets::ANTIKT, 0.4, JetAlg::Muons::NONE, JetAlg::Invisibles::NONE);
      declare(jetfs, "jets");

      // Missing momentum
      declare(MissingMomentum(fs), "MET");

      // Book histograms
      // Histograms Leptons
      book(_histLeptonPt1[0], "LeptonPt1_incl", 100, 0, 100);
      book(_histLeptonPt2[0], "LeptonPt2_incl", 100, 0, 100);
      book(_histLeptonEta1[0], "LeptonEta1_incl", 54, -2.7, 2.7);
      book(_histLeptonEta2[0], "LeptonEta2_incl", 54, -2.7, 2.7);
      book(_histLeptonPt1[1], "LeptonPt1_Nch0", 100, 0, 100);
      book(_histLeptonPt2[1], "LeptonPt2_Nch0", 100, 0, 100);
      book(_histLeptonEta1[1], "LeptonEta1_Nch0", 54, -2.7, 2.7);
      book(_histLeptonEta2[1], "LeptonEta2_Nch0", 54, -2.7, 2.7);
      book(_histLeptonPt1[2], "LeptonPt1_Nch1", 100, 0, 100);
      book(_histLeptonPt2[2], "LeptonPt2_Nch1", 100, 0, 100);
      book(_histLeptonEta1[2], "LeptonEta1_Nch1", 54, -2.7, 2.7);
      book(_histLeptonEta2[2], "LeptonEta2_Nch1", 54, -2.7, 2.7);

      // Histograms di-lepton system
      book(_histMll[0], "mll_incl", 500, 0, 500);
      book(_histPtll[0], "ptll_incl", 500, 0, 500);
      book(_histDphill[0], "Dphill_incl", 64, 0.0, 3.2);
      book(_histAcoll[0], "LogAco_incl", 20, -10, 0.0);
      book(_histMll[1], "mll_Nch0", 500, 0, 500);
      book(_histPtll[1], "ptll_Nch0", 500, 0, 500);
      book(_histDphill[1], "Dphill_Nch0", 64, 0.0, 3.2);
      book(_histAcoll[1], "LogAco_Nch0", 20, -10, 0.0);
      book(_histMll[2], "mll_Nch1", 500, 0, 500);
      book(_histPtll[2], "ptll_Nch1", 500, 0, 500);
      book(_histDphill[2], "Dphill_Nch1", 64, 0.0, 3.2);
      book(_histAcoll[2], "LogAco_Nch1", 20, -10, 0.0);

      // specify custom binning
      book(_histMult   , "Mult", 100, -0.5, 99.5);

      book(_histEtaSumEt , "EtaSumEt", 25, 0, 5);
      book(_histPt , "Pt", 100, 0, 10.0);
      book(_histEta    , "Eta", 80, -20, 20);
      book(_histPhi    , "Phi", 50, 0, TWOPI);

/*    //examples
      book(_h["XXXX"], "myh1", 20, 0.0, 100.0);
      book(_h["YYYY"], "myh2", logspace(20, 1e-2, 1e3));
      book(_h["ZZZZ"], "myh3", {0.0, 1.0, 2.0, 4.0, 8.0, 16.0});
      // take binning from reference data using HEPData ID (digits in "d01-x01-y01" etc.)
      book(_h["AAAA"], 1, 1, 1);
      book(_p["BBBB"], 2, 1, 1);
      book(_c["CCCC"], 3, 1, 1);
*/
    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {

      // Retrieve dressed leptons, sorted by pT
      vector<DressedLepton> leptons = apply<DressedLeptons>(event, "leptons").dressedLeptons();

      // Charged + neutral final state
      const FinalState& fs = apply<FinalState>(event, "FS");
      // Charged FS particles only
      const FinalState& cfs_withlep = apply<FinalState>(event, "CFS");
      const FinalState& cfs_nolep = apply<FinalState>(event, "CFS_NL");

      Particles cfs_nolep_bypt = cfs_nolep.particles(cmpMomByPt); 

      const FourMomentum dilepton = leptons.size()>1 ? leptons[0].momentum() + leptons[1].momentum() : FourMomentum(0,0,0,0);
      
      const FourMomentum l1 = leptons.size()>0 ? leptons[0].momentum() : FourMomentum(0,0,0,0);
      const FourMomentum l2 = leptons.size()>1 ? leptons[1].momentum() : FourMomentum(0,0,0,0);

      const double Etal1 = leptons.size()>0 ? l1.eta() : -15;
      const double Etal2 = leptons.size()>1 ? l2.eta() : -15;

      const double mll = leptons.size()>1 ? dilepton.mass()/GeV : -1;
      const double pTll = leptons.size()>1 ? dilepton.pT()/GeV : -1;
      const double dphill = leptons.size()>1 ? fabs(deltaPhi(leptons[0], leptons[1])) : -1;
      const double aco = leptons.size()>1 ? fabs(1 - ((leptons[0].phi() - leptons[1].phi())/PI)): -1;

      const double Ptl1 = leptons.size()>0 ? l1.pT()/GeV : -1;
      const double Ptl2 = leptons.size()>1 ? l2.pT()/GeV : -1;

      // Selection 
      // veto events if no 2 leptons
      if (leptons.size()!=2) vetoEvent;

      // same or opposite flavor leptons
      //if (leptons[0].abspid() != leptons[1].abspid()) vetoEvent;
      //if (leptons[0].abspid() = leptons[1].abspid()) vetoEvent;

      // both are muons
      if (leptons[0].abspid() != PID::MUON and leptons[1].abspid() != PID::MUON) vetoEvent;
      
      // opposite charged leptons
      if (leptons[0].charge() == leptons[1].charge()) vetoEvent;

      // di-lepton invariant mass selection
      if (mll <= 10) vetoEvent;

      // Fill histograms
      _histLeptonPt1[0] -> fill(Ptl1);
      _histLeptonPt2[0] -> fill(Ptl2);
      _histLeptonEta1[0] -> fill(Etal1);
      _histLeptonEta2[0] -> fill(Etal2);

      _histMll[0] ->fill(mll);
      _histPtll[0] -> fill(pTll);
      _histDphill[0] -> fill(dphill);
      _histAcoll[0] -> fill(log(aco));

      _histMult->fill(cfs_nolep.size());
      for (const Particle& p : cfs_nolep.particles()) {
        _histEtaSumEt->fill(p.abseta(), p.Et());
        _histPt->fill(p.pT()/GeV);
        _histEta->fill(p.eta());
        _histPhi->fill(p.phi());
      }

      //events with zero charged particles
      if (cfs_nolep.size()==0){
	_histLeptonPt1[1] -> fill(Ptl1);
	_histLeptonPt2[1] -> fill(Ptl2);
	_histLeptonEta1[1] -> fill(Etal1);
	_histLeptonEta2[1] -> fill(Etal2);

	_histMll[1] ->fill(mll);
	_histPtll[1] -> fill(pTll);
	_histDphill[1] -> fill(dphill);
	_histAcoll[1] -> fill(log(aco));
      }

      //events with one or more charged particles
      if (cfs_nolep.size()>0){
	_histLeptonPt1[2] -> fill(Ptl1);
	_histLeptonPt2[2] -> fill(Ptl2);
	_histLeptonEta1[2] -> fill(Etal1);
	_histLeptonEta2[2] -> fill(Etal2);

	_histMll[2] ->fill(mll);
	_histPtll[2] -> fill(pTll);
	_histDphill[2] -> fill(dphill);
	_histAcoll[2] -> fill(log(aco));
      }
    }


    /// Normalise histograms etc., after the run
    void finalize() {

      double normfac = crossSection()/sumOfWeights();

      scale(_histLeptonPt1[0], normfac);
      scale(_histLeptonPt2[0], normfac);
      scale(_histLeptonEta1[0], normfac);
      scale(_histLeptonEta2[0], normfac);

      scale(_histMll[0], normfac);
      scale(_histPtll[0], normfac);
      scale(_histDphill[0], normfac);
      scale(_histAcoll[0], normfac);

      scale(_histLeptonPt1[1], normfac);
      scale(_histLeptonPt2[1], normfac);
      scale(_histLeptonEta1[1], normfac);
      scale(_histLeptonEta2[1], normfac);

      scale(_histMll[1], normfac);
      scale(_histPtll[1], normfac);
      scale(_histDphill[1], normfac);
      scale(_histAcoll[1], normfac);

      scale(_histLeptonPt1[2], normfac);
      scale(_histLeptonPt2[2], normfac);
      scale(_histLeptonEta1[2], normfac);
      scale(_histLeptonEta2[2], normfac);

      scale(_histMll[2], normfac);
      scale(_histPtll[2], normfac);
      scale(_histDphill[2], normfac);
      scale(_histAcoll[2], normfac);

      normalize(_histMult, normfac); 

      normalize(_histPt); 
      normalize(_histEta);
      normalize(_histPhi);

    }

    /// @}


    /// @name Histograms
    /// @{

    Histo1DPtr _histMll[3], _histPtll[3], _histDphill[3], _histAcoll[3];
    Histo1DPtr _histLeptonPt1[3], _histLeptonPt2[3], _histLeptonEta1[3], _histLeptonEta2[3];

    Histo1DPtr _histMult, _histEta, _histRapidity, _histPt, _histE, _histPhi;
    Histo1DPtr _histEtaCh, _histRapidityCh, _histPtCh, _histECh, _histPhiCh;
    Profile1DPtr _histEtaSumEt;
    Scatter2DPtr _histEtaPMRatio, _histEtaChPMRatio, _histRapidityPMRatio, _histRapidityChPMRatio;

    Histo1DPtr _tmphistEtaPlus, _tmphistEtaMinus, _tmphistEtaChPlus, _tmphistEtaChMinus;
    Histo1DPtr _tmphistRapPlus, _tmphistRapMinus, _tmphistRapChPlus, _tmphistRapChMinus;

    map<string, Histo1DPtr> _h;
    map<string, Profile1DPtr> _p;
    map<string, CounterPtr> _c;

    /// @}

  };


  RIVET_DECLARE_PLUGIN(RivetAnalysis_yy);

}

