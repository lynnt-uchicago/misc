#include "SampleAnalyzer/User/Analyzer/sm_test_analysis.h"
#include <math.h>  
using namespace MA5;
using namespace std;

// -----------------------------------------------------------------------------
// Initialize
// function called one time at the beginning of the analysis
// -----------------------------------------------------------------------------
bool sm_test_analysis::Initialize(const MA5::Configuration& cfg, const std::map<std::string,std::string>& parameters)
{
  cout << "BEGIN Initialization" << endl;
  // initialize variables, histos
  TH1F* plot_mass_check;
  TH1F* plot_invar_mass;
  plot_mass_check = new TH1F("plot_mass_check", "mass_check", 200, 0., 200.);
  plot_invar_mass = new TH1F("plot_invar_mass", "invar_mass", 200, 0., 200.);

  cout << "END Initialization" << endl;
  return true;
}

// -----------------------------------------------------------------------------
// Finalize
// function called one time at the end of the analysis
// -----------------------------------------------------------------------------
void sm_test_analysis::Finalize(const SampleFormat& summary, const std::vector<SampleFormat>& files)
{
  cout << "BEGIN Finalization" << endl;
  // saving histos
  TFile* output = new TFile("histos.root", "RECREATE");
  plot_mass_check -> Write();
  plot_invar_mass -> Write();
  output -> Close();  
  cout << "END Finalization" << endl;
}

// -----------------------------------------------------------------------------
// Execute
// function called each time one event is read
// -----------------------------------------------------------------------------
bool sm_test_analysis::Execute(SampleFormat& sample, const EventFormat& event)
{
  // ***************************************************************************
  // Example of analysis with generated particles
  // Concerned samples : LHE/STDHEP/HEPMC
  // ***************************************************************************
  
  if (event.mc()!=0)
  {
    //    cout << "---------------NEW EVENT-------------------" << endl;

    // Event weight
    double myWeight=1.;
    if (!Configuration().IsNoEventWeight()) myWeight=event.mc()->weight();
    double mass_tot = 0;
    double e = 0;
    double p2 = 0;
    double px = 0;
    double py = 0;
    double pz = 0;
    // Initial state
    for (MAuint32 i=0;i<event.mc()->particles().size();i++){
      const MCParticleFormat& part = event.mc()->particles()[i];
      if (part.pdgid() == 25){ // if particle is higgs
	plot_mass_check ->Fill(part.m());
	e += part.e();
	px += part.px();
	py += part.py();
	pz += part.pz();
      }
    }
    p2 = px*px + py*py + pz*pz;
    mass_tot = sqrt(e*e - p2);
    plot_invar_mass->Fill(mass_tot);
    
    /*
	cout << "----------------------------------" << endl;
      cout << "MC particle" << endl;
      cout << "----------------------------------" << endl;

      // display index particle
      cout << "index=" << i+1;

      // display the status code
      cout << "Status Code=" << part.statuscode() << endl;
      if (PHYSICS->Id->IsInitialState(part)) cout << " (Initial state) ";
      else if (PHYSICS->Id->IsFinalState(part)) cout << " (Final state) ";
      else cout << " (Intermediate state) ";
      cout << endl;

      // pdgid
      cout << "pdg id=" << part.pdgid() << endl;
      if (PHYSICS->Id->IsInvisible(part)) cout << " (invisible particle) ";
      else cout << " (visible particle) ";
      cout << endl;

      // display kinematics information
      cout << "px=" << part.px()
                << " py=" << part.py()
                << " pz=" << part.pz()
                << " e="  << part.e()
                << " m="  << part.m() << endl;
      cout << "pt=" << part.pt() 
                << " eta=" << part.eta() 
                << " phi=" << part.phi() << endl;

      // display particle mother id
      if (part.mothers().empty()) 
      {
        cout << "particle with no mother." << endl;
      }
      else
      {
        std::cout << "particle coming from the decay of";
        for(MAuint32 j=0;j<part.mothers().size();j++)
        {
          const MCParticleFormat* mother = part.mothers()[j];
          cout << " " << mother->pdgid();
        }
        std::cout << "." << endl;
      }
    }

    // Transverse missing energy (MET)
    cout << "MET pt=" << event.mc()->MET().pt()
         << " phi=" << event.mc()->MET().phi() << endl;
    cout << endl;

    // Transverse missing hadronic energy (MHT)
    cout << "MHT pt=" << event.mc()->MHT().pt()
              << " phi=" << event.mc()->MHT().phi() << endl;
    cout << endl;

    // Total transverse energy (TET)
    cout << "TET=" << event.mc()->TET() << endl;
    cout << endl;

    // Total transverse hadronic energy (THT)
    cout << "THT=" << event.mc()->THT() << endl; 
   cout << endl;

    */
    return true;
  }
  
  
  
  // ***************************************************************************
  // Example of analysis with reconstructed objects
  // Concerned samples : 
  //   - LHCO samples
  //   - LHE/STDHEP/HEPMC samples after applying jet-clustering algorithm
  // ***************************************************************************
  /*
  // Event weight
  double myWeight=1.;
  if (!Configuration().IsNoEventWeight() && event.mc()!=0) myWeight=event.mc()->weight();

  Manager()->InitializeForNewEvent(myWeight);

  if (event.rec()!=0)
  {
    cout << "---------------NEW EVENT-------------------" << endl;

    // Looking through the reconstructed electron collection
    for (MAuint32 i=0;i<event.rec()->electrons().size();i++)
    {
      const RecLeptonFormat& elec = event.rec()->electrons()[i];
      cout << "----------------------------------" << endl;
      cout << "Electron" << endl;
      cout << "----------------------------------" << endl;
      cout << "index=" << i+1 
                << " charge=" << elec.charge() << endl;
      cout << "px=" << elec.px()
                << " py=" << elec.py()
                << " pz=" << elec.pz()
                << " e="  << elec.e()
                << " m="  << elec.m() << endl;
      cout << "pt=" << elec.pt() 
                << " eta=" << elec.eta() 
                << " phi=" << elec.phi() << endl;
      cout << "pointer address to the matching MC particle: " 
                << elec.mc() << endl;
      cout << endl;
    }

    // Looking through the reconstructed muon collection
    for (MAuint32 i=0;i<event.rec()->muons().size();i++)
    {
      const RecLeptonFormat& mu = event.rec()->muons()[i];
      cout << "----------------------------------" << endl;
      cout << "Muon" << endl;
      cout << "----------------------------------" << endl;
      cout << "index=" << i+1 
                << " charge=" << mu.charge() << endl;
      cout << "px=" << mu.px()
                << " py=" << mu.py()
                << " pz=" << mu.pz()
                << " e="  << mu.e()
                << " m="  << mu.m() << endl;
      cout << "pt=" << mu.pt() 
                << " eta=" << mu.eta() 
                << " phi=" << mu.phi() << endl;
      cout << "ET/PT isolation criterion =" << mu.ET_PT_isol() << endl;
      cout << "pointer address to the matching MC particle: " 
           << mu.mc() << endl;
      cout << endl;
    }

    // Looking through the reconstructed hadronic tau collection
    for (MAuint32 i=0;i<event.rec()->taus().size();i++)
    {
      const RecTauFormat& tau = event.rec()->taus()[i];
      cout << "----------------------------------" << endl;
      cout << "Tau" << endl;
      cout << "----------------------------------" << endl;
      cout << "tau: index=" << i+1 
                << " charge=" << tau.charge() << endl;
      cout << "px=" << tau.px()
                << " py=" << tau.py()
                << " pz=" << tau.pz()
                << " e="  << tau.e()
                << " m="  << tau.m() << endl;
      cout << "pt=" << tau.pt() 
                << " eta=" << tau.eta() 
                << " phi=" << tau.phi() << endl;
      cout << "pointer address to the matching MC particle: " 
           << tau.mc() << endl;
      cout << endl;
    }

    // Looking through the reconstructed jet collection
    for (MAuint32 i=0;i<event.rec()->jets().size();i++)
    {
      const RecJetFormat& jet = event.rec()->jets()[i];
      cout << "----------------------------------" << endl;
      cout << "Jet" << endl;
      cout << "----------------------------------" << endl;
      cout << "jet: index=" << i+1 
           << " charge=" << jet.charge() << endl;
      cout << "px=" << jet.px()
           << " py=" << jet.py()
           << " pz=" << jet.pz()
           << " e="  << jet.e()
           << " m="  << jet.m() << endl;
      cout << "pt=" << jet.pt() 
           << " eta=" << jet.eta() 
           << " phi=" << jet.phi() << endl;
      cout << "b-tag=" << jet.btag()
           << " true b-tag (before eventual efficiency)=" 
           << jet.true_btag() << endl;
      cout << "EE/HE=" << jet.EEoverHE()
           << " ntracks=" << jet.ntracks() << endl;
      cout << endl;
    }

    // Transverse missing energy (MET)
    cout << "MET pt=" << event.rec()->MET().pt()
         << " phi=" << event.rec()->MET().phi() << endl;
    cout << endl;

    // Transverse missing hadronic energy (MHT)
    cout << "MHT pt=" << event.rec()->MHT().pt()
              << " phi=" << event.rec()->MHT().phi() << endl;
    cout << endl;

    // Total transverse energy (TET)
    cout << "TET=" << event.rec()->TET() << endl;
    cout << endl;

    // Total transverse hadronic energy (THT)
    cout << "THT=" << event.rec()->THT() << endl;
    cout << endl;
  }
  */
  return true;
}

