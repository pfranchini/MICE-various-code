#include <iostream>
#include <string>
#include <vector>

#include "TCanvas.h"
#include "TH2F.h"
#include "TF3.h"
#include "TEllipse.h"

#include "src/common_cpp/JsonCppStreamer/IRStream.hh"
#include "src/common_cpp/DataStructure/Spill.hh"
#include "src/common_cpp/DataStructure/Data.hh"
#include "src/common_cpp/DataStructure/ReconEvent.hh"
#include "src/common_cpp/DataStructure/SciFiEvent.hh"
#include "src/common_cpp/DataStructure/SciFiSpacePoint.hh"
#include "src/common_cpp/DataStructure/ThreeVector.hh"

#include "Fit/BinData.h"
#include "Fit/UnBinData.h"
#include "Fit/Fitter.h"
#include "Math/WrappedMultiTF1.h"

void process_spacepoints( MAUS::ThreeVector* position ){
  
  // create a 2D un-binned data structure for 5 points
  ROOT::Fit::BinData helix(5,2);
  ROOT::Fit::Fitter fitter;
  
  // 3D Fit function
  //  TF3 * f3 = new TF3("f3","[0]*(x**2+y**2) + [1]*x*z + [2]*y*z + [3]*x + [4]*y + [5]*z -1", -100.,100., -100.,100., 0.,1200.);
  TF2 * f2 = new TF2("f2","x**2 + y**2 + [0]*x + [1]*y + [2]", -100.,100., -100.,100.);

  TH2F* circle = new TH2F("circle","circle",100,-100,100,100,-100,100);
  circle->SetMarkerStyle(20);  
  circle->SetMarkerSize(1);

  float c_x = 0;
  float c_y = 0;
  float r = 0;

  // Get the spacepoints

  for(int s = 1; s <= 5; ++s) {
    /*    std::cout << " x: " << position[s].x() << std::endl;
    std::cout << " y: " << position[s].y() << std::endl;
    std::cout << " z: " << position[s].z() << std::endl << std::endl;*/

    c_x = c_x + position[s].x();
    c_y = c_y + position[s].y();
    
    circle->Fill(position[s].x(),position[s].y());
    
    // add the 2D-data coordinate
    helix.Add(position[s].x(),position[s].y());
    
  }
  
  c_x=c_x/5;
  c_y=c_y/5;

  for(int s = 1; s <= 5; ++s) {
    r = r + sqrt( pow(c_x-position[s].x(),2) + pow(c_y-position[s].y(),2) );
  }
  r=r/5;  // average radius

  std::cout << "c_x: " << c_x << std::endl;
  std::cout << "c_y: " << c_y << std::endl;
  std::cout << "r  : " << r   << std::endl;

  TCanvas *c = new TCanvas("c","histograms",500,500); 
  circle->Draw(); 

  TEllipse *el1 = new TEllipse(c_x,c_y,r,r);
  el1->SetFillStyle(0);
  el1->SetLineColor(18);
  el1->Draw("same");

  std::cout << "data points: " << helix.NPoints() << std::endl;

  // Do the fit for this event

  /*  f2->SetParLimits(0,-100,100);
  f2->SetParLimits(1,-100,100);
  f2->SetParLimits(2,-100,100);
  
  f2->SetParameter(0,-2*c_x);
  f2->SetParameter(1,-2*c_y);
  f2->SetParameter(2,c_x*c_x+c_y*c_y-r*r);
  */

  ROOT::Math::WrappedMultiTF1 wf(*f2,2);
  fitter.SetFunction(wf);
  
  fitter.Config().ParSettings(0).SetLimits(-100,100);
  fitter.Config().ParSettings(1).SetLimits(-100,100);
  fitter.Config().ParSettings(2).SetLimits(0,2*(c_x*c_x+c_y*c_y-r*r));

  fitter.Config().ParSettings(0).SetValue(-2*c_x);
  fitter.Config().ParSettings(1).SetValue(-2*c_y);				       
  fitter.Config().ParSettings(2).SetValue(c_x*c_x+c_y*c_y-r*r);

  bool ret = fitter.Fit(helix);
  
  if (ret) {
    const ROOT::Fit::FitResult & res = fitter.Result();
    // print result (should be around 1)
    res.Print(std::cout);

    c_x = -fitter.Result().Parameter(0)/2;
    c_y = -fitter.Result().Parameter(1)/2;
    r = sqrt( c_x*c_x + c_y*c_y - fitter.Result().Parameter(2) );

    std::cout << "c_x: " << c_x << std::endl;
    std::cout << "c_y: " << c_y << std::endl;
    std::cout << "r  : " << r   << std::endl;

    TEllipse *el2 = new TEllipse(c_x,c_y,r,r);
    el2->SetFillStyle(0);
    el2->Draw("same");
    
    /*f2->SetParameter(0,fitter.Result().Parameter(0));
      f2->SetParameter(1,fitter.Result().Parameter(1));
      f2->SetParameter(2,fitter.Result().Parameter(2));
      f2->SetLineWidth(3);
      f2->Draw("same");*/

    c->SaveAs("circle.png");
    system("eog circle.png");

  }


    
}

int main() {
  /////////////////////////////////////////
  //
  //  Get Space Points and process them
  //
  /////////////////////////////////////////

  // Variables
  int station[6];
  int five_stations;
  int good_events;
  MAUS::ThreeVector position[6];
  
  MAUS::Data data;
  MAUS::Spill* spill;

  std::vector<MAUS::ReconEvent*>* revts;
  MAUS::SciFiEvent* sfevt;

  std::vector<MAUS::SciFiSpacePoint*> spacepoints;
  std::vector<MAUS::SciFiHelicalPRTrack*> htrks;

  std::vector<MAUS::SciFiTrack*> track;
  
  std::string filename = "Hunt/10432_recon.root";
  irstream infile(filename.c_str(), "Spill");


  // Loop over all spills   
  while ( (infile >> readEvent != NULL) ) {

    infile >> branchName("data") >> data;
    spill = data.GetSpill();
  
    if (spill == NULL || !(spill->GetDaqEventType() == "physics_event")) continue;

    std::cout << "Spill: " << spill->GetSpillNumber() << "\n";

    revts = spill->GetReconEvents();

    // Loop over recon events in spill                                                  
    for ( size_t j = 0; j < revts->size(); ++j ) {
      if ( !revts->at(j) ) continue;

      // Pull out scifi event  
      sfevt = revts->at(j)->GetSciFiEvent(); 

      // Pull out space points from the event
      spacepoints = sfevt->spacepoints();

      /*
      // Pull out helical PR tracks
      htrks = sfevt->helicalprtracks();

      // Pull out (Kalman??) tracks  
      track = sfevt->scifitracks(); 

      // Pull out 
      spacepoints = htrks->spacepoints();   
      */
      
      for (int l=1; l<=6; ++l) station[l]=0;
      
      // Loop over spacepoints in one event
      for (unsigned int k = 0; k < spacepoints.size(); ++k) {
	
	if (spacepoints[k]->get_tracker() != 0 ) continue;

	position[spacepoints[k]->get_station()] = spacepoints[k]->get_position();
	
	++station[spacepoints[k]->get_station()];
	
      } 
      
      five_stations = station[1]*station[2]*station[3]*station[4]*station[5];
      if (five_stations != 1) continue;

      // Good event: 1 spacepoint (x,y,z) in each of the 5 stations
      std::cout << "Event:   " << spacepoints[0]->get_event() << std::endl;      
      ++good_events;
      
      process_spacepoints(position);

      //      char *key = new char[1];
      //      gets(key); 
      
    }
  }
 

  std::cout << "Good events: " << good_events << std::endl;
  
  return 0;
}

