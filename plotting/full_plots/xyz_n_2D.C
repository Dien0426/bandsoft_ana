// Plots 2D plots for xn, yn, zn
// Data ans simulation (xn, yn), (xn, zn), (yn, zn)

void xyz_n_2D(TString inDat, TString inSim) {

  gSystem->Load("libclashit_tree.so");
  gSystem->Load("libtaghit_tree.so");
  gSystem->Load("libbandhit_tree.so");
  gSystem->Load("libgenpart_tree.so");

  gStyle->SetOptStat(0);
  // Load TFiles
  TFile *inFileDat = new TFile(inDat);
  TFile *inFileSim = new TFile(inSim);

  // Get the TTrees
  TTree *inTreeDat = (TTree *)inFileDat->Get("tagged");
  TTree *inTreeSim = (TTree *)inFileSim->Get("tagged");

  // Data histogram
  TH2D *h2_xy_n_dat =
      new TH2D("h2_xy_n_dat", "yn vs xn Data", 30, -120, 120, 300, -50, 100);
  TH2D *h2_xz_n_dat =
      new TH2D("h2_xz_n_dat", "zn vs xn Data", 30, -120, 120, 80, -300, -260);
  TH2D *h2_yz_n_dat =
      new TH2D("h2_yz_n_dat", " yn vs zn Data", 80, -300, -260, 300, -50, 100);

  // MC histogram
  TH2D *h2_xy_n_MC =
      new TH2D("h2_xy_n_MC", "yn vs xn MC", 30, -120, 120, 300, -50, 100);

  TH2D *h2_xz_n_MC =
      new TH2D("h2_xz_n_MC", "zn vs xn MC", 30, -120, 120, 80, -300, -260);

  TH2D *h2_yz_n_MC =
      new TH2D("h2_yz_n_MC", " yn vs zn MC", 80, -300, -260, 300, -50, 100);

  // Doing the 2D plots
  TCanvas *c1 = new TCanvas("c1", "", 1200, 1000);
  c1->Divide(3, 2);
  c1->cd(1);
  inTreeDat->Draw("nHits[nleadindex]->getDL().Y(): "
                  "nHits[nleadindex]->getDL().X() >> h2_xy_n_dat",
                  "tag[nleadindex]->getMomentumN().Mag() > 0.3", "colz");

  c1->cd(2);
  inTreeDat->Draw("nHits[nleadindex]->getDL().Z(): "
                  "nHits[nleadindex]->getDL().X() >> h2_xz_n_dat",
                  "tag[nleadindex]->getMomentumN().Mag() > 0.3", "colz");

  c1->cd(3);
  inTreeDat->Draw("nHits[nleadindex]->getDL().Y(): "
                  "nHits[nleadindex]->getDL().Z() >> h2_yz_n_dat",
                  "tag[nleadindex]->getMomentumN().Mag() > 0.3", "colz");

  c1->cd(4);
  inTreeSim->Draw("nHits[nleadindex]->getDL().Y(): "
                  "nHits[nleadindex]->getDL().X() >> h2_xy_n_MC",
                  "tag[nleadindex]->getMomentumN().Mag() > 0.3", "colz");

  c1->cd(5);
  inTreeSim->Draw("nHits[nleadindex]->getDL().Z(): "
                  "nHits[nleadindex]->getDL().X() >> h2_xz_n_MC",
                  "tag[nleadindex]->getMomentumN().Mag() > 0.3", "colz");

  c1->cd(6);
  inTreeSim->Draw("nHits[nleadindex]->getDL().Y(): "
                  "nHits[nleadindex]->getDL().Z() >> h2_yz_n_MC",
                  "tag[nleadindex]->getMomentumN().Mag() > 0.3", "colz");

  c1->Print("./xyz_n_2D.pdf");
}
