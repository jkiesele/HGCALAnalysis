// -*- C++ -*-
//
// Package:    HGCAL_Analysis/TruthAna
// Class:      TruthAna
//
/**\class TruthAna TruthAna.cc HGCAL_Analysis/TruthAna/plugins/TruthAna.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Jan Kieseler
//         Created:  Wed, 02 Jun 2021 16:22:31 GMT
//
//

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "SimDataFormats/CaloAnalysis/interface/SimCluster.h"
#include "SimDataFormats/CaloAnalysis/interface/CaloParticle.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TTree.h"
//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<>
// This will improve performance in multithreaded jobs.

using reco::TrackCollection;

class TruthAna : public edm::one::EDAnalyzer<edm::one::SharedResources> {
public:
  explicit TruthAna(const edm::ParameterSet&);
  ~TruthAna();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  void beginJob() override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void endJob() override;


  // ----------member data ---------------------------
  edm::EDGetTokenT<std::vector<SimCluster>> simClustersToken_;  //used to select what tracks to read from configuration file
  edm::EDGetTokenT<std::vector<CaloParticle>> caloParticlesToken_;

  TTree* outtree_;
  edm::Service<TFileService> fs_;

  void fillSCproperties(const SimCluster&sc, int asso){
      auto st = sc.g4Tracks().at(0);
      auto mom = st.getMomentumAtBoundary();
      auto pos = st.getPositionAtBoundary();

      sc_energy_.push_back(sc.p4().E());
      sc_px_.push_back(sc.p4().px());
      sc_py_.push_back(sc.p4().py());
      sc_pz_.push_back(sc.p4().pz());

      st_energy_.push_back(mom.E());
      st_px_.push_back(mom.px());
      st_py_.push_back(mom.py());
      st_pz_.push_back(mom.pz());
      st_x_.push_back(pos.x());
      st_y_.push_back(pos.y());
      st_z_.push_back(pos.z());

      sc_cp_ass_.push_back(asso);
  }

  std::vector<float> sc_energy_;
  std::vector<float> sc_px_;
  std::vector<float> sc_py_;
  std::vector<float> sc_pz_;
  std::vector<float> st_energy_;
  std::vector<float> st_px_;
  std::vector<float> st_py_;
  std::vector<float> st_pz_;
  std::vector<float> st_x_;
  std::vector<float> st_y_;
  std::vector<float> st_z_;
  std::vector<int>   sc_cp_ass_;

  std::vector<float> cp_energy_;
  int n_sc_;
  int n_sc_ass_;

  #ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
  edm::ESGetToken<SetupData, SetupRecord> setupToken_;
#endif
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
TruthAna::TruthAna(const edm::ParameterSet& iConfig)
    : simClustersToken_(consumes<std::vector<SimCluster>>(iConfig.getParameter<edm::InputTag>("SimClusters"))),
      caloParticlesToken_(consumes<std::vector<CaloParticle>>(iConfig.getParameter<edm::InputTag>("CaloParticles"))),
      outtree_(0){

}

TruthAna::~TruthAna() {
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  //
  // please remove this method altogether if it would be left empty
}

//
// member functions
//



// ------------ method called for each event  ------------
void TruthAna::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  using namespace edm;

  //clear per event

  sc_energy_.clear();
  sc_px_.clear();
  sc_py_.clear();
  sc_pz_.clear();
  st_energy_.clear();
  st_px_.clear();
  st_py_.clear();
  st_pz_.clear();
  st_x_.clear();
  st_y_.clear();
  st_z_.clear();
  sc_cp_ass_.clear();

  cp_energy_.clear();
  n_sc_=0;
  n_sc_ass_=0;


  /// first fill associated ones

  std::vector<const SimCluster* > associated;
  int i=0;
  for (const auto& cp : iEvent.get(caloParticlesToken_)) {
      for(const auto& sc: cp.simClusters()){
          fillSCproperties(*sc, i);
          associated.push_back(&*sc);
          n_sc_ass_++;
      }
      cp_energy_.push_back(cp.energy());
      i++;
  }
  i=-1;
  for (const auto& sc : iEvent.get(simClustersToken_)) {
      n_sc_++;
      if(std::find(associated.begin(),associated.end(),&sc) != associated.end())
          continue ; //no double counting
      fillSCproperties(sc, i);
  }

  //check initial caloparticle energy / energy of all scs attached to it etc

  outtree_->Fill();
}

// ------------ method called once each job just before starting event loop  ------------
void TruthAna::beginJob() {
    if (!fs_)
        throw edm::Exception(edm::errors::Configuration,
                "TFile Service is not registered in cfg file");

    outtree_ = fs_->make<TTree>("tree", "tree");



    outtree_->Branch("sc_energy",&sc_energy_)  ;
    outtree_->Branch("sc_px",&sc_px_    )  ;
    outtree_->Branch("sc_py",&sc_py_    )  ;
    outtree_->Branch("sc_pz",&sc_pz_    )  ;
    outtree_->Branch("st_energy",&st_energy_)  ;
    outtree_->Branch("st_px",&st_px_    )  ;
    outtree_->Branch("st_py",&st_py_    )  ;
    outtree_->Branch("st_pz",&st_pz_    )  ;
    outtree_->Branch("st_x",&st_x_     )  ;
    outtree_->Branch("st_y",&st_y_     )  ;
    outtree_->Branch("st_z",&st_z_     )  ;
    outtree_->Branch("sc_cp_ass",&sc_cp_ass_)  ;
    outtree_->Branch("cp_energy",&cp_energy_)  ;
    outtree_->Branch("n_sc",&n_sc_     )  ;
    outtree_->Branch("n_sc_ass",&n_sc_ass_ )  ;

}

// ------------ method called once each job just after ending the event loop  ------------
void TruthAna::endJob() {
  // please remove this method if not needed
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void TruthAna::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);

  //Specify that only 'tracks' is allowed
  //To use, remove the default given above and uncomment below
  //ParameterSetDescription desc;
  //desc.addUntracked<edm::InputTag>("tracks","ctfWithMaterialTracks");
  //descriptions.addWithDefaultLabel(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(TruthAna);
