void must_plot(TString inFileDatTagName, TString inFileBacTagName,
	       	 TString inFileSimTagName, TString inFileDatIncName, 
		 TString inFileSimIncName ){
        gSystem->Load("libclashit_tree.so");
        gSystem->Load("libtaghit_tree.so");
        gSystem->Load("libbandhit_tree.so");
        gSystem->Load("libgenpart_tree.so");
	// Define some function used
	void label1D(TH1D* data, TH1D* sim, TString xlabel, TString ylabel);
	void label1D_ratio(TH1D* data, TH1D* sim, TString xlabel, TString ylabel);



	// Load TFiles
	TFile * inFileDatTag = new TFile(inFileDatTagName);
	TFile * inFileBacTag = new TFile(inFileBacTagName);
	TFile * inFileSimTag = new TFile(inFileSimTagName);
	TFile * inFileDatInc = new TFile(inFileDatIncName);
	TFile * inFileSimInc = new TFile(inFileSimIncName);

	// Get the TTrees
	TTree * inTreeDatTag = (TTree*) inFileDatTag->Get("tagged");
	TTree * inTreeBacTag = (TTree*) inFileBacTag->Get("tagged");
	TTree * inTreeSimTag = (TTree*) inFileSimTag->Get("tagged");
	TTree * inTreeDatInc = (TTree*) inFileDatInc->Get("electrons");
	TTree * inTreeSimInc = (TTree*) inFileSimInc->Get("electrons");

	// Get and set the background normalization for tagged
	TVector3 * datnorm = (TVector3*)inFileDatTag->Get("bacnorm");
	TVector3 * bacnorm = (TVector3*)inFileBacTag->Get("bacnorm");
	inTreeBacTag->SetWeight( datnorm->X() / bacnorm->X() );


	// Get simulation normalization
	double 		L_tag = 2.28e10 / (130.1);
	double 		L_inc = 9.61e6 / (130.1);
	double 		Q_inc = 535885;
	double		Q_tag = 3.72e+07;

	// Define the histograms for the ratio:
	const int nAs_bins = 4;
	const double As_min = 1.2;
	const double As_max = 1.6;
	const int nXp_bins = 6;
	const double Xp_min = 0.1;
	const double Xp_max = 0.7;
	const int nXb_bins = 6;
	const double Xb_min = 0.1;
	const double Xb_max = 0.7;
	// What we want:
	//			[ N_tag,data (Q2,theta_nq,x';alphaS) / Q_tag] / [ N_tag,sim(Q2,theta_nq,x';alphaS) / L_tag,sim ]
	//	R(tag/inc) = 	------------------------------------------------------------------------------------------------
	//				     [ N_inc,data (Q2, x=x') / Q_inc] / [ N_inc,sim (Q2, x=x') / L_inc,sim ]
	//
	//			[ N_tag,data / Q_tag ]	 [ N_inc,sim / L_inc,sim ]
	//	R(tag/inc) = 	---------------------- * -------------------------
	//			[ N_inc,data / Q_inc ]   [ N_tag,sim / L_tag,sim ]
	//
	//			 [ N_tag,data / Q_tag ]	     [ N_inc,sim / L_inc,sim ]
	//	R(tag/inc) = 	------------------------   * -------------------------
	//			[ N_tag,sim / L_tag,sim ]      [ N_inc,dat / Q_inc ]
	//
	// These histograms all share a Q2 bin of [2,10]
	// The tag histograms have cos_theta_nq bin of [-1,-0.8]
	
	TH1D ** h1_as_xp_dat = new TH1D*[nXp_bins]; // this is an alphaS histogram in a bin of X' for dat
	TH1D ** h1_as_xp_bac = new TH1D*[nXp_bins]; // this is an alphaS histogram in a bin of X' for dat
	TH1D ** h1_as_xp_sim = new TH1D*[nXp_bins]; // this is an alphaS histogram in a bin of X' for sim

       	TH1D ** h1_as_xp_lpt_dat = new TH1D*[nXp_bins]; // this is an alphaS histogram in a bin of X' for dat
	TH1D ** h1_as_xp_lpt_bac = new TH1D*[nXp_bins]; // this is an alphaS histogram in a bin of X' for dat
	TH1D ** h1_as_xp_lpt_sim = new TH1D*[nXp_bins]; // this is an alphaS histogram in a bin of X' for sim

       	TH1D ** h1_as_xp_hpt_dat = new TH1D*[nXp_bins]; // this is an alphaS histogram in a bin of X' for dat
	TH1D ** h1_as_xp_hpt_bac = new TH1D*[nXp_bins]; // this is an alphaS histogram in a bin of X' for dat
	TH1D ** h1_as_xp_hpt_sim = new TH1D*[nXp_bins]; // this is an alphaS histogram in a bin of X' for sim

	TH1D ** h1_as_xp_lpt_03 = new TH1D*[nXp_bins];
	TH1D ** h1_as_xp_hpt_03 = new TH1D*[nXp_bins];
	TH1D ** h1_as_xp_03 = new TH1D*[nXp_bins];
	
	for( int bin = 0 ; bin < nXp_bins ; bin++ ){
		h1_as_xp_dat[bin]	= new TH1D(Form("h1_as_xp_dat_%i",bin),"",nAs_bins,As_min,As_max);
		h1_as_xp_bac[bin]	= new TH1D(Form("h1_as_xp_bac_%i",bin),"",nAs_bins,As_min,As_max);
		h1_as_xp_sim[bin]	= new TH1D(Form("h1_as_xp_sim_%i",bin),"",nAs_bins,As_min,As_max);

		h1_as_xp_lpt_dat[bin]	= new TH1D(Form("h1_as_xp_lpt_dat_%i",bin),"",nAs_bins,As_min,As_max);
		h1_as_xp_lpt_bac[bin]	= new TH1D(Form("h1_as_xp_lpt_bac_%i",bin),"",nAs_bins,As_min,As_max);
		h1_as_xp_lpt_sim[bin]	= new TH1D(Form("h1_as_xp_lpt_sim_%i",bin),"",nAs_bins,As_min,As_max);

		h1_as_xp_hpt_dat[bin]	= new TH1D(Form("h1_as_xp_hpt_dat_%i",bin),"",nAs_bins,As_min,As_max);
		h1_as_xp_hpt_bac[bin]	= new TH1D(Form("h1_as_xp_hpt_bac_%i",bin),"",nAs_bins,As_min,As_max);
		h1_as_xp_hpt_sim[bin]	= new TH1D(Form("h1_as_xp_hpt_sim_%i",bin),"",nAs_bins,As_min,As_max);

	      	h1_as_xp_hpt_03[bin]	= new TH1D(Form("h1_as_xp_hpt_03_%i",bin),"",nAs_bins,As_min,As_max);
        	h1_as_xp_lpt_03[bin]	= new TH1D(Form("h1_as_xp_lpt_03_%i",bin),"",nAs_bins,As_min,As_max);
		h1_as_xp_03[bin]	= new TH1D(Form("h1_as_xp_03_%i",bin),"",nAs_bins,As_min,As_max);
		
	}
	TH1D ** h1_q2_xb_dat = new TH1D*[10];
	TH1D ** h1_q2_xb_sim = new TH1D*[10];
	for( int bin = 0 ; bin < nXb_bins ; bin++ ){
		h1_q2_xb_dat[bin]	= new TH1D(Form("h1_q2_xb_dat_%i",bin),"",10,0,10);
		h1_q2_xb_sim[bin]	= new TH1D(Form("h1_q2_xb_sim_%i",bin),"",10,0,10);
	}

	// Draw alphaS distribution
	TH1D * as_dat = new TH1D("as_dat","",nAs_bins,As_min,As_max);
	TH1D * as_bac = new TH1D("as_bac","",nAs_bins,As_min,As_max);
	TH1D * as_sim = new TH1D("as_sim","",nAs_bins,As_min,As_max);

	TCanvas * c_as = new TCanvas("c_as","",800,600);
	c_as->cd();

	inTreeDatTag->Draw("tag[nleadindex]->getAs() >> as_dat","tag[nleadindex]->getMomentumN().Mag() > 0.3");
	inTreeBacTag->Draw("tag[nleadindex]->getAs() >> as_bac","tag[nleadindex]->getMomentumN().Mag() > 0.3");
	inTreeSimTag->Draw("tag[nleadindex]->getAs() >> as_sim","tag[nleadindex]->getMomentumN().Mag() > 0.3");

	as_dat->Add(as_bac,-1);
	double full_simnorm = (double)as_dat->Integral() / as_sim->Integral();
	as_sim->Scale( full_simnorm );
	
	as_dat->SetTitle(Form("Full distribution, C_{sim} = %f",full_simnorm));
	label1D(as_dat,as_sim,"Alpha_{S}","Counts");
	c_as->SaveAs("full_as.pdf");


	// First let's get:
	//	R(tag) = 	N_tag,data (Q2,theta_nq,x';alphaS) / [ N_tag,sim(Q2,theta_nq,x';alphaS) ]
	//	as just individual histograms for numerator and denominator
	TCanvas * c1_as_xp_tag = new TCanvas("c1_as_xp_tag","",800,600);
	TCanvas * c1_as_xp_lpt_tag = new TCanvas("c1_as_xp_lpt_tag","",800,600);
	TCanvas * c1_as_xp_hpt_tag = new TCanvas("c1_as_xp_hpt_tag","",800,600);
	
	
	c1_as_xp_tag->Divide(3,2);
	c1_as_xp_lpt_tag->Divide(3,2);
	c1_as_xp_hpt_tag->Divide(3,2);

       	TCut lpt = "tag[nleadindex]->getPt().Mag() <  0.1 ";
      	TCut hpt = "tag[nleadindex]->getPt().Mag() >= 0.1 ";
	
	for( int bin = 0 ; bin < nXp_bins ; bin++ ){
		double this_min_xp = Xp_min+0.05 + bin*(Xp_max - Xp_min)/nXp_bins;
		double this_max_xp = Xp_min+0.05 + (bin+1)*(Xp_max - Xp_min)/nXp_bins;
		TCut this_xp_cut = Form("tag[nleadindex]->getXp() > %f && tag[nleadindex]->getXp() < %f",this_min_xp,this_max_xp);

		inTreeDatTag->Draw(Form("tag[nleadindex]->getAs() >> h1_as_xp_dat_%i",bin),this_xp_cut && "tag[nleadindex]->getMomentumN().Mag() > 0.3", "goff");
		inTreeBacTag->Draw(Form("tag[nleadindex]->getAs() >> h1_as_xp_bac_%i",bin),this_xp_cut && "tag[nleadindex]->getMomentumN().Mag() > 0.3", "goff");
		inTreeSimTag->Draw(Form("tag[nleadindex]->getAs() >> h1_as_xp_sim_%i",bin),this_xp_cut && "tag[nleadindex]->getMomentumN().Mag() > 0.3", "goff");

	       	inTreeDatTag->Draw(Form("tag[nleadindex]->getAs() >> h1_as_xp_lpt_dat_%i",bin),this_xp_cut && "tag[nleadindex]->getMomentumN().Mag() > 0.3" && lpt, "goff");
		inTreeBacTag->Draw(Form("tag[nleadindex]->getAs() >> h1_as_xp_lpt_bac_%i",bin),this_xp_cut && "tag[nleadindex]->getMomentumN().Mag() > 0.3" && lpt, "goff");
		inTreeSimTag->Draw(Form("tag[nleadindex]->getAs() >> h1_as_xp_lpt_sim_%i",bin),this_xp_cut && "tag[nleadindex]->getMomentumN().Mag() > 0.3" && lpt, "goff");

		inTreeDatTag->Draw(Form("tag[nleadindex]->getAs() >> h1_as_xp_hpt_dat_%i",bin),this_xp_cut && "tag[nleadindex]->getMomentumN().Mag() > 0.3" && hpt, "goff");
		inTreeBacTag->Draw(Form("tag[nleadindex]->getAs() >> h1_as_xp_hpt_bac_%i",bin),this_xp_cut && "tag[nleadindex]->getMomentumN().Mag() > 0.3" && hpt, "goff");
		inTreeSimTag->Draw(Form("tag[nleadindex]->getAs() >> h1_as_xp_hpt_sim_%i",bin),this_xp_cut && "tag[nleadindex]->getMomentumN().Mag() > 0.3" && hpt, "goff");
                
		// Do the background subtraction:
		h1_as_xp_dat[bin]->Add(h1_as_xp_bac[bin],-1);
	       	h1_as_xp_lpt_dat[bin]->Add(h1_as_xp_lpt_bac[bin],-1);
	       	h1_as_xp_hpt_dat[bin]->Add(h1_as_xp_hpt_bac[bin],-1);

		// Do the normalization for simulation and data:
		h1_as_xp_dat[bin]->Scale(1./Q_tag);
		h1_as_xp_sim[bin]->Scale(1./L_tag);

	       	h1_as_xp_lpt_dat[bin]->Scale(1./Q_tag);
		h1_as_xp_lpt_sim[bin]->Scale(1./L_tag);

		h1_as_xp_hpt_dat[bin]->Scale(1./Q_tag);
		h1_as_xp_hpt_sim[bin]->Scale(1./L_tag);
		

		// Plot data and simulation full pt:
	       	c1_as_xp_tag->cd(bin+1);
		double new_simnorm = h1_as_xp_dat[bin]->Integral() / h1_as_xp_sim[bin]->Integral();
		TString current_title = Form("%f < x' < %f",this_min_xp,this_max_xp);
		h1_as_xp_dat[bin]->SetTitle( current_title + Form(", C_{new} = %f",new_simnorm)  );
       	        label1D(h1_as_xp_dat[bin],h1_as_xp_sim[bin],"Alpha_{S}","Counts");
		c1_as_xp_tag->Update();
		
	

                // Plot data and simulation lpt:
	       	c1_as_xp_lpt_tag->cd(bin+1);
		double new_simnorm_lpt = h1_as_xp_lpt_dat[bin]->Integral() / h1_as_xp_lpt_sim[bin]->Integral();
	       	TString current_title_lpt = Form("%f < x' < %f, pt<100",this_min_xp,this_max_xp);
		h1_as_xp_lpt_dat[bin]->SetTitle( current_title_lpt + Form(", C_{new} = %f",new_simnorm_lpt)  );
	       	label1D(h1_as_xp_lpt_dat[bin],h1_as_xp_lpt_sim[bin],"Alpha_{S}","Counts");
		c1_as_xp_lpt_tag->Update();
		//
                // Plot data and simulation hpt:
	       	c1_as_xp_hpt_tag->cd(bin+1);
		double new_simnorm_hpt = h1_as_xp_hpt_dat[bin]->Integral() / h1_as_xp_hpt_sim[bin]->Integral();
	       	TString current_title_hpt = Form("%f < x' < %f, pt>100",this_min_xp,this_max_xp);
		h1_as_xp_hpt_dat[bin]->SetTitle( current_title_hpt + Form(", C_{new} = %f",new_simnorm_hpt)  );
	       	label1D(h1_as_xp_hpt_dat[bin],h1_as_xp_hpt_sim[bin],"Alpha_{S}","Counts");
	
		c1_as_xp_hpt_tag->Update();
				
	}
	c1_as_xp_tag->SaveAs("as_xpBins_tag.pdf");
	c1_as_xp_lpt_tag->SaveAs("as_xpBins_lpt_tag.pdf");
	c1_as_xp_hpt_tag->SaveAs("as_xpBins_hpt_tag.pdf");


	

	// Now we can go get:
	//	R(inc) = 	N_inc,data (Q2,x=x') / N_inc,sim(Q2,x=x') 
	//	by saving each number individiually
	TCanvas * c1_q2_xb_inc = new TCanvas("c1_q2_xb_inc","",800,600);
	c1_q2_xb_inc->Divide(3,2);
	double DatIncCounts[nXb_bins] = {0};
	double SimIncCounts[nXb_bins] = {0};
	for( int bin = 0 ; bin < nXb_bins ; bin++ ){
		c1_q2_xb_inc->cd(bin+1);
		double this_min_xb = Xb_min+0.05 + bin*(Xb_max - Xb_min)/nXb_bins;
		double this_max_xb = Xb_min+0.05 + (bin+1)*(Xb_max - Xb_min)/nXb_bins;
		TCut this_xb_cut = Form("eHit->getXb() > %f && eHit->getXb() < %f",this_min_xb,this_max_xb);

		inTreeDatInc->Draw(Form("eHit->getQ2() >> h1_q2_xb_dat_%i",bin),this_xb_cut);
		inTreeSimInc->Draw(Form("eHit->getQ2() >> h1_q2_xb_sim_%i",bin),this_xb_cut);

		// Do the normalization for simulation and data:
		h1_q2_xb_dat[bin]->Scale(1./Q_inc);
		h1_q2_xb_sim[bin]->Scale(1./L_inc);

		DatIncCounts[bin] = h1_q2_xb_dat[bin]->Integral();
		SimIncCounts[bin] = h1_q2_xb_sim[bin]->Integral();


		// Plot data and simulation:
		h1_q2_xb_dat[bin]->SetTitle(Form("%f < x_{B} < %f",this_min_xb,this_max_xb));
		label1D(h1_q2_xb_dat[bin],h1_q2_xb_sim[bin],"Q^{2}","Counts");


	}
	c1_q2_xb_inc->SaveAs("q2_xbBins_inc.pdf");


	// Now do the R_tag/R_inc ratio
	TCanvas * c1_must = new TCanvas("c1_must","",800,600);
	TCanvas * c1_must_lpt = new TCanvas("c1_must_lpt","",800,600);
	TCanvas * c1_must_hpt = new TCanvas("c1_must_hpt","",800,600);

	TCanvas * c1_must_overlaid = new TCanvas("c1_must_overlaid","",800,600);
	
	c1_must->Divide(3,2);
	c1_must_lpt->Divide(3,2);
	c1_must_hpt->Divide(3,2);
	c1_must_overlaid->Divide(3,2);
   
	
	for( int bin = 0 ; bin < nXp_bins ; bin++ ){
		
		//h1_as_xp_dat[bin] -> Scale( 1./Q_tag );  // N_data,tag
		//h1_as_xp_sim[bin] -> Scale( 1./L_tag );	 // N_sim,tag

		double N_inc_dat = (DatIncCounts[bin]);
		double N_inc_sim = (SimIncCounts[bin]);

		//Plots for full Pt
		c1_must->cd(bin+1);
		h1_as_xp_dat[bin]->Scale( N_inc_sim / N_inc_dat );	// N_dat,tag / N_dat,inc
		label1D_ratio(h1_as_xp_dat[bin],h1_as_xp_sim[bin],"Alpha_{S}","R=R_{tag}/R_{inc}");

				
	       	//Plots for lpt
		c1_must_lpt->cd(bin+1);
		h1_as_xp_lpt_dat[bin]->Scale( N_inc_sim / N_inc_dat );	// N_dat,tag / N_dat,inc
		label1D_ratio(h1_as_xp_lpt_dat[bin],h1_as_xp_lpt_sim[bin],"Alpha_{S}","R=R_{tag}/R_{inc}");

		
	       	//Plots for hpt
		c1_must_hpt->cd(bin+1);
		h1_as_xp_hpt_dat[bin]->Scale( N_inc_sim / N_inc_dat );	// N_dat,tag / N_dat,inc
		label1D_ratio(h1_as_xp_hpt_dat[bin],h1_as_xp_hpt_sim[bin],"Alpha_{S}","R=R_{tag}/R_{inc}");
				
	}
	c1_must->SaveAs("ratio_tag_inc.pdf");
       	c1_must_lpt->SaveAs("ratio_lpt_tag_inc.pdf");
       	c1_must_hpt->SaveAs("ratio_hpt_tag_inc.pdf");


	
	TCanvas * c1_must_xp03 = new TCanvas("c1_must_xp03","",800,600);
	TCanvas * c1_must_xp03_lpt = new TCanvas("c1_must_xp03_lpt","",800,600);
	TCanvas * c1_must_xp03_hpt = new TCanvas("c1_must_xp03_hpt","",800,600);
	TCanvas * c1_must_xp03_overlaid = new TCanvas("c1_must_xp03_overlaid","",800,600);
	
	c1_must_xp03->Divide(3,2);
	c1_must_xp03_lpt->Divide(3,2);
	c1_must_xp03_hpt->Divide(3,2);

	c1_must_xp03_overlaid->Divide(3,2);
	

	for( int bin = 0 ; bin < nXp_bins ; bin++ ){
		c1_must_xp03->cd(bin+1);
		h1_as_xp_dat[bin]->Divide( h1_as_xp_sim[bin] );
		label1D_ratio(h1_as_xp_dat[bin],h1_as_xp_dat[1],"Alpha_{S}","R=R_{tag}/R_{inc} / R_(x'=0.3)");
		
        	c1_must_xp03_lpt->cd(bin+1);
		h1_as_xp_lpt_dat[bin]->Divide( h1_as_xp_lpt_sim[bin] );
		label1D_ratio(h1_as_xp_lpt_dat[bin],h1_as_xp_lpt_dat[1],"Alpha_{S}","R=R_{tag}/R_{inc} / R_(x'=0.3)");

 
		
	       	c1_must_xp03_hpt->cd(bin+1);
		h1_as_xp_hpt_dat[bin]->Divide( h1_as_xp_hpt_sim[bin] );
		label1D_ratio(h1_as_xp_hpt_dat[bin],h1_as_xp_hpt_dat[1],"Alpha_{S}","R=R_{tag}/R_{inc} / R_(x'=0.3)");
				

		//overlaid plot for lpt and hpt
		c1_must_xp03_overlaid->cd(bin+1);
		
         	h1_as_xp_03[bin]->Divide(h1_as_xp_dat[bin], h1_as_xp_dat[1]);
		
		h1_as_xp_lpt_03[bin]->Divide(h1_as_xp_lpt_dat[bin], h1_as_xp_lpt_dat[1]);
	       	h1_as_xp_hpt_03[bin]->Divide(h1_as_xp_hpt_dat[bin], h1_as_xp_hpt_dat[1]);
		
	       	h1_as_xp_lpt_03[bin]->Draw("ep");
		h1_as_xp_lpt_03[bin]->GetYaxis()->SetRangeUser(0.7,3);
		h1_as_xp_lpt_03[bin]->GetXaxis()->SetTitle("Alpha_{S}");
		h1_as_xp_lpt_03[bin]->GetYaxis()->SetTitle("R=R_{tag}/R_{inc} / R_(x'=0.3)");
                h1_as_xp_lpt_03[bin]->SetLineColor(1);

	       	h1_as_xp_hpt_03[bin]->Draw("ep same");
	        h1_as_xp_hpt_03[bin]->SetLineColor(2);
		

	}
	c1_must_xp03->SaveAs("ratio_tag_inc_xp03.pdf");
	c1_must_xp03_lpt->SaveAs("ratio_tag_inc_xp03_lpt.pdf");
	c1_must_xp03_hpt->SaveAs("ratio_tag_inc_xp03_hpt.pdf");
	c1_must_xp03_overlaid->SaveAs("ratio_tag_inc_xp03_overlaid.pdf");

	//Doing the inverted plots xp bin for a given bin in as
	double as_xp= 0., as_xp_lpt =0, as_xp_hpt;
	double as_xp_err=0, as_xp_lpt_err =0, as_xp_hpt_err =0;;

	double xp_bins_val[6] = {0.2, 0.3, 0.4, 0.5, 0.6, 0.7};
	double as_bins_val[5] = {1.2, 1.3, 1.4, 1.5, 1.6};

	cout << "===Doing inverted plots ======="<< endl;

	TGraphErrors *gr_as[nAs_bins];
	TGraphErrors *gr_as_lpt[nAs_bins];
	TGraphErrors *gr_as_hpt[nAs_bins];
	
	TCanvas *c1_must_invert = new TCanvas("c1_must_invert", "", 900, 500);
	c1_must_invert->Divide(3,1);

      	TCanvas *c1_must_invert_lpt = new TCanvas("c1_must_invert_lpt", "", 900, 500);
	c1_must_invert_lpt->Divide(3,1);
	
	TCanvas *c1_must_invert_hpt = new TCanvas("c1_must_invert_hpt", "", 900, 500);
	c1_must_invert_hpt->Divide(3,1);
	
	
        for (int as =1; as < nAs_bins; as++){

	  gr_as[as] = new TGraphErrors();
	  gr_as_lpt[as] = new TGraphErrors();
	  gr_as_hpt[as] = new TGraphErrors();

	  double as_low = as_bins_val[as];
	  double as_high =as_bins_val[as+1];

	  TString tit = Form("%2.1f< Alpha_{s}<%2.1f", as_low, as_high);
	  TString tit_lpt = Form("%2.1f< Alpha_{s}<%2.1f, pt <100", as_low, as_high);
	  TString tit_hpt = Form("%2.1f< Alpha_{s}<%2.1f, pt>100", as_low, as_high);

	  int index =0;
	  int index_lpt =0;
	  int index_hpt =0;

	  for (int xp =1; xp< nXp_bins; xp++){
	   
	    as_xp = h1_as_xp_03[xp]->GetBinContent(as+1);
	    as_xp_err = h1_as_xp_03[xp]->GetBinError(as+1);

	    as_xp_lpt = h1_as_xp_lpt_03[xp]->GetBinContent(as+1);
	    as_xp_lpt_err = h1_as_xp_lpt_03[xp]->GetBinError(as+1);

	    as_xp_hpt = h1_as_xp_hpt_03[xp]->GetBinContent(as+1);
	    as_xp_hpt_err = h1_as_xp_hpt_03[xp]->GetBinError(as+1);

	    if (as_xp >0){
	      gr_as[as]->SetPoint(index, xp_bins_val[xp], as_xp);
	      gr_as[as]->SetPointError(index, 0, as_xp_err);
	      index ++;
	    }

	    if (as_xp_lpt >0){
	      gr_as_lpt[as]->SetPoint(index_lpt, xp_bins_val[xp], as_xp_lpt);
	      gr_as_lpt[as]->SetPointError(index_lpt, 0, as_xp_lpt_err);
	      index_lpt ++;
	    }

	    if (as_xp_hpt >0){
	      gr_as_hpt[as]->SetPoint(index_hpt, xp_bins_val[xp], as_xp_hpt);
	      gr_as_hpt[as]->SetPointError(index_hpt, 0, as_xp_hpt_err);
	      index_hpt ++;
	    }



	  }

	  c1_must_invert->cd(as);
	  gr_as[as]->Draw("AP");
	  gr_as[as]->SetTitle(tit);
	  gr_as[as]->GetXaxis()->SetTitle("x^{'}");
	  gr_as[as]->GetYaxis()->SetTitle("R=R_{tag}/R_{inc} / R_(x'=0.3)");
	  gr_as[as]->SetMaximum(3);
	  gr_as[as]->SetMinimum(0.5);
	  gr_as[as]->SetMarkerStyle(20);

	  c1_must_invert_lpt->cd(as);
	  gr_as_lpt[as]->Draw("AP");
	  gr_as_lpt[as]->GetXaxis()->SetTitle("x^{'}");
	  gr_as_lpt[as]->GetYaxis()->SetTitle("R=R_{tag}/R_{inc} / R_(x'=0.3)");
	  gr_as_lpt[as]->SetTitle(tit_lpt);
	  gr_as_lpt[as]->SetMaximum(3);
	  gr_as_lpt[as]->SetMinimum(0.5);
	  gr_as_lpt[as]->SetMarkerStyle(20);


	  c1_must_invert_hpt->cd(as);
	  gr_as_hpt[as]->Draw("AP");
	  gr_as_hpt[as]->GetXaxis()->SetTitle("x^{'}");
	  gr_as_hpt[as]->GetYaxis()->SetTitle("R=R_{tag}/R_{inc} / R_(x'=0.3)");
	  gr_as_hpt[as]->SetTitle(tit_lpt);
	  gr_as_hpt[as]->SetMaximum(3);
	  gr_as_hpt[as]->SetMinimum(0.5);
	  gr_as_hpt[as]->SetMarkerStyle(20);
	  
	  

	}

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

	data->Draw("p");
	sim->Draw("hist,same");

	double max1 = data->GetMaximum()*1.1;
	double max2 = sim->GetMaximum()*1.1;
	data->GetYaxis()->SetRangeUser(0,max(max1,max2));
	
	data->GetXaxis()->SetTitle(xlabel);
	data->GetYaxis()->SetTitle(ylabel);

	TLegend * legend = new TLegend(0.7,0.8,0.9,0.9);
	//legend->AddEntry(data,"Radiation On","f");
	//legend->AddEntry(sim,"Radiation Off","f");
	legend->AddEntry(data,"Data","f");
	legend->AddEntry(sim,"Sim","f");
	legend->Draw();

	return;
}

void label1D_ratio(TH1D* data, TH1D* sim, TString xlabel, TString ylabel){
	TH1D * data_copy = (TH1D*) data->Clone();
	TH1D * sim_copy = (TH1D*) sim->Clone();
	
	data_copy->SetLineColor(1);
	data_copy->SetLineWidth(3);
	data_copy->SetStats(0);

	sim_copy->SetLineColor(9);
	sim_copy->SetLineWidth(3);
	sim_copy->SetStats(0);
	//sim_copy->Scale(data_copy->Integral() / sim_copy->Integral() );

	data_copy->Divide(sim_copy);


	//	for( int bin = 1 ; bin < data_copy->GetXaxis()->GetNbins()+1 ; bin++){
	//	cout << data_copy->GetXaxis()->GetBinCenter(bin) << " " << data_copy->GetBinContent(bin) << "\n";
	//	}

	data_copy->Draw("ep");
	TLine* line = new TLine(data_copy->GetXaxis()->GetBinCenter(1), 1., data_copy->GetXaxis()->GetBinCenter(data_copy->GetXaxis()->GetNbins()), 1.);
	line->SetLineWidth(3);
	line->SetLineColor(2);
	line->Draw("same");

	double max1 = data_copy->GetMaximum()*1.1;
	data_copy->GetYaxis()->SetRangeUser(0.7,3);
	
	data_copy->GetXaxis()->SetTitle(xlabel);
	data_copy->GetYaxis()->SetTitle(ylabel);

	return;
}
