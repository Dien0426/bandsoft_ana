// I created a simple skim (convert hipo - root)
// just to read RUN::config -> "eventnumber"
// Read RUN::Event-> gated_charge
// Read RUN::scaler->fcupgated &fcup
// Each run make a plots of these 3 ways of Q vs event number
// And get the max_ID = max_eventnumber
// And Q out at that max_ID
#include "bandhit.h"
#include "clashit.h"
#include "taghit.h"

void get_Q_root_hipo() {
  // Getting the runlist_file
  ifstream run_inc;
  ofstream outfile;
  run_inc.open("./Q_root/Q_run_list.txt");

  // outfile.open("Q_skim_inc.txt");
  outfile.open("Q_root_hipo_good.txt");

  TString filename;
  double Q_gated = 0;     // gated_Q(max_ID)
  double Q_fcupgated = 0; // fcupgated(max_ID)
  double Q_fcup = 0;      // fcup(max_ID)
  int N_run = 0;

  //=====Getting the tagg data======//

  // TCanvas *c0 = new TCanvas();
  // c0->Print("Q_root_hipo_scale_event.pdf[");

  while (run_inc >> filename) {

    cout<< "Working on file: "<< filename<< endl;

    TFile *inc_file = new TFile(Form("./Q_root/%s", filename.Data()));

    if (inc_file->IsOpen() == false)
      continue;

    int Max_ID = 0;

    int eventnumber = 0;
    double gated_charge = 0;
    double fcupgated = 0;
    double fcup = 0;

    double max_ID_last=0;
    double gated_charge_last =0;
    double fcupgated_last =0;

    TTree *inc_tree = (TTree *)inc_file->Get("Q");

    // TCanvas *c = new TCanvas(Form("c_%d", N_run));

    //inc_tree->Draw("gated_charge:eventnumber>>h1()");

    //inc_tree->Draw("fcupgated:eventnumber>>h2()"), "", "same";

    //inc_tree->Draw("fcup:eventnumber>>h3()", "", "same");

    // c->Print("Q_root_hipo_scale_event.pdf");

    inc_tree->SetBranchAddress("gated_charge", &gated_charge);
    inc_tree->SetBranchAddress("eventnumber", &eventnumber);
    inc_tree->SetBranchAddress("fcupgated", &fcupgated);
    inc_tree->SetBranchAddress("fcup", &fcup);

    int Nentries = inc_tree->GetEntries();
    Max_ID = inc_tree->GetMaximum("eventnumber");

    // loop over the event
    for (int i = 0; i < Nentries; i++) {

      if (i % 10000000 == 0)
        cout << "process event " << i << endl;

      inc_tree->GetEntry(i);

      if (eventnumber == Max_ID) {
        cout << "Find the entry: " << i << "   " << eventnumber << "   "
             << Max_ID << endl;

	cout << fcupgated<< "    "<< fcup << "    "<< gated_charge<< endl;

        Q_gated = gated_charge;
        Q_fcupgated = fcupgated;
        Q_fcup = fcup;
      }

      if(i == Nentries -1){
	gated_charge_last = gated_charge;
	fcupgated_last = fcupgated;

	cout <<"Last entry, event number, gated_charge: "<< eventnumber << "    "<< gated_charge<< endl;

      }
    }

    outfile << filename << "     " << Q_fcupgated << "    " << Q_fcup << "    "
            << Q_gated << "    "<< gated_charge_last<< "    "<< fcupgated_last<<  endl;

    N_run++;
  }

  //c0->Print("Q_root_hipo_scale_event.pdf]");
}
