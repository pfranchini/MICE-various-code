
////////////////////////////////////////////////////////////////////
//
//  Produce plots for TOF SP and Tracker Tracks
//  p.franchini@imperial.ac.uk 
// 
////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>

#include "TCanvas.h"
#include "TH2F.h"
//#include "TStyle.h"

#include "src/common_cpp/JsonCppStreamer/IRStream.hh"
#include "src/common_cpp/DataStructure/Spill.hh"
#include "src/common_cpp/DataStructure/Data.hh"
#include "src/common_cpp/DataStructure/ReconEvent.hh"
#include "src/common_cpp/DataStructure/TOFEvent.hh"
#include "src/common_cpp/DataStructure/SciFiEvent.hh"
#include "src/common_cpp/DataStructure/SciFiSpacePoint.hh"
#include "src/common_cpp/DataStructure/ThreeVector.hh"

////////////////////////////////////////////////////////////////////////
Int_t spills = 500; 
////////////////////////////////////////////////////////////////////////

TH2D* TOFsp_vs_Tracks = new TH2D("TOFsp_vs_Tracks", "TOF0 SP vs TKU Tracks", 5, 0, 5, 5, 0, 5);
TH2D* TOFsp_vs_TOF = new TH2D("TOFsp_vs_TOF", "TOF0 SP vs TOF01", 100, 10, 40, 5, 0, 5);
TH2D* Tracks_vs_TOF = new TH2D("Tracks_vs_TOF", "TKU Tracks vs TOF01", 100, 10, 40, 5, 0, 5);
TH2D* TrackPoints_vs_TOF = new TH2D("TrackPoints_vs_TOF", "TKU Track Points vs TOF01", 100, 10, 40, 100, 0, 100);
TH1D* tof01 = new TH1D("tof01","TOF01", 200, 10, 40);
TH1D* tof01_cuts = new TH1D("tof01_cuts","TOF01 [ns]", 200, 10, 40);

TCanvas *c1 = new TCanvas("c1","Canvas",900,900);

void process_run( std::string filename ) {
  
  MAUS::Data data;
  MAUS::Spill* spill;
  
  std::vector<MAUS::ReconEvent*>* revts;
  
  MAUS::TOFEvent* tof_events;
  MAUS::TOFEventSpacePoint tof_space_points;
  MAUS::TOFSpacePoint tof0_space_points, tof1_space_points;

  MAUS::SciFiEvent* scifi_events;
  std::vector<MAUS::SciFiTrack*> scifi_tracks;
  //std::vector<MAUS::SciFiSpacePoint*> scifi_space_points;
  std::vector<MAUS::SciFiTrackPoint*> trackpoints;

  Int_t nspill = 0;
  Float_t dt01;

  std::cout << std::endl << "Processing file: " << filename << std::endl;
  irstream infile(filename.c_str(), "Spill");

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

      // Optional CUT for a single TOF1 SP
      if (tof_space_points.GetTOF1SpacePointArray().size() != 1) continue;

      // If there are SPs fill a T.O.F. plot
      if ((tof_space_points.GetTOF0SpacePointArray().size() != 0) && (tof_space_points.GetTOF1SpacePointArray().size() != 0)) {

	tof0_space_points = tof_space_points.GetTOF0SpacePointArray()[0];
	tof1_space_points = tof_space_points.GetTOF1SpacePointArray()[0];

	// tof01                                                                                                                                                           
        dt01 = tof1_space_points.GetTime()-tof0_space_points.GetTime();
	tof01->Fill(dt01);

      }
      
      //// Tracker
      scifi_events = revts->at(j)->GetSciFiEvent(); // Pull out scifi event
      scifi_tracks = scifi_events->scifitracks(); // Pull out tracks  

      Float_t tku_tracks = 0;
      Float_t tku_trackpoints = 0;

      // Loop over _tracks_ in each event
      //std::cout << scifi_tracks.size() << std::endl;
      for (unsigned int k = 0; k < scifi_tracks.size(); ++k)
	if (scifi_tracks[k]->tracker()==0) {
	  tku_tracks = tku_tracks +1;
	  trackpoints = scifi_tracks[k]->scifitrackpoints();
	  tku_trackpoints = tku_trackpoints + trackpoints.size();
	}
      
      //std::cout << tku_tracks << std::endl;
      TOFsp_vs_Tracks->Fill(tku_tracks, tof_space_points.GetTOF0SpacePointArray().size());
      Tracks_vs_TOF->Fill(dt01, tku_tracks);
      TOFsp_vs_TOF->Fill(dt01, tof_space_points.GetTOF0SpacePointArray().size());
      TrackPoints_vs_TOF->Fill(dt01, tku_trackpoints);

      if ( (tof_space_points.GetTOF0SpacePointArray().size()==1) && (tof_space_points.GetTOF1SpacePointArray().size()==1) && (tku_tracks==1) )
	tof01_cuts->Fill(dt01);

    }
  }
}

int main(){
  
  process_run("~/tmp/3.3.2/07666_recon.root");
  //process_run("~/tmp/3.1.2/07666_recon.root");

  TOFsp_vs_Tracks->GetXaxis()->SetTitle("TKU Tracks");
  TOFsp_vs_Tracks->GetYaxis()->SetTitle("TOF0 SP");

  TrackPoints_vs_TOF->GetXaxis()->SetTitle("TOF01 [ns]");
  TrackPoints_vs_TOF->GetYaxis()->SetTitle("TKU Track Points");

  TOFsp_vs_TOF->GetXaxis()->SetTitle("TOF01 [ns]");
  TOFsp_vs_TOF->GetYaxis()->SetTitle("TOF0 SP");

  Tracks_vs_TOF->GetXaxis()->SetTitle("TOF01 [ns]");
  Tracks_vs_TOF->GetYaxis()->SetTitle("TKU Tracks");

  TOFsp_vs_Tracks->Draw("colz");
  c1->SaveAs("TOFsp_vs_Tracks.png");  

  TrackPoints_vs_TOF->Draw("colz");
  c1->SaveAs("TrackPoints_vs_TOF.png");

  Tracks_vs_TOF->Draw("colz");
  c1->SaveAs("Tracks_vs_TOF.png");

  TOFsp_vs_TOF->Draw("colz");   
  c1->SaveAs("TOFsp_vs_TOF.png");

  tof01->GetXaxis()->SetTitle("TOF01 [ns]");
  tof01_cuts->GetXaxis()->SetTitle("TOF01 [ns]");
  tof01->Draw();
  tof01_cuts->SetLineColor(kRed);
  tof01_cuts->Draw("sames");
  c1->SaveAs("TOF01.png");  
  
  system("eog .");

}

