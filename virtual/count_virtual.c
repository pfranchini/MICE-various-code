
/////////////////////////////////////////
//
//  Show where the Virtual planes are
//
/////////////////////////////////////////
  
#include <iostream>
#include <string>
#include <vector>

#include "TCanvas.h"
#include "TGraph.h"
#include "TH2D.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TLine.h"

#include "src/common_cpp/JsonCppStreamer/IRStream.hh"
#include "src/common_cpp/DataStructure/Spill.hh"
#include "src/common_cpp/DataStructure/Data.hh"
#include "src/common_cpp/DataStructure/ReconEvent.hh"
#include "src/common_cpp/DataStructure/ThreeVector.hh"
#include "src/common_cpp/DataStructure/MCEvent.hh"
#include "src/common_cpp/DataStructure/VirtualHit.hh"
#include "Math/WrappedMultiTF1.h"

Int_t maxZ, minZ;


void draw_line(float position, int color, float heigth) {

  if ((position>maxZ) || (position<minZ)) return;

  TLine *line1 = new TLine(position,0,position,heigth);
  line1->SetLineColor(color);
  line1->SetLineWidth(2);
  line1->SetLineStyle(9);
  line1->Draw("sames");

}

int main() {

  // Variables:

  //////////////////////////////
  Int_t spills = 20000;
  minZ = 0;
  maxZ = 16000;
  //////////////////////////////
  
  MAUS::Data data;
  MAUS::Spill* spill;
  MAUS::VirtualHitArray* hits;
  std::vector<MAUS::MCEvent*>* MCevts;

  std::string filename = "mc_3mm200_07469_MAUS-trunk_magic_p4-v10-corr_minus2-diffuserVP.root";

  irstream infile(filename.c_str(), "Spill");
  std::cout << "Processing MC file: " << filename << std::endl;

  TCanvas *c1 = new TCanvas("c1","histograms",1800,900);
  c1->Divide(2,2);
  gStyle->SetOptStat(1101);
  
  Double_t z;
  Int_t nspill=0;
    
  // Loop over all spills   
  while ( (infile >> readEvent != NULL) ) {
    
    infile >> branchName("data") >> data;
    spill = data.GetSpill();
    
    if (spill == NULL || !(spill->GetDaqEventType() == "physics_event")) continue;

    nspill++;
    if (nspill>=spills) break;
    std::cout << "\b\b" << nspill;
    
    MCevts = spill->GetMCEvents();
    // Loop over MC events in spill                                                  
    for ( size_t j = 0; j < MCevts->size(); ++j ) {
      if ( !MCevts->at(j) ) continue;
      
      // Loop over VirtualHits in the MC event                                                                                                                          
      for ( size_t k = 0; k < MCevts->at(j)->GetVirtualHits()->size(); ++k ) {
	
	hits = MCevts->at(j)->GetVirtualHits();
	z = hits->at(k).GetPosition().z();

	std::cout << "Position: " << z << std::endl;
      }
    }
  }
  
  return 0;
}
