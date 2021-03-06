/*
 * C++ Program to read the pdf values from an ntuple,
 * and use LHAPDF to calculate the density functions,
 * and produce weights.
 *
 *Example Execution
 *./PDFProducer.exe "/afs/cern.ch/work/c/cranelli/public/WGamGam/Acceptances/AnalysisRECOCuts_Skim/LepGammaGammaFinalEl_2015_03_31_ScaleFactors/job_summer12_Wgg_FSR/Job_0000/tree.root" "test.root" "ggNtuplizer/EventTree"
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

//const string inRootFileLoc = "/afs/cern.ch/work/c/cranelli/public/WGamGam/Acceptances/AnalysisRECOCuts_Skim/LepGammaGammaFinalEl_2015_03_31_ScaleFactors/job_summer12_Wgg_FSR/Job_0000/tree.root";
//const string outfileLoc = "tree.root"; 
//const string treeLoc = "ggNtuplizer/EventTree";

//PDF Set Names
//string array_setnames[] = {"cteq6l1", "cteq66", "MSTW2008lo68cl"};
string array_setnames[] = {"NNPDF30_nlo_nf_5_pdfas", "CT10nlo", "MSTW2008nlo68cl"};
vector<string> setnames (array_setnames, array_setnames + sizeof(array_setnames) / sizeof(string));

int main(int argc, char *argv[]){
  //Catch if the user does not supply enough command line arguments
  if(argc !=4){
    cout << "useage: " << argv[0] << " <infilename> <outfilename> <treedir>" << endl;
    return 0;
  }

  string inRootFileLoc = argv[1];
  string outfileLoc = argv[2];
  string treeLoc = argv[3];
  
  TFile * infile = TFile::Open(inRootFileLoc.c_str(), "READ");
  TFile * outfile  = TFile::Open(outfileLoc.c_str(), "RECREATE");

  TTree * tree =(TTree*)infile->Get((inRootFileLoc+":ggNtuplizer/EventTree").c_str());

  Float_t original_pdf[7];
  TBranch *b_pdf;
  tree->SetBranchAddress("pdf", original_pdf, &b_pdf);

  TTree * out_tree = new TTree("out_tree", "Contains Parton Distribution calculations for different PDF Sets");

  //Clone Original Tree
  out_tree = tree->CloneTree(0);
 
  map<string,pair<vector<double>, vector<double> > > map_xfx;
  map<string, vector<LHAPDF::PDF*> > map_pdfs;

  //Loop Over PDF Sets
  for( unsigned int i=0; i < setnames.size(); i++){
    
    //Set New Branches
    pair<vector<double> , vector<double> > xfx;
    map_xfx.insert(pair<string, pair< vector<double>, vector<double> > >(setnames[i], xfx));
    
    stringstream branch_name_first, branch_name_second;
    branch_name_first << "xfx_first_" << setnames[i];
    out_tree->Branch(branch_name_first.str().c_str(),
		     "std::vector<double>",&(map_xfx[setnames[i]].first));
    branch_name_second << "xfx_second_" << setnames[i];
    out_tree->Branch(branch_name_second.str().c_str(),
		     "std::vector<double>",&(map_xfx[setnames[i]].second));

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
  for (Long64_t ientry=0; ientry<nentries;ientry++) {
  //for (Long64_t ientry=0; ientry<10000;ientry++) {
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
      map_xfx[ setnames[i] ].first.clear();
      map_xfx[ setnames[i] ].second.clear();
      for(unsigned int j =0; j < map_pdfs[setnames[i]].size(); j++){
      
	// Calculates x f(x) at a given x and Q for a certain particle ID
	map_xfx[ setnames[i] ].first.push_back(map_pdfs[ setnames[i] ][j]->xfxQ(id_first, x_first, Q));
	map_xfx[ setnames[i] ].second.push_back(map_pdfs[ setnames[i] ][j]->xfxQ(id_second, x_second, Q));
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
