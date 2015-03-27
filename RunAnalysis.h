#ifndef PDFPRODUCER_H
#define PDFPRODUCER_H

#include "LHAPDF/LHAPDF.h"
#include "TTree.h"
#include "TFile.h"

using namespace std;


//Root File Information                        
const string inRootFileLoc = "/afs/cern.ch/work/c/cranelli/public/WGamGam/ggNtuples/ggtree_mc_ISR.root";
const string outfilepath = "test.root";
const string treeLoc = "ggNtuplizer/EventTree";

//PDF Set Names 
string array_setnames[] = {"cteq6l1", "CT10"};
vector<string> setnames (array_setnames, array_setnames + sizeof(array_setnames) / sizeof(string));


TFile * infile = TFile::Open(inRootFileLoc.c_str(), "READ");
TFile * outfile  = TFile::Open(outfilepath.c_str(), "RECREATE");


class RunModule {
 public :
  RunModule() {}

  void initialize();
  void execute();

 private :
  map<string,pair<vector<double>, vector<double> > > map_xfx;
  map<string, vector<LHAPDF::PDF*> > map_pdfs;

  TTree * tree;
  TTree * out_tree;

  Float_t original_pdf[7];

};

#endif
