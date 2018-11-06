
/////////////////////////////////////////
//
//  Get Virtual planes and plot
//  - #events vs Z
//  - mean R vs Z
//  - x,y vs Z
//  - Pz,P vs R for each Z
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
  Int_t maxR = 500;
  //////////////////////////////

  Int_t bins = trunc((maxZ-minZ)/500);
  
  MAUS::Data data;
  MAUS::Spill* spill;
  MAUS::VirtualHitArray* hits;
  std::vector<MAUS::MCEvent*>* MCevts;

  // Geometry:

  std::vector<int> elements;
  elements.push_back(5287.24720607); // TOF0
  elements.push_back(5642.7616468);  // CKOV1
  elements.push_back(6083.56572488); // CKOV2

  std::vector<int> quads;
  //26.21                         // D2
  quads.push_back(867.308);       // Q4
  quads.push_back(2027.308);      // Q5
  quads.push_back(3187.308);      // Q6
  quads.push_back(8499.408);      // Q7
  quads.push_back(9659.408);      // Q8
  quads.push_back(10819.408);     // Q9

  elements.push_back(12929.5636425); // TOF1
  elements.push_back(14515.836783);  // TKU
  elements.push_back(16757.029);     // FC1
  elements.push_back(17161.43);      // FC2
  elements.push_back(19393.7675747); // TKD
  elements.push_back(21139.3745347); // TOF2
  elements.push_back(21221.6490768); // KL

  //std::string filename = "/home/phswbb/MC/Tom_3-140+M3-Test4_800MeV_DS_Y+2deg/Tom_3-140+M3-Test4_merged.root";
  //std::string filename = "MC/mc_3mm200_07469_MAUS-trunk_magic_p4-v4-rot-glue.root";
  std::string filename = "/home/phswbb/MC/7469/7469_merged.root";

  irstream infile(filename.c_str(), "Spill");
  std::cout << "Processing MC file: " << filename << std::endl;

  TCanvas *c1 = new TCanvas("c1","histograms",1800,900);
  c1->Divide(2,2);
  gStyle->SetOptStat(1101);
  
  TH2D* radius = new TH2D("radius", "R vs Z", bins, minZ, maxZ, 200, 0, maxR);
  TH2D* x = new TH2D("x", "X vs Z", bins, minZ, maxZ, 200, -maxR, +maxR);
  TH2D* y = new TH2D("y", "Y vs Z", bins, minZ, maxZ, 200, -maxR, +maxR);
  
  TH1D* pions = new TH1D("pions","Number of events vs Z", bins, minZ, maxZ);
  TH1D* muons = new TH1D("muons","Number of events vs Z", bins, minZ, maxZ);
  TH1D* positrons = new TH1D("positrons","Number of events vs Z", bins, minZ, maxZ);
  TH1D* protons = new TH1D("protons","Number of events vs Z", bins, minZ, maxZ);
  pions->GetXaxis()->SetTitle("z [mm]");
  muons->GetXaxis()->SetTitle("z [mm]");
  positrons->GetXaxis()->SetTitle("z [mm]");
  protons->GetXaxis()->SetTitle("z [mm]");
  pions->SetLineColor(kRed);
  muons->SetLineColor(kBlack);
  positrons->SetLineColor(kGreen);
  protons->SetLineColor(kBlue);

  TH1D** Pz    = new TH1D*[bins]; 
  TH2D** PzvsR = new TH2D*[bins]; 
  for ( int i=0; i<bins; i++){
    Pz[i]    = new TH1D( Form("Pz%d", i*500 + minZ), Form("Pz - Z:%d", i*500 + minZ), 100, 0, 350 ); 
    PzvsR[i] = new TH2D( Form("PzvsR%d", i*500 + minZ), Form("Pz vs R - Z:%d", i*500 + minZ), 200, 0, maxR, 100, 0, 350 ); 
    PzvsR[i]->GetXaxis()->SetTitle("R [mm]");
    PzvsR[i]->GetYaxis()->SetTitle("Pz [MeV/c]");
    Pz[i]->GetXaxis()->SetTitle("Pz [MeV/c]");
  }

  TH1D** Pt    = new TH1D*[bins]; 
  TH2D** PtvsR = new TH2D*[bins]; 
  for ( int i=0; i<bins; i++){
    Pt[i]    = new TH1D( Form("Pt%d", i*500 + minZ), Form("Pt - Z:%d", i*500 + minZ), 100, 0, 100 ); 
    PtvsR[i] = new TH2D( Form("PtvsR%d", i*500 + minZ), Form("Pt vs R - Z:%d", i*500 + minZ), 200, 0, maxR, 100, 0, 100 ); 
    PtvsR[i]->GetXaxis()->SetTitle("R [mm]");
    PtvsR[i]->GetYaxis()->SetTitle("Pt [MeV/c]");
    Pt[i]->GetXaxis()->SetTitle("Pt [MeV/c]");
  }

  TH1D** Y    = new TH1D*[bins]; 
  TH2D** XvsY = new TH2D*[bins]; 
  for ( int i=0; i<bins; i++){
    Y[i]    = new TH1D( Form("Y%d", i*500 + minZ), Form("Y - Z:%d", i*500 + minZ), 200, -maxR, maxR ); 
    XvsY[i] = new TH2D( Form("XvsY%d", i*500 + minZ), Form("Y vs X - Z:%d", i*500 + minZ), 200, -maxR, maxR, 200, -maxR, maxR ); 
    XvsY[i]->GetXaxis()->SetTitle("X [mm]");
    XvsY[i]->GetYaxis()->SetTitle("Y [mm]");
    Y[i]->GetXaxis()->SetTitle("Y [mm]");
  }

  TLegend *leg1 = new TLegend(0.8,0.7,0.9,0.9);
  leg1->AddEntry(pions,"Pions","l");
  leg1->AddEntry(muons,"Muons","l");
  //leg1->AddEntry(positrons,"Positrons","l");
  leg1->AddEntry(protons,"Protons","l");
  
  Double_t z,r,pt;
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
	if ( int(z)%500 != 0 ) continue;

	if (hits->at(k).GetParticleId()==-13) {
	  muons->Fill(z);

	  // x,y vs z
	  x->Fill(z,hits->at(k).GetPosition().x());
	  y->Fill(z,hits->at(k).GetPosition().y());
	  
	  // r vs z
	  r = sqrt( pow(hits->at(k).GetPosition().x(),2) + pow(hits->at(k).GetPosition().y(),2) );
	  radius->Fill(z,r);
	  
	  // Pz v R for each z
	  if ((z>minZ) && (z<maxZ)){
	    Pz[int((z-minZ)/500)]->Fill(hits->at(k).GetMomentum().z());
	    PzvsR[int((z-minZ)/500)]->Fill(r,hits->at(k).GetMomentum().z());
	  }
	  
	  // Pt v R for each z
	  if ((z>minZ) && (z<maxZ)){
	    pt = sqrt( pow(hits->at(k).GetMomentum().x(),2) + pow(hits->at(k).GetMomentum().y(),2) );
	    Pt[int((z-minZ)/500)]->Fill(pt);
	    PtvsR[int((z-minZ)/500)]->Fill(r,pt);
	  }
	  
	  // Y v R for each z
	  if ((z>minZ) && (z<maxZ)){
	    Y[int((z-minZ)/500)]->Fill( hits->at(k).GetPosition().y() );
	    XvsY[int((z-minZ)/500)]->Fill(hits->at(k).GetPosition().x(), hits->at(k).GetPosition().y() );
	  }
	  
	  
	  
	}
	
	if (hits->at(k).GetParticleId()==211) pions->Fill(z);
	if (hits->at(k).GetParticleId()==2212) protons->Fill(z);
	
      }
     }
  }
  
  //////////////////////////////////////////
  // Normalize TH2D for each vertical slice
  
  c1->cd(1);
  gPad->Range(minZ,0,maxZ,maxR);
  TH2D* radius_norm = new TH2D("radius_norm", "Muons: R vs Z", bins, minZ, maxZ, 200, 0, maxR);
  
  for ( Int_t i = 0; i < radius->GetNbinsX(); i++ ) {
    TH1D *prof = radius->ProjectionY("prof",i,i,"");
    for ( Int_t j = 0; j < radius->GetNbinsY(); j++ ) {
      
      radius_norm->SetBinContent(i,j,radius->GetBinContent(i,j)/prof->GetEntries());
      
    }
  }
  
  radius_norm->Draw("colz");

  ///////////////

  // Mean of each slice  
  
  TGraph* graph = new TGraph(radius->GetNbinsX());
  graph->SetMaximum(maxR);
  graph->SetTitle("Mean radius");
  graph->GetXaxis()->SetTitle("z [m]");
  graph->GetYaxis()->SetTitle("r [mm]");
  for ( Int_t bin = 1; bin < radius->GetNbinsX(); bin++ ){
    TH1D *prof = radius->ProjectionY("prof",bin,bin+1,"");
    graph->SetPoint(bin*maxZ/radius->GetNbinsX(),bin*maxZ/radius->GetNbinsX(),prof->GetMean());
    
  }
  
  graph->Draw("sames*");
  
  for ( unsigned int el=0; el<elements.size(); el++ )
    draw_line(elements[el],4,maxR);
  
  for ( unsigned int el=0; el<quads.size(); el++ )
    draw_line(quads[el],2,maxR);
  
  TLine *line2 = new TLine(minZ,236,maxZ,236);
  line2->SetLineColor(16   );
  line2->SetLineWidth(3);
  //line2->SetLineStyle(9);
  line2->Draw("sames");  

  /// x and y

  c1->cd(2);
  gPad->Range(minZ,0,maxZ,maxR);
  TH2D* x_norm = new TH2D("x_norm", "Muons: X vs Z", bins, minZ, maxZ, 200, -maxR, +maxR);
  
  for ( Int_t i = 0; i < x->GetNbinsX(); i++ ) {
    TH1D *prof = x->ProjectionY("prof",i,i,"");
    for ( Int_t j = 0; j < x->GetNbinsY(); j++ ) {
      
      x_norm->SetBinContent(i,j,x->GetBinContent(i,j)/prof->GetEntries());
      
    }
  }
  
  x_norm->Draw("colz");
  TLine *line3 = new TLine(minZ,0,maxZ,0);
  line3->SetLineColor(16);
  line3->SetLineWidth(3);
  line3->Draw("sames");

  for ( unsigned int el=0; el<elements.size(); el++ )
    draw_line(elements[el],4,maxR);
  
  for ( unsigned int el=0; el<quads.size(); el++ )
    draw_line(quads[el],2,maxR);


  c1->cd(4);
  gPad->Range(minZ,0,maxZ,maxR);
  TH2D* y_norm = new TH2D("y_norm", "Muons: Y vs Z", bins, minZ, maxZ, 200, -maxR, +maxR);
  
  for ( Int_t i = 0; i < y->GetNbinsX(); i++ ) {
    TH1D *prof = x->ProjectionY("prof",i,i,"");
    for ( Int_t j = 0; j < y->GetNbinsY(); j++ ) {
      
      y_norm->SetBinContent(i,j,y->GetBinContent(i,j)/prof->GetEntries());
      
    }
  }
  
  y_norm->Draw("colz");
  line3->Draw("sames");
  
  for ( unsigned int el=0; el<elements.size(); el++ )
    draw_line(elements[el],4,maxR);
  
  for ( unsigned int el=0; el<quads.size(); el++ )
    draw_line(quads[el],2,maxR);

  ////////////////////////////////////////////
  
  c1->cd(3);
  gPad->SetLogy();

  pions->Draw();
  muons->Draw("sames");
  //positrons->Draw("sames");  // Do not plot e+ since are generated from the target with a wrong yield: too many
  protons->Draw("sames");
  
  for ( unsigned int el=0; el<elements.size(); el++ )
    draw_line(elements[el],4,1E3);

  for ( unsigned int el=0; el<quads.size(); el++ )
    draw_line(quads[el],2,1E3);

  leg1->Draw();
  
  //////////////
  
  c1->SaveAs("radius.png");
  c1->SaveAs("radius.pdf");
  system("eog radius.png &");
  
  //////////////////////////////////////

  TCanvas *c2 = new TCanvas("c2","histograms",1600,900);
  gStyle->SetOptStat(1101);
  c2->Divide(3,2);
  
  for ( int i=0; i<bins; i++){

    c2->cd(1);
    Pz[i]->Draw();
    
    c2->cd(4);
    PzvsR[i]->Draw("colz"); 

    c2->cd(2);
    Pt[i]->Draw();

    c2->cd(5);
    PtvsR[i]->Draw("colz");

    c2->cd(3);
    Y[i]->Draw();
    
    c2->cd(6);
    XvsY[i]->Draw("colz");

    c2->SaveAs(Form("PvsR_%02d.png",i));
    c2->SaveAs(Form("PvsR_%02d.pdf",i));
    
  }


  
  
  return 0;

}
