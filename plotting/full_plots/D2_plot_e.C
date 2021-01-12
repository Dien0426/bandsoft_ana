// electron 2D plots
// only use the inclusive data file for now

void D2_plot_e(TString inFileDatIncName) {
  gSystem->Load("libclashit_tree.so");
  gSystem->Load("libtaghit_tree.so");
  gSystem->Load("libbandhit_tree.so");
  gSystem->Load("libgenpart_tree.so");

  gStyle->SetOptStat(0);

  // Loadfile
  TFile *inFileDatInc = new TFile(inFileDatIncName);

  // Get the tree
  TTree *inTreeDatInc = (TTree *)inFileDatInc->Get("electrons");

  // Define histogram:

  TH2D *h2_xB_Q2 =
      new TH2D("h2_xB_Q2", "Q^{2} vs x_{B}", 20, 0.1, 0.5, 20, 2, 6);
  TH2D *h2_Q2_W = new TH2D("h2_Q2_W", "Q^{2} vs W", 18, 2, 3.8, 20, 2, 6);
  TH2D *h2_theta_p_e =
      new TH2D("h2_theta_p_e", "#theta_{e} vs P_{e}", 50, 2, 8, 30, 5, 35);

  // Doing plots
  TCanvas *c1 = new TCanvas("c1", "", 800, 600);
  inTreeDatInc->Draw("eHit->getQ2():eHit->getXb() >> h2_xB_Q2", "", "colz");
  h2_xB_Q2->GetXaxis()->SetTitle("x_{B}");
  h2_xB_Q2->GetYaxis()->SetTitle("Q^{2}");
  c1->Print("../2d_plot/inc_xb_Q2.pdf");

  TCanvas *c2 = new TCanvas("c2", "", 800, 600);
  inTreeDatInc->Draw("eHit->getQ2():sqrt(eHit->getW2()) >> h2_Q2_W", "",
                     "colz");
  h2_Q2_W->GetXaxis()->SetTitle("W");
  h2_Q2_W->GetYaxis()->SetTitle("Q^{2}");

  c2->Print("../2d_plot/inc_Q2_W.pdf");

  TCanvas *c3 = new TCanvas("c3", "", 800, 600);
  inTreeDatInc->Draw(
      "(eHit->getTheta()*180/3.14):eHit->getMomentum()>>h2_theta_p_e", "",
      "colz");
  h2_theta_p_e->GetXaxis()->SetTitle("p_{e}");
  h2_theta_p_e->GetYaxis()->SetTitle("#theta_{e}");
  c3->Print("../2d_plot/inc_theta_p_e.pdf");
}
