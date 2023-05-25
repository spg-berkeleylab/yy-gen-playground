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
    int evt_counter; 
    int mult_ch_counter; 
    int mult_ch_counter_cut;
    bool first_finalized;

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

      // FinalState of direct photons and bare muons and electrons in the event
      DirectFinalState photons(Cuts::abspid == PID::PHOTON);  
      DirectFinalState bare_leps(Cuts::abspid == PID::MUON || Cuts::abspid == PID::ELECTRON);

      // Dress the bare direct leptons with direct photons within dR < 0.1,
      // and apply some fiducial cuts on the dressed leptons
      Cut lepton_cuts = Cuts::abseta < 2.5 && Cuts::pT > 20*GeV; 

      evt_counter=0;
      mult_ch_counter=0;
      mult_ch_counter_cut=0;
      first_finalized=false;

      const FinalState fs(Cuts::abseta < 2.5 && Cuts::pT > 500*MeV);
      declare(fs, "FS");

      const ChargedFinalState cfs(fs);
      declare(cfs, "CFS");

      VetoedFinalState cfs_nolep(cfs); 
      cfs_nolep.addVetoPair(PID::ELECTRON, lepton_cuts); 
      cfs_nolep.addVetoPair(PID::MUON, lepton_cuts);

      declare(cfs_nolep, "CFS_NL");

      VetoedFinalState bare_leps_no_tau(bare_leps);
      bare_leps_no_tau.addDecayProductsVeto(PID::TAU); 
      bare_leps_no_tau.addDecayProductsVeto(-PID::TAU);

      declare(bare_leps_no_tau, "leps_no_tau");

      // DressedLeptons dressed_leps(photons, bare_leps, 0.1, lepton_cuts);
      // declare(dressed_leps, "leptons");
      DressedLeptons dressed_leps_no_tau(photons, bare_leps_no_tau, 0.1, lepton_cuts);
      declare(dressed_leps_no_tau, "dressed_leptons_no_tau");

      // The final-state particles declared above are clustered using FastJet with
      // the anti-kT algorithm and a jet-radius parameter 0.4
      // muons and neutrinos are excluded from the clustering
      FastJets jetfs(fs, FastJets::ANTIKT, 0.4, JetAlg::Muons::NONE, JetAlg::Invisibles::NONE);
      declare(jetfs, "jets");

      // Missing momentum
      declare(MissingMomentum(fs), "MET");

      // Book histograms
      // specify custom binning

      //BOOK HISTOGRAMS, BEFORE ("Pre") AND AFTER ("Post") APPLYING SELECTIONS

      //HISTOGRAMS FOR DILEPTON
      book(_histMllPre, "Mll_Pre", 300, -1.5, 900);
      book(_histPtllPre, "Ptll_Pre", 300, -1.5, 160);
      book(_histDphillPre, "Dphill_Pre", 50, -1.5, 3.5);
      book(_histAcollPre, "Acoll_Pre", 50, -1.5,1);

      book(_histMllPost, "Mll_Post", 300, -1.5, 900);
      book(_histPtllPost, "Ptll_Post", 300, -1.5, 160);
      book(_histDphillPost, "Dphill_Post", 50, -1.5, 3.5);
      book(_histAcollPost, "Acoll_Post", 50, -1.5,1);

      //HISTOGRAMS FOR LEADING LEPTONS (INDIVIDUAL)
      book(_histPtL1Pre, "Pt_l1_Pre", 300, -1.5, 550);
      book(_histPtL2Pre, "Pt_l2_Pre", 300, -1.5, 550);
      book(_histEtaL1Pre, "Eta_l1_Pre", 300, -15.5, 3);
      book(_histEtaL2Pre, "Eta_l2_Pre", 300, -15.5, 3);

      book(_histPtL1Post, "Pt_l1_Post", 300, -1.5, 550);
      book(_histPtL2Post, "Pt_l2_Post", 300, -1.5, 550);
      book(_histEtaL1Post, "Eta_l1_Post", 300, -15.5, 3);
      book(_histEtaL2Post, "Eta_l2_Post", 300, -15.5, 3);

      //HISTOGRAMS FOR LEADING TRACK (NO LEPTONS CONSIDERED) 
      book(_histPtT1Pre, "Pt_t1_Pre", 300, -1.5, 550);
      book(_histEtaT1Pre, "Eta_t1_Pre", 300, -15.5, 3);

      book(_histPtT1Post, "Pt_t1_Post", 300, -1.5, 550);
      book(_histEtaT1Post, "Eta_t1_Post", 300, -15.5, 3);

      //HISTOGRAMS FOR ALL TRACKS (NO LEPTONS CONSIDERED)
      book(_histMultChPre , "MultCh_Pre", 100, -0.5, 9.5);
      book(_histPtChPre , "PtCh_Pre", 300, 0, 550); 
      book(_histEtaChPre , "EtaCh_Pre", 300, -3, 3);


      book(_histMultChPost , "MultCh_Post", 100, -0.5, 9.5);  //MultCh=0 after selection
      book(_histPtChPost , "PtCh_Post", 300, 0, 550); 
      book(_histEtaChPost , "EtaCh_Post", 300, -3, 3);

      //HISTOGRAMS FOR PARTICLES BELONGING TO A CH=4+ EVENT
      book(_histMultChPlus_PID, "MultChPlus_PID", 500, -250,250);
      book(_histMultChPlus_Pt, "MultChPlus_Pt", 100, 0, 550);
      book(_histMultChPlus_Eta, "MultChPlus_Eta", 100, -15.5,3);


      book(_histCutFlow, "CutFlow",10, 0.5, 10.5); //bin centered at integer, bin width = 1

    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {

      //Deleted all comments related to jets and missing momentum
      evt_counter++;
      MSG_DEBUG("NEW EVENT: " << evt_counter);

      // Retrieve dressed leptons, sorted by pT

      //vector<DressedLepton> leptons = apply<DressedLeptons>(event, "leptons").dressedLeptons(cmpMomByPt);
      vector<DressedLepton> leptons = apply<DressedLeptons>(event, "dressed_leptons_no_tau").dressedLeptons(cmpMomByPt);

      // Charged + neutral final state
      const FinalState& fs = apply<FinalState>(event, "FS");
      MSG_DEBUG("Total number of final state particles = " << fs.size());

      // Same for the charged FS particles only
      const FinalState& cfs_withlep = apply<FinalState>(event, "CFS");
      const FinalState& cfs_nolep = apply<FinalState>(event, "CFS_NL");

      //Debugging: change order of vetoing electrons and then muons later; try to print for loop 
      //with PID of charged particles with and without leptons removed 
      MSG_DEBUG("Number of all charged particles (without leptons removed) =" << cfs_withlep.size()); 
      MSG_DEBUG("Number of charged particles with leptons removed =" << cfs_nolep.size());

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
      MSG_DEBUG("pT for l1 =" << Ptl1);
      MSG_DEBUG("pT for l2: " <<  Ptl2);

      const double Pt_t1 = cfs_nolep.size()>0 ? cfs_nolep_bypt[0].pT()/GeV : -1;
      const double Eta_t1 = cfs_nolep.size()>0 ? cfs_nolep_bypt[0].eta() : -15;

      //if there are 2+ leptons, and first two leading leptons are both electrons or taus
      if  (leptons.size()>1){
        if (leptons[0].abspid() == leptons[1].abspid()){
          if ((leptons[0].abspid()==PID::ELECTRON) || (leptons[0].abspid()==PID::TAU)) {
            MSG_DEBUG("Vetoing e-e or tau-tau event");
            vetoEvent;
          }
        }
      }
      //if only leading lepton is an electron or tau
      if  (leptons.size()==1){
        if ((leptons[0].abspid()==PID::ELECTRON) || (leptons[0].abspid()==PID::TAU)){
          MSG_DEBUG("Vetoing event with one lepton that is e or tau");
          vetoEvent;
        }
      }
      _histMllPre->fill(mll);
      _histPtllPre -> fill(pTll);
      _histDphillPre -> fill(dphill);
      _histAcollPre -> fill(aco);

      _histPtL1Pre -> fill(Ptl1);
      _histPtL2Pre -> fill(Ptl2);
      _histEtaL1Pre -> fill(Etal1);
      _histEtaL2Pre -> fill(Etal2);

      _histPtT1Pre -> fill(Pt_t1);
      _histEtaT1Pre -> fill(Eta_t1);

      _histMultChPre->fill(cfs_nolep.size());

      if (cfs_nolep.size()>3){
        MSG_DEBUG("WARNING: NUMBER OF CHARGED PARTICLES: " << cfs_nolep.size());
        MSG_DEBUG("EVENT NUMBER: " << evt_counter);
        mult_ch_counter++;
        MSG_DEBUG("Counter for events with 4+ charged particles: " << mult_ch_counter);
        int ch_counter = 1; 
        MSG_DEBUG("Now listing particle properties:");
        for (const Particle& p : cfs_nolep.particles()) {
          MSG_DEBUG("For particle " << ch_counter);

          MSG_DEBUG("PID: " << p.pid());
          _histMultChPlus_PID -> fill(p.pid());

          MSG_DEBUG("pT: " << p.pT()/GeV);
          _histMultChPlus_Pt -> fill(p.pT()/GeV);

          MSG_DEBUG("eta: " << p.eta());
          _histMultChPlus_Eta -> fill(p.eta());

          ch_counter++;
      }


      }
      for (const Particle& p : cfs_nolep.particles()) {
        _histEtaChPre->fill(p.eta());
        _histPtChPre->fill(p.pT()/GeV);
      }

      //SELECTIONS

      _histCutFlow->fill(1); 

      //1) Dilepton event
      if (leptons.size()!=2){
        vetoEvent;
      }
      _histCutFlow->fill(2);

      //2) Leptons have same flavor
      if (leptons[0].abspid() != leptons[1].abspid()){
        vetoEvent;
      }
      _histCutFlow->fill(3);

      //3) Leptons have opposite charge
      if (leptons[0].charge() == leptons[1].charge()){
        vetoEvent;
      }
      _histCutFlow->fill(4);

      //4) Leading lepton (l1) has pT > 27 GeV
      if (l1.pT()/GeV <= 27){
        vetoEvent;
      }

      _histCutFlow->fill(5);
    
      //5) Second leading lepton (l2) has pT > 20 GeV
      if (l2.pT()/GeV <= 20) {
        vetoEvent;
      }
      _histCutFlow->fill(6);

      if (cfs_nolep.size()>3){
        mult_ch_counter_cut++;
      }
      //ADD NEW MULT_CH COUNTER HERE

      //CHANGE
      //6) Invariant mass of dilepton > 20 GeV
      if (mll <= 110){ 
        vetoEvent;
      }
      _histCutFlow->fill(7);

      //DELETE or >-1GeV (want 0 GeV to pass)
      //7) pT of dilepton > 30 GeV
      if (pTll <= -1){
        vetoEvent;
      }
      _histCutFlow->fill(8);

      //8) N_track = 0 (not including dilepton)
      if (cfs_nolep.size()>0){
        vetoEvent;
      }
      _histCutFlow->fill(9);

      _histMllPost->fill(mll);
      _histPtllPost -> fill(pTll);
      _histDphillPost -> fill(dphill);
      _histAcollPost -> fill(aco);

      _histPtL1Post -> fill(Ptl1);
      _histPtL2Post -> fill(Ptl2);
      _histEtaL1Post -> fill(Etal1);
      _histEtaL2Post -> fill(Etal2);

      _histMultChPost->fill(cfs_nolep.size());
      for (const Particle& p : cfs_nolep.particles()) {
        _histEtaChPost->fill(p.eta());
        _histPtChPost->fill(p.pT()/GeV);
      }

    }


    /// Normalise histograms etc., after the run
    void finalize() {
      if (first_finalized == true) {
        MSG_INFO("PAY ATTENTION: finalize() BEING CALLED MORE THAN ONCE! FIX ME!!!!!!!!!!!!!!");
        return;
      }
      first_finalized = true;

      MSG_INFO("EVENT COUNTER: " << evt_counter);
      MSG_INFO("SumOfWeights: " << sumOfWeights());
      MSG_DEBUG("Number of events with 4+ charged particles: " << mult_ch_counter);
      MSG_DEBUG("Number of events with 4+ charged particles after cut 6: " << mult_ch_counter_cut);
      double normfac = crossSection()/sumOfWeights();

      scale(_histMllPre, normfac);
      scale(_histPtllPre, normfac);
      scale(_histDphillPre, normfac);
      scale(_histAcollPre, normfac);

      scale(_histMllPost, normfac);
      scale(_histPtllPost, normfac);
      scale(_histDphillPost, normfac);
      scale(_histAcollPost, normfac);

      scale(_histPtL1Pre, normfac);
      scale(_histPtL2Pre, normfac);
      scale(_histEtaL1Pre, normfac);
      scale(_histEtaL2Pre, normfac);

      scale(_histPtL1Post, normfac);
      scale(_histPtL2Post, normfac);
      scale(_histEtaL1Post, normfac);
      scale(_histEtaL2Post, normfac);

      scale(_histPtT1Pre, normfac);
      scale(_histEtaT1Pre, normfac);

      scale(_histPtT1Post, normfac);
      scale(_histEtaT1Post, normfac);

      scale(_histMultChPre, normfac);
      scale(_histPtChPre, normfac);
      scale(_histEtaChPre, normfac);

      scale(_histMultChPost, normfac);
      scale(_histPtChPost, normfac);
      scale(_histEtaChPost, normfac);


      scale(_histMultChPlus_PID, normfac);
      scale(_histMultChPlus_Pt, normfac);
      scale(_histMultChPlus_Eta, normfac);

      scale(_histCutFlow, normfac);

    }

    /// @}


    /// @name Histograms
    /// @{

    Histo1DPtr _histMllPre, _histPtllPre, _histDphillPre, _histAcollPre;
    Histo1DPtr _histMllPost, _histPtllPost, _histDphillPost, _histAcollPost;

    Histo1DPtr _histPtL1Pre, _histPtL2Pre, _histEtaL1Pre, _histEtaL2Pre;
    Histo1DPtr _histPtL1Post, _histPtL2Post, _histEtaL1Post, _histEtaL2Post;

    Histo1DPtr _histPtT1Pre, _histEtaT1Pre;
    Histo1DPtr _histPtT1Post, _histEtaT1Post;

    Histo1DPtr _histMultChPre, _histPtChPre, _histEtaChPre;
    Histo1DPtr _histMultChPost, _histPtChPost, _histEtaChPost;

    Histo1DPtr _histMultChPlus_PID, _histMultChPlus_Pt, _histMultChPlus_Eta;

    Histo1DPtr _histCutFlow;

    map<string, Histo1DPtr> _h;
    map<string, Profile1DPtr> _p;
    map<string, CounterPtr> _c;

    /// @}


  };


  RIVET_DECLARE_PLUGIN(RivetAnalysis_yy);

}
