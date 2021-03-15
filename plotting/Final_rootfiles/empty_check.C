// This code: checking the distribution of empty run
// And need to think of how to do this subtraction

#include "constants.h"
#include "kinematic_cuts.h"

#include "bandhit.h"
#include "clashit.h"
#include "taghit.h"

void empty_check() {

  //===Getting empty Run first====//
  // Get the TTrees
  TChain *inTree = new TChain("tagged");
  inTree->Add("./Empty_run/tagged_skim_006599.root");
  inTree->Add("./Empty_run/tagged_skim_006601.root");
  inTree->Add("./Empty_run/tagged_skim_006603.root");

  double scale = 7.55; //

  // Define histogram
  TH1D *h1_TOF = new TH1D("h1_TOF", "", 200, -150, 350);
  TH1D *h1_vz = new TH1D("h1_vz", "", 150, -10, 5);

  // Initialize the input branches
  clashit *input_eHit = new clashit;
  TClonesArray *input_nHit = new TClonesArray("bandhit");
  TClonesArray *input_tag = new TClonesArray("taghit");
  int input_nleadindex = -1;
  bool input_ngoodneutron = false;
  double input_Ebeam = 0;

  inTree->SetBranchAddress("eHit", &input_eHit);
  inTree->SetBranchAddress("Ebeam", &input_Ebeam);
  inTree->SetBranchAddress("nHits", &input_nHit);
  inTree->SetBranchAddress("tag", &input_tag);
  inTree->SetBranchAddress("nleadindex", &input_nleadindex);
  inTree->SetBranchAddress("goodneutron", &input_ngoodneutron);

  // Set up some cut
  const double max_CosTheta_nq = cos(NCUT_THETANQ_min);
  const double min_CosTheta_nq = cos(NCUT_THETANQ_max);

  // Loop over the tagg event
  for (int ev = 0; ev < inTree->GetEntries(); ev++) {
    input_eHit->Clear();
    input_nHit->Clear();
    input_tag->Clear();

    inTree->GetEntry(ev);

    // Require good neutron
    if (input_ngoodneutron != NCUT_goodneutron)
      continue;
    if (input_nleadindex == NCUT_leadindex)
      continue;

    // Grab our good neutron from bandhit
    bandhit *lead_n = (bandhit *)input_nHit->At(input_nleadindex);
    bandhit copy_n;
    copy_n = *lead_n;

    // Check to see if the neutron is within costhnq bin
    // Using the taghit
    taghit *this_tag = (taghit *)input_tag->At(0);
    double this_CosThetaNQ = cos(this_tag->getThetaNQ());
    if (this_CosThetaNQ > NCUT_COSTHETANQ_max ||
        this_CosThetaNQ < NCUT_COSTHETANQ_min)
      continue;

    // Checking Edep cut and status
    if (lead_n->getStatus() != NCUT_status)
      continue;
    if (lead_n->getEdep() > NCUT_Edep * DataAdcToMeVee)
      continue;

    // Require a good electron event:
    if (input_eHit->getPID() != ECUT_PID)
      continue;
    if (input_eHit->getCharge() != ECUT_charge)
      continue;
    if (input_eHit->getEoP() < ECUT_EoP_min ||
        input_eHit->getEoP() > ECUT_EoP_max)
      continue;
    if (input_eHit->getEpcal() < ECUT_Epcal_min)
      continue;
    if (input_eHit->getV() < ECUT_V_min || input_eHit->getW() < ECUT_W_min)
      continue;
    if (input_eHit->getVtz() < ECUT_vtx_min ||
        input_eHit->getVtz() > ECUT_vtx_max)
      continue;
    if (input_eHit->getMomentum() < ECUT_pE_min ||
        input_eHit->getMomentum() > ECUT_pE_max)
      continue;
    if (input_eHit->getQ2() < ECUT_Q2_min || input_eHit->getQ2() > ECUT_Q2_max)
      continue;
    if (input_eHit->getW2() < ECUT_W2_min)
      continue;

    h1_TOF->Fill(lead_n->getTof());
    h1_vz->Fill(input_eHit->getVtz());
  }

  TCanvas *c1 = new TCanvas("c1", "", 800, 600);
  c1->Divide(2, 1);
  c1->cd(1);
  h1_TOF->Draw();
  h1_TOF->GetXaxis()->SetTitle("TOF");

  c1->cd(2);
  h1_vz->Draw();
}
