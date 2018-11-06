
//////////////////
//
//  Ckovs:
//   - NPEs vs P
//   - PE spectra
//
//////////////////


#include <iostream>
#include <string>
#include <vector>

#include "TCanvas.h"
#include "TH2F.h"
#include "TF1.h"
#include "TGraph.h"
#include "TStyle.h"

#include "src/common_cpp/JsonCppStreamer/IRStream.hh"
#include "src/common_cpp/DataStructure/Spill.hh"
#include "src/common_cpp/DataStructure/Data.hh"
#include "src/common_cpp/DataStructure/ReconEvent.hh"
#include "src/common_cpp/DataStructure/TOFEvent.hh"
#include "src/common_cpp/DataStructure/SciFiEvent.hh"
#include "src/common_cpp/DataStructure/SciFiSpacePoint.hh"
#include "src/common_cpp/DataStructure/ThreeVector.hh"

Int_t spills=5000;
Int_t maxP = 500;
Int_t maxNPE = 25;

TH1D* pe_muon_a = new TH1D("pe_muon_a","PE spectra - CkovA", 30, 0, 30);
TH1D* pe_pion_a = new TH1D("pe_pion_a","PE spectra - CkovA", 30, 0, 30);
TH1D* pe_muon_b = new TH1D("pe_muon_b","PE spectra - CkovB", 30, 0, 30);
TH1D* pe_pion_b = new TH1D("pe_pion_b","PE spectra - CkovB", 30, 0, 30);
TH1D* pe_electron = new TH1D("pe_electron","PE for electrons", 100, 0, 50);
TH2D* activation_muon_a = new TH2D("activation_muon_a", "NPE vs P CkovA", 100, 0, maxP, 100, 0, maxNPE);
TH2D* activation_muon_b = new TH2D("activation_muon_b", "NPE vs P CkovB", 100, 0, maxP, 100, 0, maxNPE);
TH2D* activation_pion_a = new TH2D("activation_pion_a", "NPE vs P CkovA", 100, 0, maxP, 100, 0, maxNPE);
TH2D* activation_pion_b = new TH2D("activation_pion_b", "NPE vs P CkovB", 100, 0, maxP, 100, 0, maxNPE);
TH2D* activation_electron_a = new TH2D("activation_electron_a", "NPE vs P CkovA", 100, 24, 26, 100, 0, maxNPE);
TH2D* activation_electron_b = new TH2D("activation_electron_b", "NPE vs P CkovB", 100, 24, 26, 100, 0, maxNPE);

TH1D* muons = new TH1D("muons","muons spectrum", 100, 0, maxP);
TH1D* pions = new TH1D("pions","pions spectrum", 100, 0, maxP);

TH2D* tof0_slabs = new TH2D("tof0_slabs", "TOF0", 10, 0, 9, 10, 0, 9);

TCanvas *c1 = new TCanvas("c1","muons and pions",1400,1200);
TCanvas *c2 = new TCanvas("c2","electrons",1400,600);
TCanvas *c3 = new TCanvas("c3","PEs",1400,600);
TCanvas *c4 = new TCanvas("c4","muons and pions",1400,1200);

void process_run( std::string filename, Double_t t_min, Double_t t_cut, Float_t fieldon ) {

  MAUS::Data data;
  MAUS::Spill* spill;

  std::vector<MAUS::ReconEvent*>* revts;

  MAUS::TOFEvent* tof_events;
  MAUS::TOFEventSpacePoint tof_space_points;
  MAUS::TOFSpacePoint tof0_space_points, tof1_space_points;

  MAUS::SciFiEvent* scifi_events;
  std::vector<MAUS::SciFiTrack*> scifi_tracks;
  //  std::vector<MAUS::SciFiSpacePoint*> scifi_space_points;
  std::vector<MAUS::SciFiTrackPoint*> trackpoints;

  MAUS::CkovEvent* ckov_events;
  MAUS::CkovA ckovA;
  MAUS::CkovB ckovB;

  std::cout << std::endl << "Processing file: " << filename << std::endl;
  irstream infile(filename.c_str(), "Spill");

  Int_t NPEA, NPEB;
  Double_t dt01, m, P2;
  Int_t nspill = 0;
  Double_t m_muon = 105.6583745; // MeV/c2
  Double_t m_pion = 139.57018; // MeV/c2
  Double_t m_electron = 0.5109989461; // MeV/c2
  Double_t c = 299792458; // m/s
  Double_t tof0 = 5287.24720607/1000.;  // as in the gdml file in meters                                                                                                 
  Double_t tof1 = 12929.5636425/1000.;  // as in the gdml file in meters  

  TH1D* tof = new TH1D("tof","TOF01 [ns]", 200, 22, 38);
  
  // Loop over all spills 
  while ( (infile >> readEvent != NULL) ) {
    
    infile >> branchName("data") >> data;
    spill = data.GetSpill();
    
    if (spill == NULL || !(spill->GetDaqEventType() == "physics_event")) continue;
    
    nspill++;
    if (nspill>=spills) break;
    std::cout << "\b\b\b\b\b" << nspill;
    
    revts = spill->GetReconEvents();

    // Loop over recon events in spill  
    for ( size_t j = 0; j < revts->size(); ++j ) {
      if ( !revts->at(j) ) continue;

      //// TOF
      tof_events = revts->at(j)->GetTOFEvent(); // Pull out TOF event
      tof_space_points = tof_events->GetTOFEventSpacePoint();  // Pull out space points

      // CUT: select reco events with only one space point in TOF0 and one in TOF1
      if (tof_space_points.GetTOF0SpacePointArray().size() != 1) continue;
      if (tof_space_points.GetTOF1SpacePointArray().size() != 1) continue;

      tof0_space_points = tof_space_points.GetTOF0SpacePointArray()[0];
      tof1_space_points = tof_space_points.GetTOF1SpacePointArray()[0];
      
      // tof01                                                                                                                                                             
      dt01 = tof1_space_points.GetTime()-tof0_space_points.GetTime();

      //// Tracker
      scifi_events = revts->at(j)->GetSciFiEvent(); // Pull out scifi event
      scifi_tracks = scifi_events->scifitracks(); // Pull out tracks  

      //// CKov
      ckov_events = revts->at(j)->GetCkovEvent(); // Pull out ckovs event
      ckovA = ckov_events->GetCkovDigitArrayElement(0).GetCkovA();
      ckovB = ckov_events->GetCkovDigitArrayElement(0).GetCkovB();
      
      // Loop over _tracks_ in each event
      //      for (unsigned int k = 0; k < scifi_tracks.size(); ++k) {
      //        trackpoints = scifi_tracks[k]->scifitrackpoints();  
	
      //        for (unsigned int l = 0; l < trackpoints.size(); ++l) {
          // TKU - Station 5                                                                                                                                               
      //          if (trackpoints[l]->tracker()==0 && trackpoints[l]->station()==5){
      m = 0;
      NPEA = 0;
      NPEB = 0;

      Double_t dt = 0.05;
      
      if (dt01>t_cut+dt*2)  // pions
	m=m_pion;
      else if ( (dt01>t_min+dt) && (dt01<t_cut-dt) )  // muons
	m=m_muon;
      else if (dt01<t_min)
	m=m_electron;
      else
	continue;
      
      tof->Fill(dt01);
      // P1 = sqrt(pow(trackpoints[l]->mom()[0],2)+pow(trackpoints[l]->mom()[1],2)+pow(trackpoints[l]->mom()[2],2));	    
      P2 = (m*(tof1-tof0)/c)/sqrt( pow(dt01*1e-9,2) - pow((tof1-tof0)/c,2) );
      
      NPEA = ckovA.GetNumberOfPes();
      NPEB = ckovB.GetNumberOfPes();
      if (NPEA>NPEB) continue;
      
      if (tof0_space_points.GetSlabx()<4 || tof0_space_points.GetSlabx()>5 || tof0_space_points.GetSlaby()<4 || tof0_space_points.GetSlaby()>5 ) continue;
      if (tof1_space_points.GetSlabx()!=3 || tof1_space_points.GetSlaby()!=3) continue;
      tof0_slabs->Fill(tof0_space_points.GetSlabx(),tof0_space_points.GetSlaby());

      // if (NPEA<=2) continue;
      
      if (m==m_muon) {
	if (P2>2.67610e+02+50) pe_muon_a->Fill(NPEA);//
	if (P2>2.21751e+02+50) pe_muon_b->Fill(NPEB);
	activation_muon_a->Fill(P2,NPEA);
	activation_muon_b->Fill(P2,NPEB);
	muons->Fill(P2);
      }
      if (m==m_pion) {
	if (P2>3.32162e+02+50) pe_pion_a->Fill(NPEA);//
	if (P2>2.93303e+02+50) pe_pion_b->Fill(NPEB);
	activation_pion_a->Fill(P2,NPEA);
	activation_pion_b->Fill(P2,NPEB);
	pions->Fill(P2);
      }
      if (m==m_electron) {
	pe_electron->Fill(NPEB);
	activation_electron_a->Fill(dt01,NPEA);
	activation_electron_b->Fill(dt01,NPEB);
	// std::cout << m << " dt: " << dt01 << " " << "P: " << P2 << " - NPEs: " << NPEA << ", " << NPEB << std::endl;	      
      }
    }
  }
  
  /*TCanvas *c3 = new TCanvas("c3","histograms",800,600);
  tof0_slabs->Draw("colz");
  c3->SaveAs("tof0.png");
  system("eog tof0.png"); 
  */
  //  char *key = new char[1];
  //  gets(key);

}


int main(){

  std::cout << "Spills for each file to be processed: " << spills << std::endl;
  std::string filename;

  c1->Divide(2,2);
  c2->Divide(2,1);
  c3->Divide(2,1);
  c4->Divide(2,2);
  gStyle->SetOptStat(0);

  // 140 MeV/c
  process_run("files/10488_recon.root", 27.0, 32.5, 1);
  
  // 170 MeV/c
  process_run("files/10496_recon.root", 27.0, 31.0, 1);

  // 200 MeV/c
  process_run("files/10391_recon.root", 27.0, 29.5, 1); 

  // 240 MeV/c
  process_run("files/10419_recon.root", 26.0, 27.5, 1);

  // 300 MeV/c
  process_run("files/10304_recon.root", 26.0, 27.3, 0);
  process_run("files/10221_recon.root", 26.5, 29.0, 0);

  // 400 MeV/c
  process_run("files/10519_recon.root", 25.5, 26.4, 1);

  TH1D* activation_muon_histo_a = new TH1D("activation_muon_histo_a", "Muons: NPE vs P - CkovA", 100, 0, maxP);
  TH1D* activation_muon_histo_b = new TH1D("activation_muon_histo_b", "Muons: NPE vs P - CkovB", 100, 0, maxP);
  TH1D* activation_pion_histo_a = new TH1D("activation_pion_histo_a", "Pions: NPE vs P - CkovA", 100, 0, maxP);
  TH1D* activation_pion_histo_b = new TH1D("activation_pion_histo_b", "Pions: NPE vs P - CkovB", 100, 0, maxP);
  TH1D* activation_electron_histo_a = new TH1D("activation_electron_histo_a", "Electrons: NPE vs P - CkovA", 100, 24, 26);
  TH1D* activation_electron_histo_b = new TH1D("activation_electron_histo_b", "Electrons: NPE vs P - CkovB", 100, 24, 26);
  //  TGraph* graph = new TGraph(activation_muon_a->GetNbinsX());
  //  graph->SetMaximum(maxNPE);
  //  graph->SetTitle("Activation plot muon");
  //  graph->GetXaxis()->SetTitle("P [MeV/c]");
  //  graph->GetYaxis()->SetTitle("NPE");
  for ( Int_t bin = 1; bin < activation_muon_a->GetNbinsX(); bin++ ){
    TH1D *prof_muon_a = activation_muon_a->ProjectionY("prof_muon_a",bin,bin+1,"");
    TH1D *prof_muon_b = activation_muon_b->ProjectionY("prof_muon_b",bin,bin+1,"");
    TH1D *prof_pion_a = activation_pion_a->ProjectionY("prof_pion_a",bin,bin+1,"");
    TH1D *prof_pion_b = activation_pion_b->ProjectionY("prof_pion_b",bin,bin+1,"");
    TH1D *prof_electron_a = activation_electron_a->ProjectionY("prof_electron_a",bin,bin+1,"");
    TH1D *prof_electron_b = activation_electron_b->ProjectionY("prof_electron_b",bin,bin+1,"");
    // graph->SetPoint(bin*maxP/activation_muon_a->GetNbinsX(),bin*maxP/activation_muon_a->GetNbinsX(),prof_a->GetMean())
    activation_muon_histo_a->SetBinContent(bin,prof_muon_a->GetMean());
    activation_muon_histo_a->SetBinError(bin,prof_muon_a->GetRMS()); // /sqrt(prof_muon_a->GetEntries())); // error on the mean: sd/sqrt(n)
    activation_muon_histo_b->SetBinContent(bin,prof_muon_b->GetMean());
    activation_muon_histo_b->SetBinError(bin,prof_muon_b->GetRMS()); // /sqrt(prof_muon_b->GetEntries()));
    activation_pion_histo_a->SetBinContent(bin,prof_pion_a->GetMean());
    activation_pion_histo_a->SetBinError(bin,prof_pion_a->GetRMS()); // /sqrt(prof_pion_a->GetEntries()));
    activation_pion_histo_b->SetBinContent(bin,prof_pion_b->GetMean());
    activation_pion_histo_b->SetBinError(bin,prof_pion_b->GetRMS()); // /sqrt(prof_pion_b->GetEntries()));
    activation_electron_histo_a->SetBinContent(bin,prof_electron_a->GetMean());
    activation_electron_histo_a->SetBinError(bin,prof_electron_a->GetRMS()/sqrt(prof_electron_a->GetEntries()));
    activation_electron_histo_b->SetBinContent(bin,prof_electron_b->GetMean());
    activation_electron_histo_b->SetBinError(bin,prof_electron_b->GetRMS()/sqrt(prof_electron_b->GetEntries()));
  }

  /// Fits
  TF1 *func_muon_a=new TF1("func_muon_a","[0] + [1]*(1-([2]/x)^2)",260.,500.);
  func_muon_a->SetParameters(0.75,12,260);
  func_muon_a->SetParLimits(0,0,1.5);
  func_muon_a->SetParLimits(1,10,18);
  func_muon_a->SetParLimits(2,250,270);
  TF1 *func_muon_b=new TF1("func_muon_b","[0] + [1]*(1-([2]/x)^2)",217.,500.);
  func_muon_b->SetParameters(1.10,18,217);
  func_muon_b->SetParLimits(0,0,2);
  func_muon_b->SetParLimits(1,10,18);
  func_muon_b->SetParLimits(2,200,240);
  TF1 *func_pion_a=new TF1("func_pion_a","[0] + [1]*(1-([2]/x)^2)",320.,500.);
  func_pion_a->SetParameters(0.75,12,368);
  func_pion_a->SetParLimits(0,0,2);
  func_pion_a->SetParLimits(1,2,10);
  func_pion_a->SetParLimits(2,300,350);
  TF1 *func_pion_b=new TF1("func_pion_b","[0] + [1]*(1-([2]/x)^2)",240.,500.);
  func_pion_b->SetParameters(1.10,18,285);
  func_pion_b->SetParLimits(0,0,2);
  func_pion_b->SetParLimits(1,9,16);
  func_pion_b->SetParLimits(2,250,300);

  //  c1->cd(1);
  //  activation_muon_a->Draw("colz");
  //  graph->Draw("sames*");

  //  c1->cd(2);
  //  activation_muon_b->Draw("colz");

  /// Plots
  c1->cd(1);
  activation_muon_histo_a->GetXaxis()->SetRangeUser(150.,500.);
  activation_muon_histo_a->GetYaxis()->SetRangeUser(0.,18.);
  activation_muon_histo_a->SetMarkerStyle(10);
  activation_muon_histo_a->GetXaxis()->SetTitle("P [MeV/c]");
  activation_muon_histo_a->Fit("func_muon_a","R");
  activation_muon_histo_a->Draw("E1");
  func_muon_a->Draw("sames");

  c1->cd(2);
  activation_muon_histo_b->GetXaxis()->SetRangeUser(150.,500.);
  activation_muon_histo_b->GetYaxis()->SetRangeUser(0.,18.);
  activation_muon_histo_b->SetMarkerStyle(10);
  activation_muon_histo_b->GetXaxis()->SetTitle("P [MeV/c]");
  activation_muon_histo_b->Fit("func_muon_b","R");
  activation_muon_histo_b->Draw("E1");
  func_muon_b->Draw("sames");


  c1->cd(3);
  activation_pion_histo_a->GetXaxis()->SetRangeUser(150.,500.);
  activation_pion_histo_a->GetYaxis()->SetRangeUser(0.,18.);
  activation_pion_histo_a->SetMarkerStyle(10);
  activation_pion_histo_a->GetXaxis()->SetTitle("P [MeV/c]");
  activation_pion_histo_a->Fit("func_pion_a","R");
  activation_pion_histo_a->Draw("E1");
  func_pion_a->Draw("sames");

  c1->cd(4);
  activation_pion_histo_b->GetXaxis()->SetRangeUser(150.,500.);
  activation_pion_histo_b->GetYaxis()->SetRangeUser(0.,18.);
  activation_pion_histo_b->SetMarkerStyle(10);
  activation_pion_histo_b->GetXaxis()->SetTitle("P [MeV/c]");
  activation_pion_histo_b->Fit("func_pion_b","R");
  activation_pion_histo_b->Draw("E1");
  func_pion_b->Draw("sames");

  c1->SaveAs("plot.png");
  c1->SaveAs("Ckov_photoelectrons_vs_P.pdf");
  system("eog plot.png &");


  c2->cd(1);
  //  activation_electron_histo_a->GetXaxis()->SetRangeUser(150.,500.);
  activation_electron_histo_a->GetYaxis()->SetRangeUser(15.,18.);
  activation_electron_histo_a->SetMarkerStyle(10);
  activation_electron_histo_a->GetXaxis()->SetTitle("P [MeV/c]");
  activation_electron_histo_a->Fit("pol0","R");
  activation_electron_histo_a->Draw("E1");

  c2->cd(2);
  //  activation_electron_histo_b->GetXaxis()->SetRangeUser(150.,500.);
  activation_electron_histo_b->GetYaxis()->SetRangeUser(18.,22.);
  activation_electron_histo_b->SetMarkerStyle(10);
  activation_electron_histo_b->GetXaxis()->SetTitle("P [MeV/c]");
  activation_electron_histo_b->Fit("pol0","R");
  activation_electron_histo_b->Draw("E1");

  c2->SaveAs("electron.png");
  

  c4->cd(1);
  activation_muon_a->GetXaxis()->SetRangeUser(150.,500.);
  activation_muon_a->GetYaxis()->SetRangeUser(0.,18.);
  activation_muon_a->Fit("func_muon_a","R");
  activation_muon_a->Draw();

  c4->cd(2);
  activation_muon_b->GetXaxis()->SetRangeUser(150.,500.);
  activation_muon_b->GetYaxis()->SetRangeUser(0.,18.);
  activation_muon_b->Fit("func_muon_a","R");
  activation_muon_b->Draw();

  c4->cd(3);
  activation_pion_a->GetXaxis()->SetRangeUser(150.,500.);
  activation_pion_a->GetYaxis()->SetRangeUser(0.,14.);
  activation_pion_a->Fit("func_pion_a","R");
  activation_pion_a->Draw();

  c4->cd(4);
  activation_pion_b->GetXaxis()->SetRangeUser(150.,500.);
  activation_pion_b->GetYaxis()->SetRangeUser(0.,14.);
  activation_pion_b->Fit("func_pion_b","R");
  activation_pion_b->Draw();
  
  c4->SaveAs("scatter.png");


  c3->cd(1);
  gPad->SetLogy();
  pe_muon_a->GetXaxis()->SetTitle("NPE");
  pe_pion_a->GetXaxis()->SetTitle("NPE");
  pe_muon_a->SetNormFactor();
  pe_pion_a->SetNormFactor();
  pe_muon_a->SetLineColor(kGreen);
  pe_pion_a->SetLineColor(kBlue);
  pe_muon_a->SetLineWidth(4);
  pe_pion_a->SetLineWidth(4);
  pe_muon_a->Draw();
  pe_pion_a->Draw("sames");

  c3->cd(2);
  gPad->SetLogy();
  pe_muon_b->GetXaxis()->SetTitle("NPE");
  pe_pion_b->GetXaxis()->SetTitle("NPE");
  pe_muon_b->SetNormFactor();
  pe_pion_b->SetNormFactor();
  pe_muon_b->SetLineColor(kGreen);
  pe_pion_b->SetLineColor(kBlue);
  pe_muon_b->SetLineWidth(4);
  pe_pion_b->SetLineWidth(4);
  pe_muon_b->Draw();
  pe_pion_b->Draw("sames");

  c3->SaveAs("npe.png");
  c3->SaveAs("Ckov_photoelectrons_spectra.pdf");


  /// Save
  TFile *output = new TFile("ckov.root","RECREATE");
  output->cd();
  activation_muon_histo_a->Write();
  activation_muon_histo_b->Write();
  activation_pion_histo_a->Write();
  activation_pion_histo_b->Write();
  activation_electron_histo_a->Write();
  activation_electron_histo_b->Write();
  pe_muon_a->Write();
  pe_pion_a->Write();
  pe_muon_b->Write();
  pe_pion_b->Write();
  muons->Write();
  pions->Write();
  output->Write();

}

