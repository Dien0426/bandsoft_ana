// I just want to write a simple code
// access to inc_data_skimed file
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "TApplication.h"
#include "TCanvas.h"
#include "TClonesArray.h"
#include "TFile.h"
#include "TFitResult.h"
#include "TFitResultPtr.h"
#include "TH1.h"
#include "TH2.h"
#include "TRint.h"
#include "TStyle.h"
#include "TTree.h"
#include "TF1.h"

#include "bandhit.h"
#include "clas12fiducial.h"
#include "clashit.h"
#include "constants.h"
#include "taghit.h"



using namespace std;

int main(int argc, char **argv) {

  // These lines of code to make sure we can see the plots

#ifdef WITHRINT
  // TRint *myapp = new TRint("RootSession", &argc, argv, NULL, 0);
  TRint *myapp = new TRint("RootSession", 0, 0, NULL, 0);
#else
  TApplication *myapp = new TApplication("myapp", 0, 0);
#endif

  cout << argv[0] << "  " << argv[1] << endl;
  // Star the program

  if (argc < 3) {
    cerr
        << "Wrong number of arguments. Instead use\n"
        << "\t./code [outfile.root] [apply fiducial (1, 0)] [inputDatafiles]\n";
    return -1;
  }

  // output rootfile
  TFile *outFile = new TFile(argv[1], "RECREATE");

  cout << "test oufile: " << outFile->GetName() << endl;
  // Define histogram to look at e-fiducial cut

  TH2D *h2_e_th_ph = new TH2D("h2_e_th_ph", "", 400, -200, 200, 200, 0, 50);
  TH2D *h2_DC1_xy = new TH2D("h2_DC1_xy", "DC: ele Region1", 300, -300, 300,300, -300, 300);
  TH2D *h2_DC2_xy = new TH2D("h2_DC2_xy", "DC: ele Region2", 300, -300, 300,300, -300, 300);
  TH2D *h2_DC3_xy = new TH2D("h2_DC3_xy", "DC: ele Region3", 300, -300, 300,300, -300, 300);

  TH2D *h2_DC1_xy_acc = new TH2D("h2_DC1_xy_acc", "DC: ele Region1 acc", 300, -300, 300,300, -300, 300);
  TH2D *h2_DC2_xy_acc = new TH2D("h2_DC2_xy_acc", "DC: ele Region2 acc", 300, -300, 300,300, -300, 300);
  TH2D *h2_DC3_xy_acc = new TH2D("h2_DC3_xy_acc", "DC: ele Region3 acc", 300, -300, 300,300, -300, 300);
  

  TH2D *h2_DC1_test = new TH2D("h2_DC1_test", "DC: ele Region1", 300, -300, 300,300, -300, 300);

  //Histogram for each each section in each layer
  int Nsec[6] = {1, 2, 3, 4, 5, 6};
  int Nreg[3] = {6, 18, 36};

  TH2D *h2_DC_xy1_S[6];
  TH2D *h2_DC_xy2_S[6];
  TH2D *h2_DC_xy3_S[6];
 
    for (int s =1; s<7; s++){
      h2_DC_xy1_S[s-1] = new TH2D(Form("h2_DC_xy1_S_%d",s), Form("Region 1, sector %d",s),150, 0, 150, 300, -150, 150);
      h2_DC_xy2_S[s-1] = new TH2D(Form("h2_DC_xy2_S_%d",s), Form("Region 2, sector %d",s),250, 0, 250, 300, -150, 150);
      h2_DC_xy3_S[s-1] = new TH2D(Form("h2_DC_xy3_S_%d",s), Form("Region 3, sector %d",s),300, 0, 300, 300, -150, 150);  
    }
    

  // Def the fiducial cut
  int doFiducial = atoi(argv[2]);
  clas12fiducial *fid = new clas12fiducial();


  // DC fiducial cuts
  // quick work for now
 
  const double maxparams_in[6][6][3][2] = {
{{{-14.563,0.60032},{-19.6768,0.58729},{-22.2531,0.544896}},{{-12.7486,0.587631},{-18.8093,0.571584},{-19.077,0.519895}},
{{-11.3481,0.536385},{-18.8912,0.58099},{-18.8584,0.515956}},{{-10.7248,0.52678},{-18.2058,0.559429},{-22.0058,0.53808}},
{{-16.9644,0.688637},{-17.1012,0.543961},{-21.3974,0.495489}},{{-13.4454,0.594051},{-19.4173,0.58875},{-22.8771,0.558029}}},
{{{-6.2928,0.541828},{-16.7759,0.57962},{-32.5232,0.599023}},{{-6.3996,0.543619},{-16.7429,0.578472},{-32.5408,0.600826}},
{{-5.49712,0.53463},{-16.1294,0.576928},{-32.5171,0.597735}},{{-6.4374,0.54839},{-16.9511,0.582143},{-33.0501,0.59995}},
{{-5.30128,0.529377},{-16.1229,0.579019},{-30.7768,0.593861}},{{-5.89201,0.541124},{-16.1245,0.575001},{-32.2617,0.601506}}},
{{{-6.3618,0.546384},{-17.0277,0.582344},{-34.9276,0.612875}},{{-6.36432,0.546268},{-15.8404,0.574102},{-33.0627,0.599142}},
{{-6.34357,0.548411},{-16.0496,0.575913},{-34.8535,0.610211}},{{-5.8568,0.541784},{-16.1124,0.576473},{-32.8547,0.599033}},
{{-5.91941,0.536801},{-15.726,0.575211},{-34.0964,0.606777}},{{-5.55498,0.536609},{-15.9853,0.579705},{-33.4886,0.606439}}},
{{{-12.594,0.613062},{-18.4504,0.588136},{-16.3157,0.529461}},{{-12.3417,0.61231},{-18.1498,0.590748},{-13.8106,0.52335}},
{{-12.1761,0.609307},{-15.919,0.572156},{-13.0598,0.5194}},{{-12.5467,0.612645},{-16.2129,0.572974},{-12.8611,0.51252}},
{{-13.0976,0.615928},{-16.9233,0.580972},{-13.0906,0.519738}},{{-12.884,0.622133},{-17.2566,0.585572},{-12.1874,0.510124}}},
{{{-6.51157,0.545763},{-16.4246,0.583603},{-32.2001,0.60425}},{{-6.21169,0.541872},{-16.8484,0.591172},{-31.7785,0.606234}},
{{-5.89452,0.54464},{-16.612,0.591506},{-29.9143,0.589656}},{{-6.68908,0.553374},{-16.2993,0.585165},{-30.252,0.59519}},
{{-6.17185,0.540496},{-16.7197,0.591664},{-31.619,0.608306}},{{-5.7526,0.541761},{-16.2054,0.587326},{-31.3653,0.604081}}},
{{{-11.8798,0.62389},{-20.2212,0.610786},{-16.4137,0.51337}},{{-12.0817,0.631621},{-20.7511,0.610844},{-16.9407,0.522958}},
{{-9.72746,0.605471},{-20.4903,0.622337},{-15.3363,0.520589}},{{-12.4566,0.627481},{-20.238,0.606098},{-20.7651,0.56974}},
{{-11.6712,0.622265},{-18.2649,0.591062},{-19.2569,0.580894}},{{-12.0943,0.630674},{-22.4432,0.633366},{-17.2197,0.537965}}}};

const double minparams_in[6][6][3][2] = {
{{{12.2692,-0.583057},{17.6233,-0.605722},{19.7018,-0.518429}},{{12.1191,-0.582662},{16.8692,-0.56719},{20.9153,-0.534871}},
{{11.4562,-0.53549},{19.3201,-0.590815},{20.1025,-0.511234}},{{13.202,-0.563346},{20.3542,-0.575843},{23.6495,-0.54525}},
{{12.0907,-0.547413},{17.1319,-0.537551},{17.861,-0.493782}},{{13.2856,-0.594915},{18.5707,-0.597428},{21.6804,-0.552287}}},
{{{5.35616,-0.531295},{16.9702,-0.583819},{36.3388,-0.612192}},{{6.41665,-0.543249},{17.3455,-0.584322},{37.1294,-0.61791}},
{{6.86336,-0.550492},{17.2747,-0.575263},{39.6389,-0.625934}},{{6.82938,-0.558897},{17.8618,-0.599931},{39.3376,-0.631517}},
{{6.05547,-0.54347},{15.7765,-0.569165},{35.6589,-0.611349}},{{6.3468,-0.544882},{16.7144,-0.578363},{38.2501,-0.617055}}},
{{{6.70668,-0.558853},{17.0627,-0.587751},{36.1194,-0.617417}},{{6.3848,-0.542992},{16.6355,-0.581708},{34.6781,-0.609794}},
{{6.36802,-0.539521},{15.9829,-0.569165},{32.5691,-0.59588}},{{5.94912,-0.546191},{18.0321,-0.601764},{36.5238,-0.619185}},
{{5.65108,-0.541684},{15.5009,-0.567131},{34.0489,-0.602048}},{{6.71064,-0.547956},{16.4449,-0.577051},{34.4375,-0.602515}}},
{{{12.4734,-0.608063},{16.1064,-0.575034},{16.0751,-0.536452}},{{12.1936,-0.6034},{15.9302,-0.571271},{14.2791,-0.520157}},
{{12.216,-0.600017},{14.8741,-0.56304},{11.1766,-0.498955}},{{12.7941,-0.616044},{17.1516,-0.583616},{11.6077,-0.500028}},
{{12.7448,-0.611315},{16.2814,-0.572461},{13.1033,-0.506663}},{{12.7949,-0.612051},{16.1565,-0.569143},{12.9295,-0.504203}}},
{{{7.19022,-0.562083},{16.5946,-0.591266},{31.9033,-0.589167}},{{7.80002,-0.571429},{17.8587,-0.595543},{36.5772,-0.630136}},
{{7.96121,-0.569485},{17.8085,-0.592936},{37.553,-0.632848}},{{7.52041,-0.566112},{17.3385,-0.603462},{33.7712,-0.606047}},
{{7.35796,-0.562782},{15.2865,-0.57433},{29.8283,-0.574685}},{{7.80003,-0.571429},{16.1751,-0.583286},{39.1972,-0.642803}}},
{{{13.4466,-0.633911},{22.0097,-0.62205},{18.8862,-0.519652}},{{13.0534,-0.626648},{20.2994,-0.60581},{19.3973,-0.573994}},
{{12.547,-0.62145},{18.9322,-0.596491},{16.2331,-0.546036}},{{14.5339,-0.64585},{20.0211,-0.608462},{19.0405,-0.563914}},
{{12.7388,-0.617954},{21.1677,-0.621012},{15.4502,-0.525165}},{{13.4019,-0.63075},{16.6584,-0.554797},{19.0302,-0.55004}}}};

  TF1 *f1min_S[6];
  TF1 *f1max_S[6];

  TF1 *f2min_S[6];
  TF1 *f2max_S[6];

  TF1 *f3min_S[6];
  TF1 *f3max_S[6];
  

  for(int s =0; s<6; s++){
    f1min_S[s] = new TF1(Form("f1min_S_%d", s), "[0] + [1]*x", 0, 150);
    f1min_S[s]->SetParameters(minparams_in[0][s][0][0],minparams_in[0][s][0][1]);
    
    f1max_S[s] = new TF1(Form("f1max_S_%d", s), "[0] + [1]*x", 0, 150);
    f1max_S[s]->SetParameters(maxparams_in[0][s][0][0],maxparams_in[0][s][0][1]);

    f2min_S[s] = new TF1(Form("f2min_S_%d", s), "[0] + [1]*x", 0, 250);
    f2min_S[s]->SetParameters(minparams_in[0][s][1][0],minparams_in[0][s][1][1]);
    
    f2max_S[s] = new TF1(Form("f2max_S_%d", s), "[0] + [1]*x", 0, 250);
    f2max_S[s]->SetParameters(maxparams_in[0][s][1][0],maxparams_in[0][s][1][1]);

    f3min_S[s] = new TF1(Form("f3min_S_%d", s), "[0] + [1]*x", 0, 300);
    f3min_S[s]->SetParameters(minparams_in[0][s][2][0],minparams_in[0][s][2][1]);
    // f3min_S[s]->SetParameters(0,0);
    
    f3max_S[s] = new TF1(Form("f3max_S_%d", s), "[0] + [1]*x", 0, 300);
    f3max_S[s]->SetParameters(maxparams_in[0][s][2][0],maxparams_in[0][s][2][1]);  
  }


  

  // access to the data root file

  for (int i = 3; i < argc; i++) {
    TFile *inFile = new TFile(argv[i]);
    TTree *inTree;

    // I only care electron tree
    inTree = (TTree *)inFile->Get("electrons");

    cout << "Test Tree: " << inTree << endl;

    // intitial the input branches
    int Runno = 0;
    double Ebeam = 0;
    double gated_charge = 0;
    double livetime = 0;
    double starttime = 0;
    double current = 0;
    clashit *eHit = new clashit;

    inTree->SetBranchAddress("Runno", &Runno);
    inTree->SetBranchAddress("Ebeam", &Ebeam);
    inTree->SetBranchAddress("gated_charge", &gated_charge);
    inTree->SetBranchAddress("livetime", &livetime);
    inTree->SetBranchAddress("starttime", &starttime);
    inTree->SetBranchAddress("current", &current);

    inTree->SetBranchAddress("eHit", &eHit);

    // checking the file we are working on
    cout << "Working on file: " << argv[i] << endl;
    cout << "Number of event in the file" << inTree->GetEntries() << endl;

   
    //Define 3vector for each event for rotation
    TVector3 DC_xyz1, DC_xyz2, DC_xyz3;

    double y1_min =0, y1_max =0;
    double y2_min =0, y2_max =0;
    double y3_min =0, y3_max =0;
    int count1 =0, count1_acc =0;
    int count2 =0, count2_acc =0;
    int count3 =0, count3_acc =0;
    
    // Loop over number of event
    for (int ev = 0; ev < inTree->GetEntries(); ev++) {
      if (ev % 1000000 == 0)
        cout << "Test:" << ev << endl;
      // Clear all branches before getting the entry from tree
      Runno = 0;
      Ebeam = 0;
      gated_charge = 0;
      livetime = 0;
      starttime = 0;
      current = 0;
      eHit->Clear();

      inTree->GetEntry(ev);
      // Check electron information
      if (eHit->getPID() != 11)
        continue;
      if (eHit->getCharge() != -1)
        continue;
      /*
      if (eHit->getEoP() < 0.17)
        continue;
      if (eHit->getEoP() > 0.3)
        continue;
      if (eHit->getEpcal() < 0.07)
        continue;
      if (eHit->getV() < 9)
        continue;
      if (eHit->getW() < 9)
        continue;
      if (eHit->getVtz() < -8)
        continue;
      if (eHit->getVtz() > 3)
        continue;
      */
      if (eHit->getMomentum() < 2.)
        continue;

    
      if (doFiducial) {
        int eSect = fid->GetElectronAcceptance(
            eHit->getTheta() * TMath::RadToDeg(),
            eHit->getPhi() * TMath::RadToDeg(), eHit->getMomentum());
        if (eSect < 0)
          continue;
      }


      //Theta:Phi acceptance plots
      h2_e_th_ph->Fill(eHit->getPhi() * TMath::RadToDeg(),
                       eHit->getTheta() * TMath::RadToDeg());
      

      //Plot global xy for all 6 sectors for 3 region

      h2_DC1_xy->Fill(eHit->getDC_x1(), eHit->getDC_y1());
      h2_DC2_xy->Fill(eHit->getDC_x2(), eHit->getDC_y2());
      h2_DC3_xy->Fill(eHit->getDC_x3(), eHit->getDC_y3());
      
      //Now need to check to plots in Sector coordinator system
      //And get plots for each sector for each layer 

      int sector_ID = eHit->getDC_sector();
      DC_xyz1.SetXYZ(eHit->getDC_x1(), eHit->getDC_y1(), 0);
      DC_xyz2.SetXYZ(eHit->getDC_x2(), eHit->getDC_y2(), 0);
      DC_xyz3.SetXYZ(eHit->getDC_x3(), eHit->getDC_y3(), 0);
      count1++;
      count2++;
      count3++;
      
      //Rotate to the sector cordinators system
      double rot_ang = -(sector_ID -1)*60 *  TMath::DegToRad();
      DC_xyz1.RotateZ(rot_ang);
      DC_xyz2.RotateZ(rot_ang);
      DC_xyz3.RotateZ(rot_ang);

      //I would like to do the test for each sector first
      h2_DC_xy1_S[sector_ID-1]->Fill(DC_xyz1.X(), DC_xyz1.Y());
      h2_DC_xy2_S[sector_ID-1]->Fill(DC_xyz2.X(), DC_xyz2.Y());
      h2_DC_xy3_S[sector_ID-1]->Fill(DC_xyz3.X(), DC_xyz3.Y());


      //checking DC fiducial for layer 1
      y1_min = f1min_S[sector_ID -1]->Eval(DC_xyz1.X());
      y1_max = f1max_S[sector_ID -1]->Eval(DC_xyz1.X());

      if(DC_xyz1.Y() > y1_min && DC_xyz1.Y()< y1_max){
          h2_DC1_xy_acc->Fill(eHit->getDC_x1(), eHit->getDC_y1());
	  count1_acc++;
      }

       //checking DC fiducial for layer 2
      y2_min = f2min_S[sector_ID -1]->Eval(DC_xyz2.X());
      y2_max = f2max_S[sector_ID -1]->Eval(DC_xyz2.X());

      if(DC_xyz2.Y() > y2_min && DC_xyz2.Y()< y2_max){
          h2_DC2_xy_acc->Fill(eHit->getDC_x2(), eHit->getDC_y2());
	  count2_acc++;
      }

       //checking DC fiducial for layer 3
      y3_min = f3min_S[sector_ID -1]->Eval(DC_xyz3.X());
      y3_max = f3max_S[sector_ID -1]->Eval(DC_xyz3.X());

      // cout << "y3_min: "<<y3_min<<  endl;

      if(DC_xyz3.Y() > y3_min && DC_xyz3.Y()< y3_max){
          h2_DC3_xy_acc->Fill(eHit->getDC_x3(), eHit->getDC_y3());
	  count3_acc++;
      }

      
   
      

    } // end loop event

    cout << "count1, count2, count3: "<< count1<< "  "<< count2<< "  "<< count3<< endl;
    cout << "count1_acc, count2_acc, count3_acc: "<< count1_acc<< "  "<< count2_acc<< "  "<< count3_acc<< endl;
    cout << "acc: "<< count1_acc/count1<< "   "<< count2_acc/count2<< "  "<< count3_acc/count3<< endl;

    // inFile->Close();
  } // end loop over file




  

  // Doing the plots

  TCanvas *c0 = new TCanvas("c0", "", 800, 600);
  h2_e_th_ph->Draw("colz");

  //Plot for region1 

  TCanvas *c1 = new TCanvas("c1", "", 800, 600);
  h2_DC1_xy->Draw();
  h2_DC1_xy->SetMarkerColor(2);
  h2_DC1_xy->SetMarkerSize(2);

  h2_DC1_xy->GetXaxis()->SetTitle("X [cm]");
  h2_DC1_xy->GetYaxis()->SetTitle("Y [cm]");
  h2_DC1_xy_acc->Draw("colz same");

  TCanvas *c11 = new TCanvas("c11", "", 1200, 800);
  c11->Divide(3,2);
  for(int i =1; i<7; i++){
    c11->cd(i);
    h2_DC_xy1_S[i-1]->Draw("colz");
    h2_DC_xy1_S[i-1]->GetXaxis()->SetTitle("X [cm]");
    h2_DC_xy1_S[i-1]->GetYaxis()->SetTitle("Y [cm]");

    f1min_S[i-1]->Draw("same");
    f1min_S[i-1]->SetLineColor(2);

    f1max_S[i-1]->Draw("same");
    f1max_S[i-1]->SetLineColor(2);
  }
  
  //Plot for region2

   TCanvas *c2 = new TCanvas("c2", "", 800, 600);
   h2_DC2_xy->Draw("");
   h2_DC2_xy->SetMarkerColor(2);
   h2_DC2_xy->SetMarkerSize(2);

   h2_DC2_xy->GetXaxis()->SetTitle("X [cm]");
   h2_DC2_xy->GetYaxis()->SetTitle("Y [cm]");
   h2_DC2_xy_acc->Draw("colz same");

   TCanvas *c21 = new TCanvas("c21", "", 1200, 800);
   c21->Divide(3,2);
   for(int i =1; i<7; i++){
    c21->cd(i);
    h2_DC_xy2_S[i-1]->Draw("colz");
    h2_DC_xy2_S[i-1]->GetXaxis()->SetTitle("X [cm]");
    h2_DC_xy2_S[i-1]->GetYaxis()->SetTitle("Y [cm]");
    
    f2min_S[i-1]->Draw("same");
    f2min_S[i-1]->SetLineColor(2);

    f2max_S[i-1]->Draw("same");
    f2max_S[i-1]->SetLineColor(2);
    
    
   }

    
    //Plot for region 3
   TCanvas *c3 = new TCanvas("c3", "", 800, 600);
   h2_DC3_xy->Draw();
   h2_DC3_xy->SetMarkerColor(2);
   h2_DC3_xy->SetMarkerSize(2);

   h2_DC3_xy->GetXaxis()->SetTitle("X [cm]");
   h2_DC3_xy->GetYaxis()->SetTitle("Y [cm]");
   h2_DC3_xy_acc->Draw("colz same");

   
   TCanvas *c31 = new TCanvas("c31", "", 1200, 800);
   c31->Divide(3,2);
   for(int i =1; i<7; i++){
    c31->cd(i);
    h2_DC_xy3_S[i-1]->Draw("colz");
    
    f3min_S[i-1]->Draw("same");
    f3min_S[i-1]->SetLineColor(2);

    f3max_S[i-1]->Draw("same");
    f3max_S[i-1]->SetLineColor(2);
    
   }

  // outFile->cd();

  // h2_e_th_ph->Write();

  // outFile->Close();

  cout << "All Done: " << endl;

  myapp->Run();
  return 0;
}
