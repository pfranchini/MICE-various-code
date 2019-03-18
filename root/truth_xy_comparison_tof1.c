// Delta t

{
  // Load the MAUS data structure                                                                                                                                                                                                       
  maus_root_dir = TString(gSystem->Getenv("MAUS_ROOT_DIR"));
  gSystem->Load(maus_root_dir + "/build/libMausCpp.so"); // Load the MAUS datastructure                                                                                                                                                                                                                                                                                                                                                                                               
  ifstream is;


  // INPUT:

  //is.open("/home/phrnat/MAUS/maus-pionrun/bin/tuned-current-settings/260/7-tof0.txt");
  //is.open("/home/phrnat/MAUS/maus-pionrun/bin/batch/output_1/7-tof0-1.txt");
  
  is.open("7469/8-tof1.txt");  //
  //is.open("260/batch/output_0/8-tof1.txt");  //   
    
  //TFile f1("maus_output_260-xlong_OFFSET.root"); // Reco MC 260/245`
  
  TFile f1("~/maus-2.1.0/7469/07469_recon.root"); // 

  //  TFile f1("/home/phswbb/maus-2.1.0/7469/07469_recon.root"); 
  
  //TFile f1("07724_recon.root"); // 310/230
  //TFile f1("07705_recon.root"); // 360/230
  //TFile f1("07725_recon.root"); // 410/245
  //TFile f1("07707_recon.root"); // 460/220


  // MC - G4BL

  TH2F *xy_MC    = new TH2F("xy_MC","x vs y",10,-200,200,10,-200,200);
  TH2F *xy_data  = new TH2F("xy_data","x vs y",10,-200,200,10,-200,200);

  TH1F *x_MC   = new TH1F("x_MC", "x TOF1", 7, -200, 200);
  TH1F *x_data = new TH1F("x_data", "x TOF1", 7, -200, 200);
  x_MC->GetXaxis()->SetTitle("x TOF1 (mm)");
  x_data->GetXaxis()->SetTitle("x TOF1 (mm)");

  TH1F *y_MC   = new TH1F("y_MC", "y TOF1", 7, -200, 200);
  TH1F *y_data = new TH1F("y_data", "y TOF1", 7, -200, 200);
  y_MC->GetXaxis()->SetTitle("y TOF1 (mm)");
  y_data->GetXaxis()->SetTitle("y TOF1 (mm)");

  TNtuple *ntuple1 = new TNtuple("ntuple1","data from ascii file","x:y:Px:Py:Pz:PDGid");

  Float_t x, y,z, Px, Py, Pz, t, EventID, TrackID, ParentID, Weight, Bx, By, Bz, Ex, Ey, Ez, ProperTime, PathLength, PolX, PolY, PolZ, InitX, InitY, InitZ, InitT, InitKE;
  Int_t PDGid;
  string dummy;
  
  getline(is,dummy);
  getline(is,dummy);
  while (is.good())
    {
      is >> x >> y >> z >> Px >> Py >> Pz >> t >> PDGid >> EventID >> TrackID >> ParentID >> Weight >> Bx >> By >> Bz >> Ex >> Ey >> Ez >> ProperTime >> PathLength >> PolX >> PolY >> PolZ >> InitX >> InitY >> InitZ >> InitT >> InitKE;
      ntuple1->Fill(x,y,Px,Py,Pz,PDGid);
    }
  is.close();
  

  TCanvas *c1 = new TCanvas("c1", "G4BL simulation", 900 , 700);

  //gStyle->SetOptStat(0);

  TLegend* leg = new TLegend(0.7,0.7,0.9,0.85);

  TCut cut1 = "sqrt(Px*Px)<0.10*abs(Pz)";

  //ntuple1->Draw("x:y >> xy_MC",cut1);
  //xy_MC->Draw("colz");
  //c1->SaveAs("7469_xy_MC.png");

  ntuple1->Draw("x >> x_MC",cut1);
  ntuple1->Draw("y >> y_MC",cut1);



  
  // DATA
  
  // Set up the ROOT file and data pointer                                                                                                                                                                                                 
  TTree *T = f1.Get("Spill");   // Pull out the TTree                                                                                                                                                                                      
  MAUS::Data *data_ptr; // A variable to store the Data from each spill                                                                                                                                                                     
  T->SetBranchAddress("data", &data_ptr);  // Set the address of data_ptr                                                                                                                                                                    
  TH1F *tof11_data = new TH1F("tof11_data", "TOF1-1 #Deltat", 100, 25., 35.);
  tof11_data->GetXaxis()->SetTitle("#Deltat TOF1-1 (ns)");
  
  //T->Draw("data._spill._recon->_tof_event->_tof_space_points._tof1._global_x:data._spill._recon->_tof_event->_tof_space_points._tof1._global_y >> xy_data");
  //xy_data->Draw("colz");
  //c1->SaveAs("7469_xy_data.png");

  T->Draw("data._spill._recon->_tof_event->_tof_space_points._tof1._global_x >> x_data");
  T->Draw("data._spill._recon->_tof_event->_tof_space_points._tof1._global_y >> y_data");
  
  x_MC.SetNormFactor();
  x_MC->SetLineColor(kRed);
  x_data.SetNormFactor();
  x_MC->Draw();
  x_data->Draw("same");
  c1->SaveAs("7469_truth_x_tof1.png");

  y_MC.SetNormFactor();
  y_MC->SetLineColor(kRed);
  y_data.SetNormFactor();
  y_MC->Draw();
  y_data->Draw("same");
  c1->SaveAs("7469_truth_y_tof1.png");


}

/*
  TH1F *slabX_data = new TH1F("slabX_data", "x TOF0", 10, 0, 9);
  TH1F *slabY_data = new TH1F("slabY_data", "y TOF0", 10, 0, 9);

  T->Draw("data._spill._recon->_tof_event->_tof_space_points._tof0._slabX >> slabX_data");
  slabX_data.Draw();
  c1->SaveAs("7469_truth_slabX_tof0.png");

  T->Draw("data._spill._recon->_tof_event->_tof_space_points._tof0._slabY >> slabY_data");
  slabY_data.Draw();
  c1->SaveAs("7469_truth_slabY_tof0.png");


*/




  Double_t norm;

  norm = pion_deltat->Integral() + muon_deltat->Integral();

  tof01_data->Scale(norm/tof01_data->Integral());
  tof01_data->SetMarkerStyle(2);
  tof01_data->Draw("P");

  //muon_deltat->Scale(1/norm);
  muon_deltat->SetLineColor(kRed);
  muon_deltat->Draw("same");
  
  char label[10];
  sprintf(label, "Muons: %.f", tof01_data->GetEntries());
  leg->AddEntry(tof01_data, label, "lep");

  leg->AddEntry(muon_deltat, "Muons", "l");
  leg->AddEntry(pion_deltat, "Pions", "l");

 
  //pion_deltat->Scale(1/norm);
  pion_deltat->Draw("same");
  
  leg->Draw();

  c1->SaveAs("deltat_260_hi-stat.png");
  
  
}
