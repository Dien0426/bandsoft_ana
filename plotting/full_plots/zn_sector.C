// Doing plots for xn after calibration for Data and MC

void zn_sector(TString inDat, TString inBac, TString inSim) {

  // Define some function used
  void label1D(TH1D * data, TH1D * sim, TString xlabel, TString ylabel);

  // Load TFiles
  TFile *inFileDat = new TFile(inDat);
  TFile *inFileBac = new TFile(inBac);
  TFile *inFileSim = new TFile(inSim);

  // Get the TTrees
  TTree *inTreeDat = (TTree *)inFileDat->Get("tagged");
  TTree *inTreeBac = (TTree *)inFileBac->Get("tagged");
  TTree *inTreeSim = (TTree *)inFileSim->Get("tagged");

  // Get and set the background normalization
  TVector3 *datnorm = (TVector3 *)inFileDat->Get("bacnorm");
  TVector3 *bacnorm = (TVector3 *)inFileBac->Get("bacnorm");
  // inTreeBac->SetWeight(122669. / bacnorm->X());
  inTreeBac->SetWeight(95801.8 / bacnorm->X());

  int N_layer = 5;
  int N_sector = 5;
  int N_bar = 10;

  TCanvas *c0 = new TCanvas("c0");
  c0->Print("Zn_sector_Dat_MC_check.pdf[");
  TCanvas *c00 = new TCanvas("c00");
  c00->Print("Zn_sector_Dat_BG_check.pdf[");

  TCanvas *c000 = new TCanvas("c000");
  c000->Print("Zn_Csim_layer_sector.pdf[");

  for (int l = 1; l < N_layer + 1; l++) {

    TH1D *h1_csim_layer =
        new TH1D(Form("h1_csim_layer_%d", l), "", 40, 0.15, 0.55);
    TString title_csim = Form("Csim Layer %d", l);
    h1_csim_layer->SetTitle(title_csim);

    for (int s = 1; s < N_sector + 1; s++) {
      // for (int b = 1; b < N_bar; b++) {

      TH1D *h1_dat = new TH1D(Form("h1_dat_l%d_s%d", l, s), "", 80, -300, -260);

      TH1D *h1_BG = new TH1D(Form("h1_BG_l%d_s%d", l, s), "", 80, -300, -260);

      h1_BG->SetLineColor(2);
      TH1D *h1_MC = new TH1D(Form("h1_MC_l%d_s%d", l, s), "", 80, -300, -260);

      h1_MC->SetLineColor(kGreen);

      TCut cut = Form("nHits[nleadindex]->getLayer() == %d && "
                      "nHits[nleadindex]->getSector()== %d",
                      l, s);

      // Plot Data and BG
      TCanvas *cc = new TCanvas(Form("cc_Data_BG_l%d_s%d", l, s), "", 800, 600);
      TString title0 = Form("Layer=%d, Sector=%d", l, s);

      inTreeDat->Draw(
          Form("nHits[nleadindex]->getDL().Z() >> h1_dat_l%d_s%d", l, s), cut,
          "");
      h1_dat->SetTitle(title0);
      h1_dat->GetXaxis()->SetTitle("z [cm]");
      h1_dat->GetYaxis()->SetTitle("Count");

      inTreeBac->Draw(
          Form("nHits[nleadindex]->getDL().Z() >> h1_BG_l%d_s%d", l, s), cut,
          "same hist");

      if (h1_dat->GetEntries() > 0) {

        cc->Print("Zn_sector_Dat_BG_check.pdf");
      }

      // Background subraction
      if (h1_dat->GetEntries() > 0 && h1_BG->GetEntries() > 0) {
        TCanvas *c = new TCanvas(Form("c_Data_MC_l%d_s%d", l, s), "", 800, 600);

        h1_dat->Add(h1_BG, -1);
        inTreeSim->Draw(
            Form("nHits[nleadindex]->getDL().Z() >> h1_MC_l%d_s%d", l, s), cut,
            "");

        double scaling = (double)h1_dat->Integral() / h1_MC->Integral();

        h1_csim_layer->Fill(scaling);

        TString title = Form("Layer=%d, Sector=%d, Csim =%2.4f", l, s, scaling);
        h1_MC->Scale(scaling);
        h1_MC->SetTitle(title);
        // h1_MC->Draw("hist");
        // h1_dat->Draw("same hist");
        label1D(h1_dat, h1_MC, "z [cm]", "Counts");
        c->Print("Zn_sector_Dat_MC_check.pdf");
      }
      //}
    }
    TCanvas *c_sim = new TCanvas(Form("c_sim_%d", l), "", 800, 600);
    h1_csim_layer->Draw("hist");
    c_sim->Print("Zn_Csim_layer_sector.pdf");
  }

  c000->Print("Zn_Csim_layer_sector.pdf]");
  c0->Print("Zn_sector_Dat_MC_check.pdf]");
  c00->Print("Zn_sector_Dat_BG_check.pdf]");
}

void label1D(TH1D *data, TH1D *sim, TString xlabel, TString ylabel) {
  data->SetLineColor(4);
  data->SetMarkerColor(4);
  data->SetMarkerStyle(8);
  data->SetMarkerSize(1);
  data->SetStats(0);

  sim->SetLineColor(2);
  sim->SetLineWidth(1);
  sim->SetStats(0);

  sim->Draw("hist");
  data->Draw("p,same");

  double max1 = data->GetMaximum() * 1.1;
  double max2 = sim->GetMaximum() * 1.1;
  sim->GetYaxis()->SetRangeUser(0, max(max1, max2));

  sim->GetXaxis()->SetTitle(xlabel);
  sim->GetYaxis()->SetTitle(ylabel);

  TLegend *legend = new TLegend(0.7, 0.8, 0.9, 0.9);
  // legend->AddEntry(data,"Radiation On","f");
  // legend->AddEntry(sim,"Radiation Off","f");
  legend->AddEntry(data, "Data", "f");
  legend->AddEntry(sim, "Sim", "f");
  legend->Draw();

  return;
}
