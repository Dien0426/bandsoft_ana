// I want to get the charge from the list of
// tagged file which will be included in skim file
#include "bandhit.h"
#include "clashit.h"
//#include "constants.h"
#include "taghit.h"

void get_Q_skim_tagg() {
  // Getting the runlist_file
  ifstream run_inc;
  ofstream outfile;
  run_inc.open("skim_good_run_tagg.txt");

  // outfile.open("Q_skim_inc.txt");
  outfile.open("Q_skim_good_run_tagg.txt");

  TString filename;
  double charge1 = 0;
  double charge2 = 0;
  double Q_tot1 = 0;
  double Q_tot2 = 0;
  int N_run = 0;

  // Now get the charge for the largest_ID event
  double charge3 = 0;

  //=====Getting the tagg data======//
  while (run_inc >> filename) {

    TFile *inc_file = new TFile(Form("./skim_files_ID/%s", filename.Data()));

    if (inc_file->IsOpen() == false)
      continue;

    double gated_Q = 0;

    int Max_ID = 0;

    double gated_max = 0;

    int eventnumber;

    TTree *inc_tree = (TTree *)inc_file->Get("tagged");

    inc_tree->SetBranchAddress("gated_charge", &gated_Q);
    inc_tree->SetBranchAddress("eventnumber", &eventnumber);

    int Nentries = inc_tree->GetEntries();
    Max_ID = inc_tree->GetMaximum("eventnumber");
    charge1 = inc_tree->GetMaximum("gated_charge");

    // loop over the event
    for (int i = 0; i < Nentries; i++) {

      if (i % 100000 == 0)
        cout << "process event " << i << endl;

      inc_tree->GetEntry(i);

      if (eventnumber == Max_ID) {
        cout << "Find the entry: " << i << "   " << eventnumber << "   "
             << Max_ID << endl;

	charge3 = gated_Q;

        cout << "gated_charge for the larget_ID: " << gated_Q << endl;
      }

      if (i == Nentries - 1) {

	charge2 = gated_Q;

        cout << i << "        " << gated_Q << endl;
      }
    }

    outfile << filename << "     "<< charge1 << "    "<< charge2 << "    "<< charge3<< endl;
  }


}
