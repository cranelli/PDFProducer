/*
 * C++ Program to read the pdf values from an ntuple,
 * and use LHAPDF to calculate the density functions,
 * and produce weights.
 */

#include "LHAPDF/LHAPDF.h"
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TDirectory.h"
#include "TBranch.h"

#include <iostream>
#include <fstream>

//using namespace LHAPDF;
using namespace std;

//Root File Information
const string inRootFileLoc = "/afs/cern.ch/work/c/cranelli/public/WGamGam/ggNtuples/ggtree_mc_ISR.root";
const string outfilepath = "test.root"; 
const string treeLoc = "ggNtuplizer/EventTree";

//PDF Set Names
string array_setnames[] = {"cteq6l1", "CT10"};
vector<string> setnames (array_setnames, array_setnames + sizeof(array_setnames) / sizeof(string));

int main(){
  TFile * infile = TFile::Open(inRootFileLoc.c_str(), "READ");
  TFile * outfile  = TFile::Open(outfilepath.c_str(), "RECREATE");

  TTree * tree =(TTree*)infile->Get((inRootFileLoc+":ggNtuplizer/EventTree").c_str());
  Float_t original_pdf[7];
  TBranch *b_pdf;
  tree->SetBranchAddress("pdf", original_pdf, &b_pdf);

  TTree * out_tree = new TTree("out_tree", "Contains Parton Distribution calculations for different PDF Sets");
 
  map<string,vector<double> > map_xfx;
  map<string, vector<LHAPDF::PDF*> > map_pdfs;

  //Loop Over PDF Sets
  for( unsigned int i=0; i < setnames.size(); i++){
    
    //Set New Branches
    vector<double> xfx;
    map_xfx.insert(pair<string, vector<double> >(setnames[i], xfx));
    
    stringstream branch_name;
    branch_name << "xfx_first_" << setnames[i];
    out_tree->Branch(branch_name.str().c_str(),
		     "std::vector<double>",&(map_xfx[setnames[i]]));

    // Genereate PDF Sets
    //unsigned int k = i+1;
    //LHAPDF::initPDFSet(i,setnames[i]);
   
    LHAPDF::PDFSet set(setnames[i]);
    //size_t nmem = set.size()-1;
    //cout << nmem << endl;
    vector<LHAPDF::PDF*>  pdfs = set.mkPDFs();
    map_pdfs.insert(pair<string, vector<LHAPDF::PDF*> >(setnames[i], pdfs));
    
  }

  Long64_t nentries = tree->GetEntries();
  cout << "Number of Entries: " << nentries << endl;


  // Loop Over Events
  //for (Long64_t ientry=0; ientry<nentries;ientry++) {
  for (Long64_t ientry=0; ientry<100000;ientry++) {
    if(ientry % 1000 == 0) cout << ientry << endl;
    
    tree->GetEntry(ientry);

     // From PDF Branch set id, x, and Q info.    
    float id_first = original_pdf[0];
    float id_second =  original_pdf[1];
    float x_first = original_pdf[2];
    float x_second = original_pdf[3];
    float Q = original_pdf[6];

    //For Each PDF Set, use the PDFs to calculate the xfx vectors.
    
    for(unsigned int i=0; i < setnames.size(); i++){
      //cout << setnames[i] << endl;
      map_xfx[ setnames[i] ].clear();
      for(unsigned int j =0; j < map_pdfs[setnames[i]].size(); j++){
      
	// Calculates x f(x) at a given x and Q for a certain particle ID
	map_xfx[ setnames[i] ].push_back(map_pdfs[ setnames[i] ][j]->xfxQ(id_first, x_first, Q));
	//cout << map_pdfs[ setnames[i] ][j]->xfxQ(id_first, x_first, Q) << endl;

      }

      /*
      cout << LHAPDF::numberPDF(i) << endl;
      unsigned int n_pdfs =1; 
      if( LHAPDF::numberPDF(i) >1) n_pdfs += LHAPDF::numberPDF(i);
      //cout << n_pdfs << endl;
      for( unsigned int j =0; j < n_pdfs; ++j){
	//cout << j << endl;
	// Calculates x f(x) at a given x and Q for a certain particle ID
	map_xfx[setnames[i] ]->push_back(LHAPDF::xfx(i, id_first, x_first, Q));
	}
      */
     
    }
  

    out_tree->Fill();
  }
   
  infile->Close();
  outfile->Write();
  outfile->Close();

  return 1;
}
