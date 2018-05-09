#ifndef ROOTINCLUDE_HH
#define ROOTINCLUDE_HH
//-----------------------------------------------------------
// Included files needed from ROOT and C++
//-----------------------------------------------------------

#include <iomanip>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <time.h>
#include <zlib.h>
#include <algorithm>
#include <map>
#include <cassert>


using std::cout;
using std::string;
using std::cin;
using std::endl;
using std::flush;
using std::setw;
using std::streamsize;
using std::setprecision;
using std::string;
using std::stringstream;
using std::ifstream;
using std::ios;
using std::sort;
using std::map;
using std::pair;

//#include "Cintex/Cintex.h"

// ROOT headers
#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TApplication.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>
#include <TMath.h>
#include <TChain.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH1.h>
#include <TH2.h>
#include <TH2Poly.h>
#include <TH3.h>
#include <TPostScript.h>
#include <TLatex.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TAxis.h>
#include <TProfile.h>
#include <TVector3.h>
#include <TStyle.h>
#include <TText.h>
#include <TStyle.h>
#include <TRandom.h>
#include <TRandom3.h>
#include <TArrow.h>
#include <TLine.h>
#include <TMarker.h>
#include <TF1.h>
#include <TF2.h>
#include <TGraph.h>
#include <TVector.h>
#include <TArray.h>
#include <TLegend.h>
#include <THStack.h>
#include <TChainElement.h>
#include <TFractionFitter.h>
#include <TObject.h>
#include <TColor.h>
#include <TSpectrum.h>
#include <THStack.h>
#include <TGaxis.h>
#include <TMultiLayerPerceptron.h> // neural networks
#include <TMLPAnalyzer.h> // neural networks
#include <TPaveStats.h>
#include <TGraphAsymmErrors.h>
#include <TGraphErrors.h>
#include <TGraph2D.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
//#include <RooConstVar.h>
#endif

#endif
