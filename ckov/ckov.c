
////////////////////////////////////////////////////////////////////
//
//  Ckovs:
//   - NPEs vs P
//   - NPEs vs TOF01
//   - NPEs vs beta*gamma
//   - PE spectra
//
//  p.franchini@imperial.ac.uk 
//  https://github.com/pfranchini/MICE-various-code
// 
////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>

#include "TCanvas.h"
#include "TH2F.h"
#include "TF1.h"
#include "TGraph.h"
#include "TStyle.h"
#include "TColor.h"

#include "src/common_cpp/JsonCppStreamer/IRStream.hh"
#include "src/common_cpp/DataStructure/Spill.hh"
#include "src/common_cpp/DataStructure/Data.hh"
#include "src/common_cpp/DataStructure/ReconEvent.hh"
#include "src/common_cpp/DataStructure/TOFEvent.hh"
#include "src/common_cpp/DataStructure/SciFiEvent.hh"
#include "src/common_cpp/DataStructure/SciFiSpacePoint.hh"
#include "src/common_cpp/DataStructure/ThreeVector.hh"

////////////////////////////////////////////////////////////////////////

Int_t spills = 4000; //4000;          // max number of spills processed per run
Int_t maxP = 400;           // max momentum in the activation plots (x)
Int_t minNPE = 1; //3;           // min NPEs in the 2D activation plots (y)
Int_t maxNPE = 25;          // max NPEs in the activation plots (y)
Int_t minTOF01 = 24.0;      // min TOF01
Int_t maxTOF01 = 30; //28.0;      // max TOF01
Int_t minbetagamma = 0;
Int_t maxbetagamma = 4.5;
Int_t nbins = 50;           // number of bins in the activation plots
Float_t field = false;      // Field ON or field OFF runs

////////////////////////////////////////////////////////////////////////

// NPE
TH1D* pe_muon_a = new TH1D("pe_muon_a","PE spectra - CkovA", 30, 0, 30);
TH1D* pe_pion_a = new TH1D("pe_pion_a","PE spectra - CkovA", 30, 0, 30);
TH1D* pe_muon_b = new TH1D("pe_muon_b","PE spectra - CkovB", 30, 0, 30);
TH1D* pe_pion_b = new TH1D("pe_pion_b","PE spectra - CkovB", 30, 0, 30);
TH1D* pe_muon_a_under = new TH1D("pe_muon_a_under","PE spectra - CkovA", 30, 0, 30);
TH1D* pe_pion_a_under = new TH1D("pe_pion_a_under","PE spectra - CkovA", 30, 0, 30);
TH1D* pe_muon_b_under = new TH1D("pe_muon_b_under","PE spectra - CkovB", 30, 0, 30);
TH1D* pe_pion_b_under = new TH1D("pe_pion_b_under","PE spectra - CkovB", 30, 0, 30);
TH1D* pe_electron = new TH1D("pe_electron","PE for electrons", 100, 0, 50);

// NPE vs P
TH2D* activation_muon_a = new TH2D("activation_muon_a", "NPE vs P CkovA", nbins, 0, maxP, maxNPE, 0, maxNPE);
TH2D* activation_muon_b = new TH2D("activation_muon_b", "NPE vs P CkovB", nbins, 0, maxP, maxNPE, 0, maxNPE);
TH2D* activation_pion_a = new TH2D("activation_pion_a", "NPE vs P CkovA", nbins, 0, maxP, maxNPE, 0, maxNPE);
TH2D* activation_pion_b = new TH2D("activation_pion_b", "NPE vs P CkovB", nbins, 0, maxP, maxNPE, 0, maxNPE);
TH2D* activation_electron_a = new TH2D("activation_electron_a", "NPE vs TOF01 CkovA", nbins, 24, 26, 100, 0, maxNPE);
TH2D* activation_electron_b = new TH2D("activation_electron_b", "NPE vs TOF01 CkovB", nbins, 24, 26, 100, 0, maxNPE);

TH1D* activation_muon_histo_a = new TH1D("activation_muon_histo_a", "Muons: NPE vs P - CkovA", nbins, 0, maxP);
TH1D* activation_muon_histo_b = new TH1D("activation_muon_histo_b", "Muons: NPE vs P - CkovB", nbins, 0, maxP);
TH1D* activation_pion_histo_a = new TH1D("activation_pion_histo_a", "Pions: NPE vs P - CkovA", nbins, 0, maxP);
TH1D* activation_pion_histo_b = new TH1D("activation_pion_histo_b", "Pions: NPE vs P - CkovB", nbins, 0, maxP);
TH1D* activation_electron_histo_a = new TH1D("activation_electron_histo_a", "Electrons: NPE vs TOF01 - CkovA", nbins, 24, 26);
TH1D* activation_electron_histo_b = new TH1D("activation_electron_histo_b", "Electrons: NPE vs TOF01 - CkovB", nbins, 24, 26);

TH2D* activation_muon_a_norm = new TH2D("activation_muon_a_norm", "Muons: NPE vs P - CkovA", nbins, 0, maxP, maxNPE, 0, maxNPE);
TH2D* activation_muon_b_norm = new TH2D("activation_muon_b_norm", "Muons: NPE vs P - CkovB", nbins, 0, maxP, maxNPE, 0, maxNPE);
TH2D* activation_pion_a_norm = new TH2D("activation_pion_a_norm", "Pions: NPE vs P - CkovA", nbins, 0, maxP, maxNPE, 0, maxNPE);
TH2D* activation_pion_b_norm = new TH2D("activation_pion_b_norm", "Pions: NPE vs P - CkovB", nbins, 0, maxP, maxNPE, 0, maxNPE);

// NPE vs TOF01
TH2D* velocity_a = new TH2D("velocity_a", "NPE vs TOF01 - CkovA", nbins, minTOF01, maxTOF01, maxNPE, 0, maxNPE);
TH2D* velocity_b = new TH2D("velocity_b", "NPE vs TOF01 - CkovB", nbins, minTOF01, maxTOF01, maxNPE, 0, maxNPE);
TH2D* velocity_a_norm = new TH2D("velocity_a_norm", "NPE vs TOF01 - CkovA", nbins, minTOF01, maxTOF01, maxNPE, 0, maxNPE);
TH2D* velocity_b_norm = new TH2D("velocity_b_norm", "NPE vs TOF01 - CkovB", nbins, minTOF01, maxTOF01, maxNPE, 0, maxNPE);
TH1D* velocity_histo_a = new TH1D("velocity_histo_a", "NPE vs TOF01 - CkovA", nbins, minTOF01, maxTOF01);
TH1D* velocity_histo_b = new TH1D("velocity_histo_b", "NPE vs TOF01 - CkovB", nbins, minTOF01, maxTOF01);

// NPE vs beta*gamma
TH2D* betagamma_a = new TH2D("betagamma_a", "NPE vs #beta#gamma - CkovA", nbins, minbetagamma, maxbetagamma, maxNPE, 0, maxNPE);
TH2D* betagamma_b = new TH2D("betagamma_b", "NPE vs #beta#gamma - CkovB", nbins, minbetagamma, maxbetagamma, maxNPE, 0, maxNPE);
TH2D* betagamma_a_norm = new TH2D("betagamma_a_norm", "NPE vs #beta#gamma - CkovA", nbins, minbetagamma, maxbetagamma, maxNPE, 0, maxNPE);
TH2D* betagamma_b_norm = new TH2D("betagamma_b_norm", "NPE vs #beta#gamma - CkovB", nbins, minbetagamma, maxbetagamma, maxNPE, 0, maxNPE);
TH1D* betagamma_histo_a = new TH1D("betagamma_histo_a", "NPE vs #beta#gamma - CkovA", nbins, minbetagamma, maxbetagamma);
TH1D* betagamma_histo_b = new TH1D("betagamma_histo_b", "NPE vs #beta#gamma - CkovB", nbins, minbetagamma, maxbetagamma);

// P
TH1D* muons = new TH1D("muons","muons spectrum", nbins, 0, maxP);
TH1D* pions = new TH1D("pions","pions spectrum", nbins, 0, maxP);

TH2D* tof0_slabs = new TH2D("tof0_slabs", "TOF0", 10, 0, 9, 10, 0, 9);

TCanvas *canvas4 = new TCanvas("canvas4","2x2 canvas",1400,1200);
TCanvas *canvas2 = new TCanvas("canvas2","2x1 canvas",2800,1200);
TCanvas *canvas2b = new TCanvas("canvas2b","2x1 canvas",1400,600);

TFile *output = new TFile("ckov.root","RECREATE");

void process_run( std::string filename, Double_t t_a, Double_t t_b, Double_t t_c, Float_t fieldon, double n0_a, double n0_b, double t_e ) {
  // t_a: lower bound for muons
  // t_b: upper bound for muons
  // t_c: lower bound for pions

  //  TH2D* velocity_a = new TH2D("velocity_a", "NPE vs TOF01 - CkovA", nbins, minTOF01, maxTOF01, maxNPE, 0, maxNPE);
  //  TH2D* velocity_b = new TH2D("velocity_b", "NPE vs TOF01 - CkovB", nbins, minTOF01, maxTOF01, maxNPE, 0, maxNPE);

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

  double NPEA, NPEB;
  Double_t dt01, m, P;
  Int_t nspill = 0;
  unsigned int track;
  Double_t m_muon = 105.6583745; // [MeV/c2]
  Double_t m_pion = 139.57018; // [MeV/c2]
  Double_t m_electron = 0.5109989461; // [MeV/c2]
  Double_t c = 299792458.; // [m/s]
  Double_t tof0 = 5287.24720607/1000.;  // as in the gdml file [mm]
  Double_t tof1 = 12929.5636425/1000.;  // as in the gdml file [mm]
  Int_t number_of_pions = 0;
  Int_t number_of_muons = 0;

  TH1D* tof01 = new TH1D("tof01","TOF01 [ns]", 200, minTOF01, maxTOF01);

  /*  delete pe_pion_a;
  delete pe_pion_b;
  delete pe_muon_a;
  delete pe_muon_b;
  */

  // Loop over all spills 
  while ( (infile >> readEvent != NULL) ) {
    
    infile >> branchName("data") >> data;
    spill = data.GetSpill();
    
    if (spill == NULL || !(spill->GetDaqEventType() == "physics_event")) continue;
    
    nspill++;
    if (nspill>spills) break;
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

      if (fieldon) {
	//// Tracker
	scifi_events = revts->at(j)->GetSciFiEvent(); // Pull out scifi event
	scifi_tracks = scifi_events->scifitracks(); // Pull out tracks  
      }

      //// CKov
      ckov_events = revts->at(j)->GetCkovEvent(); // Pull out ckovs event
      ckovA = ckov_events->GetCkovDigitArrayElement(0).GetCkovA();
      ckovB = ckov_events->GetCkovDigitArrayElement(0).GetCkovB();
      
      if (fieldon) {
	track=0;
	// Loop over _tracks_ in each event
	//std::cout << scifi_tracks.size() << std::endl;
	for (unsigned int k = 0; k < scifi_tracks.size(); ++k) {
	  trackpoints = scifi_tracks[k]->scifitrackpoints();  
	  for (unsigned int l = 0; l < trackpoints.size(); ++l) {
	    // CUT: only in TKU - Station 5    
	    if (trackpoints[l]->tracker()==0 && trackpoints[l]->station()==5)
	      track=l;
	  }
	}
	if (track==0)
	  continue;
      }
      
      m = 0;

      NPEA = 0;
      NPEB = 0;
      NPEA = ckovA.GetNumberOfPes() - n0_a;
      NPEB = ckovB.GetNumberOfPes() - n0_b;

      // CUT: CkovB should have more PEs than CkovA
      if (NPEA>NPEB) continue;

      // Velocity
      velocity_a->Fill(dt01,NPEA);
      velocity_b->Fill(dt01,NPEB);
      
      betagamma_a->Fill(sqrt(pow(t_e,2)/(pow(dt01,2)-pow(t_e,2))),NPEA);
      betagamma_b->Fill(sqrt(pow(t_e,2)/(pow(dt01,2)-pow(t_e,2))),NPEB);

      // Tight species selection
      Double_t dt = 0.01;
      if ( (dt01>t_a+dt) && (dt01<t_b-dt) )  // muons
	m=m_muon;
      else if (dt01>t_c+dt)  {// pions 
	m=m_pion;
	//velocity_a->Fill(dt01,NPEA);
	//velocity_b->Fill(dt01,NPEB);
      }
      else if (dt01<t_a-dt*2)  // electrons
	m=m_electron;
      else
      	continue;
      
      tof01->Fill(dt01);

      if (fieldon)
	P = sqrt(pow(trackpoints[track]->mom()[0],2)+pow(trackpoints[track]->mom()[1],2)+pow(trackpoints[track]->mom()[2],2));	// P from Trackers
      else
	P = (m*(tof1-tof0)/c)/sqrt( pow(dt01*1e-9,2) - pow((tof1-tof0)/c,2) );                                                  // P from TOF01
      
      // CUT: only central pixels in TOF0 and TOF1 in order to have co-axial tracks
      //if (tof0_space_points.GetSlabx()<4 || tof0_space_points.GetSlabx()>5 || tof0_space_points.GetSlaby()<4 || tof0_space_points.GetSlaby()>5 ) continue;
      //if (tof1_space_points.GetSlabx()!=3 || tof1_space_points.GetSlaby()!=3) continue;
      tof0_slabs->Fill(tof0_space_points.GetSlabx(),tof0_space_points.GetSlaby());

      // if (NPEA<=2) continue;

      // PE spectra      
      if (m==m_muon) {
	// PEs above the thresold
	if (P>2.67610e+02+50) pe_muon_a->Fill(NPEA);//
	if (P>2.21751e+02+50) pe_muon_b->Fill(NPEB);
	// PEs under the thresold
	if (P<2.67610e+02) pe_muon_a_under->Fill(NPEA);//
	if (P<2.21751e+02) pe_muon_b_under->Fill(NPEB);

	activation_muon_a->Fill(P,NPEA - n0_a);
	activation_muon_b->Fill(P,NPEB - n0_b);
	muons->Fill(P);

	++number_of_pions;
      }
      if (m==m_pion) {
	// PEs above the thresold 
	if (P>3.32162e+02+50) pe_pion_a->Fill(NPEA);//
	if (P>2.93303e+02+50) pe_pion_b->Fill(NPEB);
	// PEs under the thresold
	if (P<3.32162e+02) pe_pion_a_under->Fill(NPEA);//
	if (P<2.93303e+02) pe_pion_b_under->Fill(NPEB);

	activation_pion_a->Fill(P,NPEA);
	activation_pion_b->Fill(P,NPEB);
	pions->Fill(P);

	++number_of_muons;
      }
      if (m==m_electron) {
	pe_electron->Fill(NPEB);

	activation_electron_a->Fill(dt01,NPEA);
	activation_electron_b->Fill(dt01,NPEB);
	// std::cout << m << " dt: " << dt01 << " " << "P: " << P << " - NPEs: " << NPEA << ", " << NPEB << std::endl;	      
      }
    }
  }
  
  //c5->cd();
  //tof0_slabs->Draw("colz");
  //c5->SaveAs("tof0.png");
  //system("eog tof0.png"); 
  //char *key = new char[1];
  //gets(key);

  output->cd();
  muons->Write();
  pions->Write();

  tof01->GetXaxis()->SetRangeUser(25.,25.8);
  tof01->Fit("gaus");
  tof01->Write();

  std::cout << std::endl << "Muons: " << number_of_muons << " - Pions: " << number_of_pions << std::endl;

  // Analyse activation for a single run
  /*
  for ( Int_t bin = 1; bin < activation_muon_a->GetNbinsX(); bin++ ){
    TH1D *prof_velocity_a = velocity_a->ProjectionY("prof_velocity_a",bin,bin+1,"");
    TH1D *prof_velocity_b = velocity_b->ProjectionY("prof_velocity_b",bin,bin+1,"");
    velocity_histo_a->SetBinContent(bin,prof_velocity_a->GetMean());
    velocity_histo_a->SetBinError(bin,prof_velocity_a->GetRMS()/sqrt(prof_velocity_a->GetEntries()));
    velocity_histo_b->SetBinContent(bin,prof_velocity_b->GetMean());
    velocity_histo_b->SetBinError(bin,prof_velocity_b->GetRMS()/sqrt(prof_velocity_b->GetEntries()));
  }
  c6->cd(1);
  velocity_histo_a->GetYaxis()->SetRangeUser(minNPE,10.);
  velocity_histo_a->GetXaxis()->SetRangeUser(29,35);
  velocity_histo_a->Fit("pol0");
  velocity_histo_a->GetXaxis()->SetTitle("TOF01 [ns]");
  velocity_histo_a->GetYaxis()->SetTitle("NPE");
  velocity_histo_a->SetMarkerStyle(10);
  velocity_histo_a->Draw("E1");

  c6->cd(2);
  velocity_histo_b->GetYaxis()->SetRangeUser(minNPE,10.);
  velocity_histo_b->GetXaxis()->SetRangeUser(29,35.);
  velocity_histo_b->Fit("pol0"); 
  velocity_histo_b->GetXaxis()->SetTitle("TOF01 [ns]");
  velocity_histo_b->GetYaxis()->SetTitle("NPE");
  velocity_histo_b->SetMarkerStyle(10);
  velocity_histo_b->Draw("E1");

  velocity_histo_a->Write();
  velocity_histo_b->Write();
  c6->SaveAs("velocity.png");
  system("eog velocity.png");
  */

}  // process_run()


int main(){

  std::cout << "Spills for each file to be processed: " << spills << std::endl;
  std::string filename;

  canvas4->Divide(2,2);
  canvas2->Divide(2,1);
     
  gStyle->SetOptStat(0);

  // 140 MeV/c
  process_run("files/MAUS-v3.3.2/10488_recon.root", 28.0, 31.5, 33.0, 0, 0.922459, 0.90914, 2.53854e+01);

  // 170 MeV/c
  process_run("files/MAUS-v3.3.2/10496_recon.root", 27.5, 30.0, 31.5, 0, 0.952804, 0.940854, 2.53644e+01);
  
  // 200 MeV/c
  process_run("files/MAUS-v3.3.2/10391_recon.root", 27.5, 29.0, 30.0, 0, 0.969069, 0.922955, 2.53314e+01); 
  
  // 240 MeV/c
  process_run("files/MAUS-v3.3.2/10419_recon.root", 26.4, 27.2, 27.7, 0, 1.04217, 1.3788, 2.53400e+01);
  // bad n0 process_run("files/MAUS-v3.3.2/08914_recon.root", 26.4, 27.2, 27.7, 0, 1.0, 999);
  process_run("files/MAUS-v3.3.2/08915_recon.root", 26.4, 27.2, 27.7, 0, 1.11099, 1.57024, 2.53618e+01);
  

  if (field == false){

    // 300 MeV/c
    // bad n0    process_run("files/MAUS-v3.3.2/10304_recon.root", 26.4, 27.1, 27.6, 0, 1.0, 999);
    process_run("files/MAUS-v3.3.2/10221_recon.root", 27.0, 28.3, 29.2, 0, 1.00634, 1.05541, 2.53377e+01);
    process_run("files/MAUS-v3.3.2/10222_recon.root", 27.0, 28.3, 29.2, 0, 1.04102, 1.06716, 2.53445e+01);
    process_run("files/MAUS-v3.3.2/10225_recon.root", 27.0, 28.3, 29.2, 0, 0.968682, 0.97672, 2.53465e+01);
    // 400 MeV/c - 400MeV+pi_pa82
    // ?    process_run("files/MAUS-v3.3.2/08066_recon.root", 25.5, 26.3, 26.5, 0, 1.0, 999); 
    //process_run("files/MAUS-v3.3.2/08190_recon.root", 25.5, 26.3, 26.5, 0, 1.0, 999);  //SSU-E1CE2-NoTrims140A-FC50A
    /*  process_run("files/MAUS-v3.3.2/10215_recon.root", 25.5, 26.3, 26.5, 0, 1.0, 999);
    process_run("files/MAUS-v3.3.2/10518_recon.root", 25.5, 26.3, 26.5, 0, 1.0, 999);
    process_run("files/MAUS-v3.3.2/10519_recon.root", 25.5, 26.3, 26.5, 0, 1.0, 999);
    */
    
    // bad n0: 
    process_run("files/MAUS-v3.3.2/10520_recon.root", 25.5, 26.3, 26.5, 0, 1.0, 1.0, 2.53377e+01);
    
  }
  

  // Histograms from the 2D plots  
  for ( Int_t bin = 1; bin < activation_muon_a->GetNbinsX(); bin++ ){
    TH1D *prof_muon_a = activation_muon_a->ProjectionY("prof_muon_a",bin,bin+1,"");
    TH1D *prof_muon_b = activation_muon_b->ProjectionY("prof_muon_b",bin,bin+1,"");
    TH1D *prof_pion_a = activation_pion_a->ProjectionY("prof_pion_a",bin,bin+1,"");
    TH1D *prof_pion_b = activation_pion_b->ProjectionY("prof_pion_b",bin,bin+1,"");
    TH1D *prof_electron_a = activation_electron_a->ProjectionY("prof_electron_a",bin,bin+1,"");
    TH1D *prof_electron_b = activation_electron_b->ProjectionY("prof_electron_b",bin,bin+1,"");
    TH1D *prof_velocity_a = velocity_a->ProjectionY("prof_velocity_a",bin,bin+1,"");
    TH1D *prof_velocity_b = velocity_b->ProjectionY("prof_velocity_b",bin,bin+1,"");
    TH1D *prof_betagamma_a = betagamma_a->ProjectionY("prof_betagamma_a",bin,bin+1,"");
    TH1D *prof_betagamma_b = betagamma_b->ProjectionY("prof_betagamma_b",bin,bin+1,"");
    
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
    
    // Try fitting each TOF01 bin
    /*
    if (prof_velocity_a->Fit("gaus") == 0) {
      TF1 *gfit_a = (TF1 *)prof_velocity_a->GetFunction("gaus");
      velocity_histo_a->SetBinContent(bin,gfit_a->GetParameter(1));
      velocity_histo_a->SetBinError(bin,gfit_a->GetParameter(2));
    }      
    if (prof_velocity_b->Fit("gaus") == 0) {
      TF1 *gfit_b = (TF1 *)prof_velocity_b->GetFunction("gaus");
      velocity_histo_b->SetBinContent(bin,gfit_b->GetParameter(1));
      velocity_histo_b->SetBinError(bin,gfit_b->GetParameter(2));
    }*/

    // Use the mean for each TOF01 bin
    velocity_histo_a->SetBinContent(bin,prof_velocity_a->GetMean());
    velocity_histo_a->SetBinError(bin,prof_velocity_a->GetRMS()/sqrt(prof_velocity_a->GetEntries()));
    velocity_histo_b->SetBinContent(bin,prof_velocity_b->GetMean());
    velocity_histo_b->SetBinError(bin,prof_velocity_b->GetRMS()/sqrt(prof_velocity_b->GetEntries()));

    // Try fitting each betagamma bin
    prof_betagamma_a->GetXaxis()->SetRangeUser(0,20);
    if (prof_betagamma_a->Fit("gaus") == 0) {
      TF1 *gfit_a = (TF1 *)prof_betagamma_a->GetFunction("gaus");
      betagamma_histo_a->SetBinContent(bin,gfit_a->GetParameter(1));
      betagamma_histo_a->SetBinError(bin,gfit_a->GetParameter(2));
    }      
    prof_betagamma_b->GetXaxis()->SetRangeUser(0,20);
    if (prof_betagamma_b->Fit("gaus") == 0) {
      TF1 *gfit_b = (TF1 *)prof_betagamma_b->GetFunction("gaus");
      betagamma_histo_b->SetBinContent(bin,gfit_b->GetParameter(1));
      betagamma_histo_b->SetBinError(bin,gfit_b->GetParameter(2));
      
    }
    prof_betagamma_a->Write();
    prof_betagamma_b->Write();

    // Use the mean for each betagamma bin
    /*    betagamma_histo_a->SetBinContent(bin,prof_betagamma_a->GetMean());
    betagamma_histo_a->SetBinError(bin,prof_betagamma_a->GetRMS()/sqrt(prof_betagamma_a->GetEntries()));
    betagamma_histo_b->SetBinContent(bin,prof_betagamma_b->GetMean());
    betagamma_histo_b->SetBinError(bin,prof_betagamma_b->GetRMS()/sqrt(prof_betagamma_b->GetEntries()));
    */

  }

  /// Fit functions
  //TF1 *func_muon_a=new TF1("func_muon_a","[0] + [1]*(1-([2]/x)^2)",260.,500.);
  TF1 *func_muon_a=new TF1("func_muon_a","[0] + [1]*x + [2]*x*x",260.,500.);
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
  TF1 *func_pion_b=new TF1("func_pion_b","[0] + [1]*(1-([2]/x)^2)",310.,500.);
  func_pion_b->SetParameters(4.875,14.8,330);
  func_pion_b->SetParLimits(0,2,5);
  func_pion_b->SetParLimits(1,9,16);
  func_pion_b->SetParLimits(2,310,350);

  //  c1->cd(1);
  //  activation_muon_a->Draw("colz");
  //  graph->Draw("sames*");

  //  c1->cd(2);
  //  activation_muon_b->Draw("colz");

  /// Plot
  // 1D histograms with fits for muons and pions
  std::cout << std::endl << "Fit 1D activiation functions..." << std::endl;
  canvas4->cd(1);
  activation_muon_histo_a->GetXaxis()->SetRangeUser(150.,500.);
  activation_muon_histo_a->GetYaxis()->SetRangeUser(0.,18.);
  activation_muon_histo_a->SetMarkerStyle(10);
  activation_muon_histo_a->GetYaxis()->SetTitle("NPE");
  activation_muon_histo_a->GetXaxis()->SetTitle("P [MeV/c]");
  activation_muon_histo_a->Fit("func_muon_a","R");
  activation_muon_histo_a->Draw("E1");
  func_muon_a->Draw("sames");

  canvas4->cd(2);
  activation_muon_histo_b->GetXaxis()->SetRangeUser(150.,500.);
  activation_muon_histo_b->GetYaxis()->SetRangeUser(0.,18.);
  activation_muon_histo_b->SetMarkerStyle(10);
  activation_muon_histo_b->GetYaxis()->SetTitle("NPE");
  activation_muon_histo_b->GetXaxis()->SetTitle("P [MeV/c]");
  activation_muon_histo_b->Fit("func_muon_b","R");
  activation_muon_histo_b->Draw("E1");
  func_muon_b->Draw("sames");


  canvas4->cd(3);
  activation_pion_histo_a->GetXaxis()->SetRangeUser(150.,500.);
  activation_pion_histo_a->GetYaxis()->SetRangeUser(0.,18.);
  activation_pion_histo_a->SetMarkerStyle(10);
  activation_pion_histo_a->GetYaxis()->SetTitle("NPE");
  activation_pion_histo_a->GetXaxis()->SetTitle("P [MeV/c]");
  activation_pion_histo_a->Fit("func_pion_a","R");
  activation_pion_histo_a->Draw("E1");
  func_pion_a->Draw("sames");

  canvas4->cd(4);
  activation_pion_histo_b->GetXaxis()->SetRangeUser(150.,500.);
  activation_pion_histo_b->GetYaxis()->SetRangeUser(0.,18.);
  activation_pion_histo_b->SetMarkerStyle(10);
  activation_pion_histo_b->GetYaxis()->SetTitle("NPE");
  activation_pion_histo_b->GetXaxis()->SetTitle("P [MeV/c]");
  activation_pion_histo_b->Fit("func_pion_b","R");
  activation_pion_histo_b->Draw("E1");
  func_pion_b->Draw("sames");

  canvas4->SaveAs("plot.png");
  canvas4->SaveAs("Ckov_photoelectrons_vs_P.pdf");

  // 1D histograms with fits for electrons
  canvas2->cd(1);
  std::cout << std::endl << "Fit 1D activiation functions for electrons..." << std::endl;
  //  activation_electron_histo_a->GetXaxis()->SetRangeUser(150.,500.);
  activation_electron_histo_a->GetYaxis()->SetRangeUser(15.,22.);
  activation_electron_histo_a->SetMarkerStyle(10);
  activation_electron_histo_a->GetXaxis()->SetTitle("TOF01 [ns]");
  activation_electron_histo_a->Fit("pol0","R");
  activation_electron_histo_a->Draw("E1");

  canvas2->cd(2);
  //  activation_electron_histo_b->GetXaxis()->SetRangeUser(150.,500.);
  activation_electron_histo_b->GetYaxis()->SetRangeUser(15.,22.);
  activation_electron_histo_b->SetMarkerStyle(10);
  activation_electron_histo_b->GetXaxis()->SetTitle("TOF01 [ns]");
  activation_electron_histo_b->Fit("pol0","R");
  activation_electron_histo_b->Draw("E1");
  canvas2->SaveAs("electron.png");

  // 2D histograms with fits for muons and pions  
  // normalise 2D activation plots
  for ( Int_t i = 0; i < activation_muon_a->GetNbinsX(); i++ ) {
    TH1D *prof = activation_muon_a->ProjectionY("prof",i,i,"");
    for ( Int_t j = 0; j < activation_muon_a->GetNbinsY(); j++ ) {
      activation_muon_a_norm->SetBinContent(i,j,activation_muon_a->GetBinContent(i,j)/prof->GetEntries());
    }
  }  
  for ( Int_t i = 0; i < activation_muon_b->GetNbinsX(); i++ ) {
    TH1D *prof = activation_muon_b->ProjectionY("prof",i,i,"");
    for ( Int_t j = 0; j < activation_muon_b->GetNbinsY(); j++ ) {
      activation_muon_b_norm->SetBinContent(i,j,activation_muon_b->GetBinContent(i,j)/prof->GetEntries());
    }
  }  
  for ( Int_t i = 0; i < activation_pion_a->GetNbinsX(); i++ ) {
    TH1D *prof = activation_pion_a->ProjectionY("prof",i,i,"");
    for ( Int_t j = 0; j < activation_pion_a->GetNbinsY(); j++ ) {
      activation_pion_a_norm->SetBinContent(i,j,activation_pion_a->GetBinContent(i,j)/prof->GetEntries());
    }
  }  
  for ( Int_t i = 0; i < activation_pion_b->GetNbinsX(); i++ ) {
    TH1D *prof = activation_pion_b->ProjectionY("prof",i,i,"");
    for ( Int_t j = 0; j < activation_pion_b->GetNbinsY(); j++ ) {
      activation_pion_b_norm->SetBinContent(i,j,activation_pion_b->GetBinContent(i,j)/prof->GetEntries());
    }
  }  
  
  // normalise 2D velocity plots
  for ( Int_t i = 0; i < velocity_a->GetNbinsX(); i++ ) {
    TH1D *prof = velocity_a->ProjectionY("prof",i,i,"");
  for ( Int_t j = 0; j < velocity_a->GetNbinsY(); j++ ) {
    velocity_a_norm->SetBinContent(i,j,velocity_a->GetBinContent(i,j)/prof->GetEntries());
  }
  }  
  for ( Int_t i = 0; i < velocity_b->GetNbinsX(); i++ ) {
    TH1D *prof = velocity_b->ProjectionY("prof",i,i,"");
    for ( Int_t j = 0; j < velocity_b->GetNbinsY(); j++ ) {
      velocity_b_norm->SetBinContent(i,j,velocity_b->GetBinContent(i,j)/prof->GetEntries());
    }
  }  

  // normalise 2D betagamma plots
  for ( Int_t i = 0; i < betagamma_a->GetNbinsX(); i++ ) {
    TH1D *prof = betagamma_a->ProjectionY("prof",i,i,"");
    for ( Int_t j = 0; j < betagamma_a->GetNbinsY(); j++ ) {
      betagamma_a_norm->SetBinContent(i,j,betagamma_a->GetBinContent(i,j)/prof->GetEntries());
    }
  }  
  for ( Int_t i = 0; i < betagamma_b->GetNbinsX(); i++ ) {
    TH1D *prof = betagamma_b->ProjectionY("prof",i,i,"");
    for ( Int_t j = 0; j < betagamma_b->GetNbinsY(); j++ ) {
      betagamma_b_norm->SetBinContent(i,j,betagamma_b->GetBinContent(i,j)/prof->GetEntries());
    }
  }  


  // kBird for ROOT 5
  Double_t red[9]   = { 0.2082, 0.0592, 0.0780, 0.0232, 0.1802, 0.5301, 0.8186, 0.9956, 0.9764};
  Double_t green[9] = { 0.1664, 0.3599, 0.5041, 0.6419, 0.7178, 0.7492, 0.7328, 0.7862, 0.9832};
  Double_t blue[9]  = { 0.5293, 0.8684, 0.8385, 0.7914, 0.6425, 0.4662, 0.3499, 0.1968, 0.0539};
  Double_t stops[9] = { 0.0000, 0.1250, 0.2500, 0.3750, 0.5000, 0.6250, 0.7500, 0.8750, 1.0000};
  Int_t nb=255;
  TColor::CreateGradientColorTable(9, stops, red, green, blue, nb);
  //canvas4->SetFrameFillColor(31);
  //canvas2->SetFrameFillColor(41);
  //  gStyle->SetCanvasColor(3);

  // plot activation 2d plots
  canvas4->cd(1);
  std::cout << std::endl << "Fit 2D activiation functions..." << std::endl;
  activation_muon_a_norm->GetXaxis()->SetRangeUser(150.,500.);
  activation_muon_a_norm->GetYaxis()->SetRangeUser(minNPE,18.);
  activation_muon_a_norm->GetXaxis()->SetTitle("P [MeV/c]");
  //activation_muon_a_norm->Fit("func_muon_a","R");
  activation_muon_a_norm->Fit("pol3","R");
  activation_muon_a_norm->Draw("colz");

  canvas4->cd(2);
  activation_muon_b_norm->GetXaxis()->SetRangeUser(150.,500.);
  activation_muon_b_norm->GetYaxis()->SetRangeUser(minNPE,18.);
  activation_muon_b_norm->GetXaxis()->SetTitle("P [MeV/c]");
  //activation_muon_b_norm->Fit("func_muon_a","R");
  activation_muon_b_norm->Fit("pol3","R");
  activation_muon_b_norm->Draw("colz");

  canvas4->cd(3);
  activation_pion_a_norm->GetXaxis()->SetRangeUser(150.,500.);
  activation_pion_a_norm->GetYaxis()->SetRangeUser(minNPE,14.);
  activation_pion_a_norm->GetXaxis()->SetTitle("P [MeV/c]");
  //activation_pion_a_norm->Fit("func_pion_a","R");
  activation_pion_a_norm->Fit("pol3","R");
  activation_pion_a_norm->Draw("colz");

  canvas4->cd(4);
  activation_pion_b_norm->GetXaxis()->SetRangeUser(150.,500.);
  activation_pion_b_norm->GetYaxis()->SetRangeUser(minNPE,14.);
  activation_pion_b_norm->GetXaxis()->SetTitle("P [MeV/c]");
  //activation_pion_b_norm->Fit("func_pion_b","R");
  activation_pion_b_norm->Fit("pol3","R");
  activation_pion_b_norm->Draw("colz");
  
  canvas4->SaveAs("scatter_activation.png");
  canvas4->SaveAs("scatter_activation.pdf");

  // plot velocity 1d plots
  canvas2->cd(1);
  velocity_histo_a->GetYaxis()->SetRangeUser(minNPE,24.);
  velocity_histo_a->GetXaxis()->SetTitle("TOF01 [ns]");
  velocity_histo_a->GetYaxis()->SetTitle("NPE");
  //  velocity_histo_a->SetTitle("NPE vs TOF01 - CkovA");
  velocity_histo_a->SetMarkerStyle(10);
  //  velocity_histo_a->Fit("func_a","R");
  velocity_histo_a->Draw("E1");
  
  canvas2->cd(2);
  velocity_histo_b->GetYaxis()->SetRangeUser(minNPE,24.);
  velocity_histo_b->GetXaxis()->SetTitle("TOF01 [ns]");
  velocity_histo_b->GetYaxis()->SetTitle("NPE");
  //velocity_histo_b->SetTitle("NPE vs TOF01 - CkovB");
  velocity_histo_b->SetMarkerStyle(10);
  //  velocity_histo_b->Fit("func_a","R");
  velocity_histo_b->Draw("E1");
  gPad->Modified();
  gPad->Update();
  canvas2->SaveAs("velocity.png");
  system("eog velocity.png &");
  
  // velocity 2D plots
  canvas2->cd(1);
  velocity_a_norm->GetXaxis()->SetRangeUser(24.8,maxTOF01);
  velocity_a_norm->GetYaxis()->SetRangeUser(minNPE,24.);
  velocity_a_norm->GetXaxis()->SetTitle("TOF01 [ns]");
  velocity_a_norm->GetYaxis()->SetTitle("NPE");
  velocity_a_norm->Draw("colz");
  canvas2->cd(2);
  velocity_b_norm->GetXaxis()->SetRangeUser(24.8,maxTOF01);
  velocity_b_norm->GetYaxis()->SetRangeUser(minNPE,24.);
  velocity_b_norm->GetXaxis()->SetTitle("TOF01 [ns]");
  velocity_b_norm->GetYaxis()->SetTitle("NPE");
  velocity_b_norm->Draw("colz");
  gPad->Modified();
  gPad->Update();
  canvas2->SaveAs("scatter_velocity.png");


  // plot betagamma 1d plots
  canvas2->cd(1);
  betagamma_histo_a->GetYaxis()->SetRangeUser(minNPE,24.);
  betagamma_histo_a->GetXaxis()->SetTitle("#beta#gamma");
  betagamma_histo_a->GetYaxis()->SetTitle("NPE");
  betagamma_histo_a->SetMarkerStyle(10);
  betagamma_histo_a->Draw("E1");
  canvas2->cd(2);
  betagamma_histo_b->GetYaxis()->SetRangeUser(minNPE,24.);
  betagamma_histo_b->GetXaxis()->SetTitle("#beta#gamma");
  betagamma_histo_b->GetYaxis()->SetTitle("NPE");
  betagamma_histo_b->SetMarkerStyle(10);
  betagamma_histo_b->Draw("E1");
  canvas2->SaveAs("betagamma.png");


  // betagamma 2D plots
  canvas2->cd(1);
  betagamma_a_norm->GetYaxis()->SetRangeUser(minNPE,24.);
  betagamma_a_norm->GetXaxis()->SetTitle("#beta#gamma");
  betagamma_a_norm->GetYaxis()->SetTitle("NPE");
  betagamma_a_norm->Draw("colz");
  canvas2->cd(2);
  betagamma_b_norm->GetYaxis()->SetRangeUser(minNPE,24.);
  betagamma_b_norm->GetXaxis()->SetTitle("#beta#gamma");
  betagamma_b_norm->GetYaxis()->SetTitle("NPE");
  betagamma_b_norm->Draw("colz");
  gPad->Modified();
  gPad->Update();
  canvas2->SaveAs("scatter_betagamma.png");
  canvas2->SaveAs("scatter_betagamma.pdf");

  /*
  canvas2->cd(1);
  gPad->SetLogy();
  pe_muon_a->GetXaxis()->SetTitle("NPE");
  pe_pion_a->GetXaxis()->SetTitle("NPE");
  pe_pion_a_under->GetXaxis()->SetTitle("NPE");
  pe_pion_a_under->GetXaxis()->SetTitle("NPE");
  pe_muon_a->SetNormFactor();
  pe_pion_a->SetNormFactor();
  pe_muon_a_under->SetNormFactor();
  pe_pion_a_under->SetNormFactor();
  pe_muon_a->SetLineColor(kGreen);
  pe_pion_a->SetLineColor(kBlue);
  pe_muon_a_under->SetLineColor(kGreen);
  pe_pion_a_under->SetLineColor(kBlue);
  pe_muon_a->SetLineWidth(3);
  pe_pion_a->SetLineWidth(3);
  pe_muon_a_under->SetLineWidth(3);
  pe_pion_a_under->SetLineWidth(3);
  pe_muon_a_under->SetLineStyle(7);
  pe_pion_a_under->SetLineStyle(7);
  pe_muon_a->Draw();
  pe_pion_a->Draw("sames");
  pe_muon_a_under->Draw("sames");
  pe_pion_a_under->Draw("sames");

  canvas2->cd(2);
  gPad->SetLogy();
  pe_muon_b->GetXaxis()->SetTitle("NPE");
  pe_pion_b->GetXaxis()->SetTitle("NPE");
  pe_muon_b_under->GetXaxis()->SetTitle("NPE");
  pe_pion_b_under->GetXaxis()->SetTitle("NPE");
  pe_muon_b->SetNormFactor();
  pe_pion_b->SetNormFactor();
  pe_muon_b_under->SetNormFactor();
  pe_pion_b_under->SetNormFactor();
  pe_muon_b->SetLineColor(kGreen);
  pe_pion_b->SetLineColor(kBlue);
  pe_muon_b_under->SetLineColor(kGreen);
  pe_pion_b_under->SetLineColor(kBlue);
  pe_muon_b->SetLineWidth(3);
  pe_pion_b->SetLineWidth(3);
  pe_muon_b_under->SetLineWidth(3);
  pe_pion_b_under->SetLineWidth(3);
  pe_muon_b_under->SetLineStyle(7);
  pe_pion_b_under->SetLineStyle(7);
  pe_muon_b->Draw();
  pe_pion_b->Draw("sames");
  pe_muon_b_under->Draw("sames");
  pe_pion_b_under->Draw("sames");

  //  canvas2-->SaveAs("npe.png");
  //  canvas2-->SaveAs("Ckov_photoelectrons_spectra.pdf");

  */

  /// Save on root file
  activation_muon_a_norm->Write();
  activation_muon_b_norm->Write();
  activation_pion_a_norm->Write();
  activation_pion_b_norm->Write();
  velocity_a_norm->Write();
  velocity_b_norm->Write();
  betagamma_histo_a->Write();
  betagamma_histo_b->Write();
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
  pe_muon_b_under->Write();
  pe_pion_b_under->Write();

  output->Write();

}

