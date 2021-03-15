// I want to get the charge from the list of
// tagged file which will be included in skim file
// Now I use the new way of getting Q
// === Dien 02/15/2021 ======//

#include "bandhit.h"
#include "clashit.h"
#include "constants.h"
#include "kinematic_cuts.h"
#include "taghit.h"

void tagg_check_new() {
  // Getting the runlist_file
  ifstream run_tag, run_empty;
   run_tag.open("./skim_tag_file/test_list.txt");
  run_empty.open("./skim_tag_file/empty_list.txt");

  // run_tag.open("./01-28-2021/tag/test_run.txt");
  // run_empty.open("./01-28-2021/tag/run_empty.txt");
  
  const double L_target = 0.8282; // Density LD2+AL (g/cm2)
  const double L_Al = 0.0162;     // 2 Al windown density (g/cm2)

  double scale_fact = 0;

  TString filename;
  double charge = 0;
  double Q_tot = 0, Q_empty = 0;
  int N_run = 0, N_run_empty = 0;

  TChain *T_tagg = new TChain("tagged");
  TChain *T_empty = new TChain("tagged");

  //=====Getting the tagg data======//
  while (run_tag >> filename) {

    TFile *Tag_file = new TFile(Form("./skim_tag_file/%s", filename.Data()));
    //TFile *Tag_file = new TFile(Form("./01-28-2021/tag/%s", filename.Data()));

    if (Tag_file->IsOpen() == false)
      continue;
    T_tagg->Add(Tag_file->GetName());

     double gated_Q =0;

    TTree *tag_tree = (TTree *)Tag_file->Get("tagged");

    tag_tree->SetBranchAddress("gated_charge",	&gated_Q);

     int Nentries = tag_tree->GetEntries();

    tag_tree->GetEntry(Nentries -1);

    charge = gated_Q;

    Q_tot += charge;

    N_run++;

    cout << N_run << "   " << filename << "  " << charge << endl;
  }

  cout << "Number of run: " << N_run << endl;
  cout << "Q_tot: " << Q_tot << endl;

  //====Getting the empty data ======//
  while (run_empty >> filename) {

     TFile *Tag_file = new TFile(Form("./skim_tag_file/%s", filename.Data()));
     // TFile *Tag_file = new TFile(Form("./01-28-2021/tag/%s", filename.Data()));

    if (Tag_file->IsOpen() == false)
      continue;
    T_empty->Add(Tag_file->GetName());
    
    double gated_Q =0;

    TTree *tag_tree = (TTree *)Tag_file->Get("tagged");

    tag_tree->SetBranchAddress("gated_charge",	&gated_Q);

     int Nentries = tag_tree->GetEntries();

    tag_tree->GetEntry(Nentries -1);

    charge = gated_Q;

    Q_empty += charge;

    N_run_empty++;

    cout << N_run_empty << "  " << filename << "  " << charge << endl;
  }

  cout << "Number of empty run: " << N_run_empty << endl;
  cout << "Q_empty: " << Q_empty << endl;


  // scale_fact = Q_tot * L_target / Q_empty / L_Al;
  scale_fact = Q_tot / Q_empty ;

  cout << "Scaling Tagg/Empty: " << scale_fact << endl;
  cout << "Number of empty entries " << T_empty->GetEntries() << endl;
  cout << "Number of Tagged entries: " << T_tagg->GetEntries() << endl;

  

  //=== I can set up the cuts here to use for both file (tag and empty)====//
  //=====The same cuts as Final skim code using======//

  // Conditions for a final accepted event electron
  TCut ePID = Form("eHit->getPID() == %i", ECUT_PID);
  TCut eCharge = Form("eHit->getCharge() == %i", ECUT_charge);
  TCut eEoP = Form("eHit->getEoP() > %f && eHit->getEoP() < %f", ECUT_EoP_min,
                   ECUT_EoP_max);
  TCut eEpcal = Form("eHit->getEpcal() > %f", ECUT_Epcal_min);
  TCut eVW =
      Form("eHit->getV() > %f && eHit->getW() > %f", ECUT_V_min, ECUT_W_min);
  TCut eVtx = Form("eHit->getVtz() > %f && eHit->getVtz() < %f", ECUT_vtx_min,
                   ECUT_vtx_max);
  TCut eMom = Form("eHit->getMomentum() > %f && eHit->getMomentum() < %f",
                   ECUT_pE_min, ECUT_pE_max);
  TCut eQ2 = Form("eHit->getQ2() > %f && eHit->getQ2() < %f", ECUT_Q2_min,
                  ECUT_Q2_max);
  TCut eW = Form("eHit->getW2() > %f", ECUT_W2_min);
  TCut inclusive =
      ePID && eCharge && eEoP && eEpcal && eVW && eVtx && eMom && eQ2 && eW;

  // Conditions for a final accepted event neutron - signal or background
  TCut nGood = Form("goodneutron == %i", NCUT_goodneutron);
  TCut nLeadIdx = Form("nleadindex != %i", NCUT_leadindex);
  TCut nStatus = Form("nHits[nleadindex]->getStatus() == %i", NCUT_status);
  TCut nEdep =
      Form("nHits[nleadindex]->getEdep() > %f", NCUT_Edep * DataAdcToMeVee);
  TCut nThetaNQ = Form("tag[nleadindex]->getThetaNQ() > %f && "
                       "tag[nleadindex]->getThetaNQ() < %f",
                       NCUT_THETANQ_min, NCUT_THETANQ_max);
  TCut tagged = inclusive && nGood && nLeadIdx && nStatus && nEdep && nThetaNQ;
  
  //TCut tagged = inclusive && nGood && nLeadIdx && nStatus && nThetaNQ;
  // Conditions for a final accepted event neutron in signal region
  TCut nToF = Form("nHits[nleadindex]->getTofFadc() > %f", NCUT_Tofabove0);
  TCut nPn = Form("tag[nleadindex]->getMomentumN().Mag() > %f && "
                  "tag[nleadindex]->getMomentumN().Mag() < %f",
                  NCUT_Pn_min, NCUT_Pn_max);
  TCut nPnNaN = Form("tag[nleadindex]->getMomentumN().Mag() == "
                     "tag[nleadindex]->getMomentumN().Mag()");
  TCut nWp =
      Form("tag[nleadindex]->getWp() > %f && tag[nleadindex]->getWp() < %f",
           NCUT_Wp_min, NCUT_Wp_max);
  TCut nAs =
      Form("tag[nleadindex]->getAs() > %f && tag[nleadindex]->getAs() < %f",
           NCUT_As_min, NCUT_As_max);
  TCut tagged_signal = tagged && nToF && nPn && nPnNaN && nWp && nAs;

  // TFile *outfile = new TFile("wall_his.root", "RECREATE");

  //====Define histogram for TOF and vertex z=====//
  TH1D *h1_TOF_tag = new TH1D("h1_TOF_tag", "", 100, -100, 100);
  TH1D *h1_vz_tag = new TH1D("h1_vz_tag", "", 50, -10, 5);
  TH1D *h1_xp_tag = new TH1D("h1_xp_tag", "", 35, 0.1, 0.8);

  TH1D *h1_TOF_empty = new TH1D("h1_TOF_empty", "", 100, -100, 100);
  TH1D *h1_vz_empty = new TH1D("h1_vz_empty", "", 50, -10, 5);
  TH1D *h1_xp_empty = new TH1D("h1_xp_empty", "", 35, 0.1, 0.8);

  TH1D *h1_xp_ratio = new TH1D("h1_xp_ratio", "", 35, 0.1, 0.8);

  /*
  TCanvas *c0 = new TCanvas("c0", "", 1000, 600);
  c0->Divide(2, 1);
  c0->cd(1);
  T_empty->Draw("tag[nleadindex]->getXp()>> h1_xp_empty", tagged_signal, "");

  c0->cd(2);
  T_tagg->Draw("tag[nleadindex]->getXp()>>h1_xp_tag", tagged_signal, "");

  TCanvas *c00 = new TCanvas("c00", "", 1000, 600);
  c00->Divide(2, 1);
  c00->cd(1);
  h1_xp_tag->Draw();
  h1_xp_empty->Scale(scale_fact);
  h1_xp_empty->Draw("same hist");
  h1_xp_empty->SetLineColor(2);
  h1_xp_empty->SetLineWidth(2);

  c00->cd(2);
  h1_xp_ratio->Divide(h1_xp_empty, h1_xp_tag);
  h1_xp_ratio->Draw();
  */
 

  // Ploting the empty run
  TCanvas *c1 = new TCanvas("c1", "", 1000, 600);
  c1->Divide(2, 1);
  c1->cd(1);
  T_empty->Draw("nHits[nleadindex]->getTof() >> h1_TOF_empty", tagged_signal, "");
  c1->cd(2);
  T_empty->Draw("eHit->getVtz() >> h1_vz_empty", tagged_signal, "");

  // h1_TOF_empty->Write();
  //h1_vz_empty->Write();


  
  TCanvas *c2 = new TCanvas("c2", "", 1000, 600);
  c2->Divide(2, 1);
  c2->cd(1);
  T_tagg->Draw("nHits[nleadindex]->getTof() >> h1_TOF_tag", tagged_signal, "");
  h1_TOF_empty->Scale(scale_fact);
  h1_TOF_empty->Draw("same hist");
  h1_TOF_empty->SetLineColor(2);
  h1_TOF_empty->SetLineWidth(2);

  // h1_TOF_tag->SetMaximum(h1_TOF_empty->GetMaximum());
  c2->cd(2);
  T_tagg->Draw("eHit->getVtz() >> h1_vz_tag", tagged_signal, "");
  h1_vz_empty->Scale(scale_fact);
  h1_vz_empty->Draw("same hist");
  h1_vz_empty->SetLineColor(2);
  h1_vz_empty->SetLineWidth(2);
  // h1_vz_tag->SetMaximum(h1_vz_empty->GetMaximum());
  //h1_TOF_tag->Write();
  //h1_vz_tag->Write();


  
  cout << "Endcap contribution: TOF "
       << h1_TOF_empty->Integral() / h1_TOF_tag->Integral() << endl;
  cout << "Endcap contribution: vz "
       << h1_vz_empty->Integral() / h1_vz_tag->Integral() << endl;


}
