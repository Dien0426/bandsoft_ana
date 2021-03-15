//Want to write a code using clas12root to get_Q
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <chrono>
#include <TFile.h>
#include <TTree.h>
#include <TApplication.h>
#include <TROOT.h>
#include <TDatabasePDG.h>
#include <TLorentzVector.h>
#include <TH1.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TBenchmark.h>
#include "clas12reader.h"
#include <TString.h>

void get_Q_hipo(){
 
   ifstream infile;
   ofstream outfile;

   infile.open("./hipo_files_name.txt");
   outfile.open("run_Q.txt");
  
   TString filename;
   TString hipofile;
   
   while(infile >> filename){

    hipofile = Form("./hipo_files/%s", filename.Data());

    cout << hipofile.Data()<< endl;
     
    clas12reader c12(hipofile.Data());

   auto scal = c12.scalerReader();


   outfile << filename << "      "<< c12.getRunBeamCharge()<< endl;

   }

   outfile.close();
}
