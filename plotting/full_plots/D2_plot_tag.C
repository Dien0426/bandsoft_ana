// 2D plots for the tag
// Only use the data for  now

void D2_plot_tag(TString inDat) {

  gSystem->Load("libclashit_tree.so");
  gSystem->Load("libtaghit_tree.so");
  gSystem->Load("libbandhit_tree.so");
  gSystem->Load("libgenpart_tree.so");

  gStyle->SetOptStat(0);

  // Load file
  TFile *inFileDat = new TFile(inDat);

  // Load the tree
  TTree *T = (TTree *)inFileDat->Get("tagged");

  // define 2D histogram
  TH2D *h2_pn_pt = new TH2D("h2_pn_pt", "pn vs pt", 25, 0, 0.25, 40, 0.2, 0.6);
  TH2D *h2_pt_thnq =
      new TH2D("h2_pt_thnq", "pt vs #theta_{nq}", 40, 140, 180, 25, 0, 0.25);
  TH2D *h2_pt_phnq =
      new TH2D("h2_pt_phnq", "pt vs #phi_{nq}", 45, -180, 180, 25, 0, 0.25);

  TH2D *h2_xp_wp =
      new TH2D("h2_xp_wp", "x^{'} vs W^{'}", 14, 1.6, 3, 35, 0.1, 0.8);
  TH2D *h2_pn_vir =
      new TH2D("h2_pn_vir", "pn vs vir", 25, -0.6, -0.1, 40, 0.2, 0.6);
  TH2D *h2_vir_as =
      new TH2D("h2_vir_as", "vir vs Alphas", 20, 1.2, 1.7, 25, -0.6, -0.1);

  TH2D *h2_pl_pt =
      new TH2D("h2_pl_pt", "p_{||} vs pt", 25, 0, 0.25, 40, 0.2, 0.6);

  // Plots

  TCanvas *c0 = new TCanvas("c0", "", 800, 600);

  T->Draw("sqrt(pow(tag[nleadindex]->getMomentumN().Mag(),2) - "
          "pow(tag[nleadindex]->getPt().Mag(),2)) : "
          "tag[nleadindex]->getPt().Mag() >> h2_pl_pt",
          "tag[nleadindex]->getMomentumN().Mag() > 0.3", "colz");

  h2_pl_pt->GetXaxis()->SetTitle("pt");
  h2_pl_pt->GetYaxis()->SetTitle("p_{||}");
  c0->Print("../2d_plot/tag_pl_pt.pdf");

  TCanvas *c1 = new TCanvas("c1", "", 800, 600);
  T->Draw("tag[nleadindex]->getMomentumN().Mag() : "
          "tag[nleadindex]->getPt().Mag()>> h2_pn_pt",
          "tag[nleadindex]->getMomentumN().Mag() > 0.3", "colz");
  h2_pn_pt->GetXaxis()->SetTitle("pt");
  h2_pn_pt->GetYaxis()->SetTitle("pn");

  c1->Print("../2d_plot/tag_pn_pt.pdf");

  TCanvas *c2 = new TCanvas("c2", "", 800, 600);
  T->Draw("tag[nleadindex]->getPt().Mag() : "
          "tag[nleadindex]->getThetaNQ()*180./TMath::Pi() >> h2_pt_thnq",
          "tag[nleadindex]->getMomentumN().Mag() > 0.3", "colz");

  h2_pt_thnq->GetXaxis()->SetTitle("#theta_{nq}");
  h2_pt_thnq->GetYaxis()->SetTitle("pt");

  c2->Print("../2d_plot/tag_pt_thnq.pdf");

  TCanvas *c3 = new TCanvas("c3", "", 800, 600);
  T->Draw("tag[nleadindex]->getPt().Mag() : "
          "tag[nleadindex]->getPhiNQ()*180./TMath::Pi() >> h2_pt_phnq",
          "tag[nleadindex]->getMomentumN().Mag() > 0.3", "colz");

  h2_pt_phnq->GetXaxis()->SetTitle("#phi_{nq}");
  h2_pt_phnq->GetYaxis()->SetTitle("pt");

  c3->Print("../2d_plot/tag_pt_phnq.pdf");

  TCanvas *c4 = new TCanvas("c4", "", 800, 600);
  T->Draw("tag[nleadindex]->getXp2() : tag[nleadindex]->getWp() >> h2_xp_wp",
          "tag[nleadindex]->getMomentumN().Mag() > 0.3", "colz");
  h2_xp_wp->GetXaxis()->SetTitle("W^{'}");
  h2_xp_wp->GetYaxis()->SetTitle("x^{'}");

  c4->Print("../2d_plot/tag_xp_wp.pdf");

  TCanvas *c5 = new TCanvas("c5", "", 800, 600);

  T->Draw("tag[nleadindex]->getMomentumN().Mag() : (( pow(1.8756 - sqrt( "
          "pow(tag[nleadindex]->getMomentumN().Mag(),2) + pow(0.9396 ,2) ) ,2) "
          "- pow( tag[nleadindex]->getMomentumN().Mag() , 2 ) - pow(0.93827,2) "
          ")/pow(0.93827,2)) >> h2_pn_vir",
          "tag[nleadindex]->getMomentumN().Mag() > 0.3", "colz");

  h2_pn_vir->GetXaxis()->SetTitle("Vir");
  h2_pn_vir->GetYaxis()->SetTitle("pn");
  c5->Print("../2d_plot/tag_pn_vir.pdf");

  TCanvas *c6 = new TCanvas("c6", "", 800, 600);

  T->Draw("((pow(1.8756 - sqrt( pow(tag[nleadindex]->getMomentumN().Mag(),2) "
          "+ pow(0.9396 ,2) ) ,2) - pow( tag[nleadindex]->getMomentumN().Mag() "
          ", 2 ) - pow(0.93827,2) )/pow(0.93827,2)):tag[nleadindex]->getAs() "
          ">>h2_vir_as",
          "tag[nleadindex]->getMomentumN().Mag() > 0.3", "colz");

  h2_vir_as->GetXaxis()->SetTitle("AlphaS");
  h2_vir_as->GetYaxis()->SetTitle("Vir");

  c6->Print("../2d_plot/tag_vir_as.pdf");
}