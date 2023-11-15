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
  class RivetAnalysis_yy_WW : public Analysis {
  public:

    /// Constructor
    RIVET_DEFAULT_ANALYSIS_CTOR(RivetAnalysis_yy_WW);


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
      Cut lepton_cuts = ((Cuts::abseta < 2.5) && (Cuts::pT > 20*GeV));
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
      book(_histLeptonPt1, "LeptonPt1", 100, 0, 100);
      book(_histLeptonPt2, "LeptonPt2", 100, 0, 100);
      book(_histLeptonEta1, "LeptonEta1", 54, -2.7, 2.7);
      book(_histLeptonEta2, "LeptonEta2", 54, -2.7, 2.7);

      // Histograms di-lepton system
      book(_histMll, "mll", 100, 0, 500);
      book(_histPtll, "ptll", 100, 0, 500);
      book(_histDphill, "Dphill", 64, 0.0, 3.2);
      book(_histAcoll, "LogAco", 20, -10, 0.0);

      // specify custom binning
      book(_histMult   , "Mult", 100, -0.5, 99.5);

      book(_histEtaSumEt , "EtaSumEt", 25, 0, 5);
      book(_histPt , "Pt", 100, 0, 10.0);
      book(_histEta    , "Eta", 80, -20, 20);
      book(_histPhi    , "Phi", 50, 0, TWOPI);

      book(_histPtLowEta , "PtLowEta", 100, 0, 10.0);
      book(_histMultLowEta500 , "MultLowEta500", 100, -0.5, 99.5);

      book(_histMultLowEta100 , "MultLowEta100", 100, -0.5, 99.5);
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

      // opposite charged leptons
      if (leptons[0].charge() == leptons[1].charge()) vetoEvent;

      // di-lepton invariant mass selection
      if (mll <= 20) vetoEvent;

      // di-lepton pT selection
      if (pTll <= 30) vetoEvent;

      // leading lepton pT selection
      if (Ptl1 <= 27) vetoEvent;

      // Fill histograms
      _histLeptonPt1 -> fill(Ptl1);
      _histLeptonPt2 -> fill(Ptl2);
      _histLeptonEta1 -> fill(Etal1);
      _histLeptonEta2 -> fill(Etal2);

      _histMll -> fill(mll);
      _histPtll -> fill(pTll);
      _histDphill -> fill(dphill);
      _histAcoll -> fill(log(aco));

      _histMult -> fill(cfs_nolep.size());

      int lowEtaCt500 = 0;
      int lowEtaCt100 = 0;
      for (const Particle& p : cfs_nolep.particles()) {
        _histEtaSumEt -> fill(p.abseta(), p.Et());
        _histPt -> fill(p.pT()/GeV);
        _histEta -> fill(p.eta());
        _histPhi -> fill(p.phi());

        if (p.abseta() <= 2.5) {
          _histPtLowEta->fill(p.pT()/GeV);

          if (p.pT()/GeV >= 0.5) lowEtaCt500++;
          if (p.pT()/GeV >= 0.1) lowEtaCt100++;
        }
      }
      _histMultLowEta500 -> fill(lowEtaCt500);
      _histMultLowEta100 -> fill(lowEtaCt100);

    }


    /// Normalise histograms etc., after the run
    void finalize() {

      double normfac = crossSection()/sumOfWeights();

      scale(_histLeptonPt1, normfac);
      scale(_histLeptonPt2, normfac);
      scale(_histLeptonEta1, normfac);
      scale(_histLeptonEta2, normfac);

      scale(_histMll, normfac);
      scale(_histPtll, normfac);
      scale(_histDphill, normfac);
      scale(_histAcoll, normfac);

      normalize(_histMult, normfac);

      normalize(_histPt);
      normalize(_histEta);
      normalize(_histPhi);

      normalize(_histMultLowEta500, normfac);
      normalize(_histMultLowEta100, normfac);
      normalize(_histPtLowEta);
    }

    /// @}


    /// @name Histograms
    /// @{

    Histo1DPtr _histMll, _histPtll, _histDphill, _histAcoll;
    Histo1DPtr _histLeptonPt1, _histLeptonPt2, _histLeptonEta1, _histLeptonEta2;

    Histo1DPtr _histMult, _histEta, _histRapidity, _histPt, _histE, _histPhi, _histMultLowEta500, _histMultLowEta100, _histPtLowEta;
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


  RIVET_DECLARE_PLUGIN(RivetAnalysis_yy_WW);

}

