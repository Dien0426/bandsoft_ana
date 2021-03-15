void pn_thetan_layer(TString inDat, TString inBac, TString inSim, int layer){

	// Define some function used
	void label1D(TH1D* data, TH1D* sim, TString xlabel, TString ylabel);
	void label1D_ratio(TH1D* data, TH1D* sim, TString xlabel, TString ylabel, double ymin , double ymax );

	// Load TFiles
	TFile * inFileDat = new TFile(inDat);
	TFile * inFileBac = new TFile(inBac);
	TFile * inFileSim = new TFile(inSim);

	// Get the TTrees
	TTree * inTreeDat = (TTree*) inFileDat->Get("tagged");
	TTree * inTreeBac = (TTree*) inFileBac->Get("tagged");
	TTree * inTreeSim = (TTree*) inFileSim->Get("tagged");

	// Get and set the background normalization
	TVector3 * datnorm = (TVector3*)inFileDat->Get("bacnorm");
	TVector3 * bacnorm = (TVector3*)inFileBac->Get("bacnorm");
	//	inTreeBac->SetWeight( datnorm->X() / bacnorm->X() );
        inTreeBac->SetWeight(122669./ bacnorm->X());
	

	// Define histograms we want to plot:
	TH2D ** pn_thn_dat = new TH2D*[3];
	TH2D ** pn_thn_bac = new TH2D*[3];
	TH2D ** pn_thn_sim = new TH2D*[3];
	TH2D ** pn_thn_rat = new TH2D*[3];
	for(int i = 0 ; i < 3 ; i++){
	  pn_thn_dat[i] = new TH2D(Form("pn_thn_dat_%i",i),"",15,0.25,0.55,15, 150, 180);
	  pn_thn_bac[i] = new TH2D(Form("pn_thn_bac_%i",i),"",15,0.25,0.55,15, 150, 180);
	  pn_thn_sim[i] = new TH2D(Form("pn_thn_sim_%i",i),"",15,0.25,0.55,15, 150, 180);
	  pn_thn_rat[i] = new TH2D(Form("pn_thn_rat_%i",i),"",15,0.25,0.55,15, 150, 180);
			       
	}

	// Draw the full pn distribution

	//Define the cut on layer number
	TCut layer_cut = Form("nHits[nleadindex]->getLayer() == %d", layer);
	
	TCanvas * c_pn = new TCanvas("c_pn","",1200,900);

	TCanvas **c = new TCanvas*[3];

	TCanvas *c0 = new TCanvas("c0");
	c0->Print(Form("./pn_thetan_layer_check_plots/2D_pn_thn_layler_%d.pdf[", layer));
	double sim_scaling = 0;
	c_pn->Divide(3,2);
	for( int i = 0 ; i < 3 ; i++){
		TCut pTcut = "";
		TString pTtitle = "Full pT";
		if( i == 1 ){
			pTcut = "tag[nleadindex]->getPt().Mag() < 0.1";
			pTtitle = "Low pT";
		}
		if( i == 2 ){
			pTcut = "tag[nleadindex]->getPt().Mag() >= 0.1";
			pTtitle = "High pT";
		}

		c_pn->cd(i+1);
		inTreeDat->Draw(Form("tag[nleadindex]->getMomentumN().Theta()*180./TMath::Pi():tag[nleadindex]->getMomentumN().Mag() >> pn_thn_dat_%i",i),"tag[nleadindex]->getMomentumN().Mag() > 0.3" && pTcut && layer_cut);
		inTreeBac->Draw(Form("tag[nleadindex]->getMomentumN().Theta()*180./TMath::Pi():tag[nleadindex]->getMomentumN().Mag() >> pn_thn_bac_%i",i),"tag[nleadindex]->getMomentumN().Mag() > 0.3" && pTcut && layer_cut);
		inTreeSim->Draw(Form("tag[nleadindex]->getMomentumN().Theta()*180./TMath::Pi():tag[nleadindex]->getMomentumN().Mag() >> pn_thn_sim_%i",i),"tag[nleadindex]->getMomentumN().Mag() > 0.3" && pTcut && layer_cut);

		// Background subraction
	       	pn_thn_dat[i]->Add(pn_thn_bac[i],-1);

		// Simulation scaling only from no pT cut distribution (i.e. from full distribution)
		double full_simnorm = (double)pn_thn_dat[0]->Integral() / pn_thn_sim[0]->Integral();
	
		
		if( i == 0 ) sim_scaling = full_simnorm;
		pn_thn_sim[i]->Scale( sim_scaling );

		double sec_simnorm = (double)pn_thn_dat[i]->Integral()/ pn_thn_sim[i]->Integral();

		pn_thn_rat[i]->Divide(pn_thn_dat[i], pn_thn_sim[i]);
		
		
		pn_thn_sim[i]->SetTitle(pTtitle+Form(", C_{full} = %1.3f, layer %d, C = %1.3f, MC",sim_scaling, layer, sec_simnorm));

		pn_thn_sim[i]->Draw("colz");
		//	pn_thn_sim[i]->Draw("same text");
		
		//label1D(pn_dat[i],pn_sim[i],"|p_{n}| [GeV/c]","Counts");

		c_pn->cd(4+i);
		//label1D_ratio(pn_dat[i],pn_sim[i],"|p_{n}| [GeV/c]","Data/Sim",0,2);
		pn_thn_dat[i]->SetTitle(pTtitle+Form(", C_{full} = %1.3f, layer %d, C = %1.3f, Dat",sim_scaling, layer, sec_simnorm));
		pn_thn_dat[i]->Draw("colz");
		//	pn_thn_dat[i]->Draw("same text");

		c[i] = new TCanvas(Form("c_%d", i), "", 1200, 900);
		pn_thn_rat[i]->SetTitle(pTtitle+Form(", C_{full} = %1.3f, layer %d, C = %1.3f, Ratio",sim_scaling, layer, sec_simnorm));
		pn_thn_rat[i]->Draw("colz");
		
                gStyle->SetPaintTextFormat("1.2f");
		pn_thn_rat[i]->Draw("same text");
		pn_thn_rat[i]->SetMarkerColor(kWhite);

		c[i]->Print(Form("./pn_thetan_layer_check_plots/2D_pn_thn_layler_%d.pdf", layer));
	}


       	c_pn->SaveAs(Form("./pn_thetan_layer_check_plots/full_pn_thn_layer_%d.pdf", layer));
	c0->Print(Form("./pn_thetan_layer_check_plots/2D_pn_thn_layler_%d.pdf]", layer));

	return;
}

void label1D(TH1D* data, TH1D* sim, TString xlabel, TString ylabel){
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

	double max1 = data->GetMaximum()*1.1;
	double max2 = sim->GetMaximum()*1.1;
	sim->GetYaxis()->SetRangeUser(0,max(max1,max2));
	
	sim->GetXaxis()->SetTitle(xlabel);
	sim->GetYaxis()->SetTitle(ylabel);

	TLegend * legend = new TLegend(0.7,0.8,0.9,0.9);
	//legend->AddEntry(data,"Radiation On","f");
	//legend->AddEntry(sim,"Radiation Off","f");
	legend->AddEntry(data,"Data","f");
	legend->AddEntry(sim,"Sim","f");
	legend->Draw();

	return;
}

void label1D_ratio(TH1D* data, TH1D* sim, TString xlabel, TString ylabel, double ymin , double ymax ){
	gStyle->SetOptFit(1);
	
	TH1D * data_copy = (TH1D*) data->Clone();
	TH1D * sim_copy = (TH1D*) sim->Clone();
	
	data_copy->SetLineColor(1);
	data_copy->SetLineWidth(3);
	//data_copy->SetStats(0);

	sim_copy->SetLineColor(9);
	sim_copy->SetLineWidth(3);
	//sim_copy->SetStats(0);
	//sim_copy->Scale(data_copy->Integral() / sim_copy->Integral() );

	data_copy->Divide(sim_copy);
	data_copy->SetTitle( sim_copy->GetTitle() );
	data_copy->Draw("ep");
	TLine* line = new TLine(data_copy->GetXaxis()->GetBinCenter(1), 1., data_copy->GetXaxis()->GetBinCenter(data_copy->GetXaxis()->GetNbins()), 1.);
	line->SetLineWidth(3);
	line->SetLineColor(2);
	line->Draw("same");

	data_copy->GetYaxis()->SetRangeUser(ymin,ymax);
	
	data_copy->GetXaxis()->SetTitle(xlabel);
	data_copy->GetYaxis()->SetTitle(ylabel);

	return;
}
