#include "kinematic_cuts.h"

void thetanq(TString inDat, TString inBac, TString inSim){

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
	inTreeBac->SetWeight( Normmix / bacnorm->X() );

	// Define histograms we want to plot:
	TH1D ** thetanq_dat = new TH1D*[3];
	TH1D ** thetanq_bac = new TH1D*[3];
	TH1D ** thetanq_sim = new TH1D*[3];
	for(int i = 0 ; i < 3 ; i++){
		thetanq_dat[i] = new TH1D(Form("thetanq_dat_%i",i),"",40,140,180);
		thetanq_bac[i] = new TH1D(Form("thetanq_bac_%i",i),"",40,140,180);
		thetanq_sim[i] = new TH1D(Form("thetanq_sim_%i",i),"",40,140,180);
	}

	//Adding the edep cuts here
	TCut edep_cut = Form("nHits[nleadindex]->getEdep() > %f", NCUT_Edep * DataAdcToMeVee);
	
	// Draw the full thetanq distribution
	TCanvas * c_thetanq = new TCanvas("c_thetanq","",800,600);
	double sim_scaling = 0;
	c_thetanq->Divide(3,2);
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

		c_thetanq->cd(i+1);
		inTreeDat->Draw(Form("tag[nleadindex]->getThetaNQ()*180./TMath::Pi() >> thetanq_dat_%i",i),"tag[nleadindex]->getMomentumN().Mag() > 0.3" && pTcut && edep_cut);
		inTreeBac->Draw(Form("tag[nleadindex]->getThetaNQ()*180./TMath::Pi() >> thetanq_bac_%i",i),"tag[nleadindex]->getMomentumN().Mag() > 0.3" && pTcut && edep_cut);
		inTreeSim->Draw(Form("tag[nleadindex]->getThetaNQ()*180./TMath::Pi() >> thetanq_sim_%i",i),"tag[nleadindex]->getMomentumN().Mag() > 0.3" && pTcut && edep_cut);

		// Background subraction
		thetanq_dat[i]->Add(thetanq_bac[i],-1);

		// Simulation scaling only from no pT cut distribution (i.e. from full distribution)
		double full_simnorm = (double)thetanq_dat[0]->Integral() / thetanq_sim[0]->Integral();
		if( i == 0 ) sim_scaling = full_simnorm;
		thetanq_sim[i]->Scale( sim_scaling );
		
		
		thetanq_sim[i]->SetTitle(pTtitle+Form(", C_{sim} = %f, ",sim_scaling));
		label1D(thetanq_dat[i],thetanq_sim[i],"Theta_{nq} [Deg.]","Counts");

		c_thetanq->cd(4+i);
		label1D_ratio(thetanq_dat[i],thetanq_sim[i],"Theta_{nq} [Deg.]","Data/Sim",0,2);
	}


	c_thetanq->SaveAs("full_thetanq.pdf");

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
