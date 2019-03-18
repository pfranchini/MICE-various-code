// Delta t

{
  // Load the MAUS data structure                                                                                                                                                                                                       
  maus_root_dir = TString(gSystem->Getenv("MAUS_ROOT_DIR"));
  gSystem->Load(maus_root_dir + "/build/libMausCpp.so"); // Load the MAUS datastructure                                                                                                                                                                                                                                                                                                                                                                                               
  ifstream is;


  // INPUT:

  //is.open("/home/phrnat/MAUS/maus-pionrun/bin/tuned-current-settings/260/7-tof0.txt");
  //is.open("/home/phrnat/MAUS/maus-pionrun/bin/batch/output_1/7-tof0-1.txt");
  //  is.open("260-7-tof0-sort.txt");  // 260 hi-stat                                                                                                                                                                                       

  //   MC: 
  //TFile f2("maus_output_260-xlong.root"); // RECO MC from G4BL

  TChain f2("Spill");
  f2.Add("~/maus-2.1.0/7469/maus_output_7469.root");

  //   DATA:                                                                                                                                                                                                                                 
  TFile f1("~/maus-2.1.0/7469/07469_recon.root"); // 260/245    

  //TFile f1("07724_recon.root"); // 310/230                                                                                                                                                                                                 
  //TFile f1("07705_recon.root"); // 360/230                                                                                                                                                                                                 
  //TFile f1("07725_recon.root"); // 410/245                                                                                                                                                                                                 
  //TFile f1("07707_recon.root"); // 460/220      


  TH2F *xy_MC    = new TH2F("xy_MC","x vs y",7,-200,200,10,-200,200);
  TH2F *xy_data  = new TH2F("xy_data","x vs y",7,-200,200,10,-200,200);

  TH1F *x_MC   = new TH1F("x_MC", "x TOF1", 7, -200, 200);
  TH1F *x_data = new TH1F("x_data", "x TOF1", 7, -200, 200);
  x_MC->GetXaxis()->SetTitle("x TOF1 (mm)");
  x_data->GetXaxis()->SetTitle("x TOF1 (mm)");

  TH1F *y_MC   = new TH1F("y_MC", "y TOF1", 7, -200, 200);
  TH1F *y_data = new TH1F("y_data", "y TOF1", 7, -200, 200);
  y_MC->GetXaxis()->SetTitle("y TOF1 (mm)");
  y_data->GetXaxis()->SetTitle("y TOF1 (mm)");

  TH1F *slabX_MC = new TH1F("slabX_MC", "slabX TOF1", 7, 0, 6);
  TH1F *slabY_MC = new TH1F("slabY_MC", "slabY TOF1", 7, 0, 6);
  TH1F *slabX_data = new TH1F("slabX_data", "slabX TOF1", 7, 0, 6);
  TH1F *slabY_data = new TH1F("slabY_data", "slabY TOF1", 7, 0, 6);

  TCanvas *c1 = new TCanvas("c1", "G4BL simulation", 900 , 700);

  //gStyle->SetOptStat(0);

  TLegend* leg = new TLegend(0.7,0.7,0.9,0.85);


  // === MC ===

  //f2.Draw("-(data._spill._recon->_tof_event->_tof_space_points._tof0._global_x):data._spill._recon->_tof_event->_tof_space_points._tof0._global_y >> xy_MC");   
  //xy_MC->Draw("colz");
  //c1->SaveAs("260_xy_MC_tof0.png");

  f2.Draw("data._spill._recon->_tof_event->_tof_space_points._tof1._global_x >> x_MC");
  f2.Draw("data._spill._recon->_tof_event->_tof_space_points._tof1._global_y >> y_MC");
 
  f2.Draw("data._spill._recon->_tof_event->_tof_space_points._tof1._slabX >> slabX_MC");
  f2.Draw("data._spill._recon->_tof_event->_tof_space_points._tof1._slabY >> slabY_MC");

  
  // === DATA ===
  
  // Set up the ROOT file and data pointer                                                                                                                                                                                                 
  TTree *T1 = f1.Get("Spill");   // Pull out the TTree                                                                                                                                                                                      
  MAUS::Data *data_ptr; // A variable to store the Data from each spill                                                                                                                                                                     
  T1->SetBranchAddress("data", &data_ptr);  // Set the address of data_ptr                                                                                                                                                                    
  TH1F *tof11_data = new TH1F("tof11_data", "TOF1-1 #Deltat", 100, 25., 35.);
  tof11_data->GetXaxis()->SetTitle("#Deltat TOF1-1 (ns)");
  
  //T1->Draw("data._spill._recon->_tof_event->_tof_space_points._tof1._global_x:data._spill._recon->_tof_event->_tof_space_points._tof1._global_y >> xy_data");
  //xy_data->Draw("colz");
  //c1->SaveAs("260_xy_data_tof1.png");

  T1->Draw("data._spill._recon->_tof_event->_tof_space_points._tof1._global_x >> x_data");
  T1->Draw("data._spill._recon->_tof_event->_tof_space_points._tof1._global_y >> y_data");
  
  x_MC.SetNormFactor();
  x_MC->SetLineColor(kRed);
  x_data.SetNormFactor();
  x_MC->Draw();
  x_data->Draw("same");
  c1->SaveAs("7469_x_tof1.png");

  y_MC.SetNormFactor();
  y_MC->SetLineColor(kRed);
  y_data.SetNormFactor();
  y_MC->Draw();
  y_data->Draw("same");
  c1->SaveAs("7469_y_tof1.png");

  T1->Draw("data._spill._recon->_tof_event->_tof_space_points._tof1._slabX >> slabX_data");
  T1->Draw("data._spill._recon->_tof_event->_tof_space_points._tof1._slabY >> slabY_data");

  slabX_MC.SetNormFactor();
  slabX_MC->SetLineColor(kRed);
  slabX_data.SetNormFactor();
  slabX_data->Draw();
  slabX_MC->Draw("same");

  c1->SaveAs("7469_slabX_tof1.png");

  slabY_MC.SetNormFactor();
  slabY_MC->SetLineColor(kRed);
  slabY_data.SetNormFactor();
  slabY_data->Draw();
  slabY_MC->Draw("same");
  c1->SaveAs("7469_slabY_tof1.png");





}

