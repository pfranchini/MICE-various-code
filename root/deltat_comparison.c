// Delta t

{
  // Load the MAUS data structure                                                                                                                                                                                                       
  maus_root_dir = TString(gSystem->Getenv("MAUS_ROOT_DIR"));
  gSystem->Load(maus_root_dir + "/build/libMausCpp.so"); // Load the MAUS datastructure                                                                                                                                                                                                                                                                                                                                                                                               
  ifstream is;


  // INPUT:

  //  mc
  TChain f2("Spill");
  f2.Add("~/maus-2.1.0/260/maus_output_260.root");

  //  data
  TFile f1("07699_recon.root"); // 260/245
  //TFile f1("07724_recon.root"); // 310/230
  //TFile f1("07705_recon.root"); // 360/230
  //TFile f1("07725_recon.root"); // 410/245
  //TFile f1("07707_recon.root"); // 460/220


  TCanvas *c1 = new TCanvas("c1", "G4BL simulation", 900 , 700);
  gStyle->SetOptStat(0);
  TLegend* leg = new TLegend(0.7,0.7,0.9,0.85);

  // MC
  TH1F *tof01_mc = new TH1F("tof01_mc", "TOF0-1 #Deltat", 200, 25., 35.);
  tof01_mc->GetXaxis()->SetTitle("#Deltat TOF0-1 (ns)");
  f2.Draw("data._spill._recon->_tof_event->_tof_space_points._tof1._time - data._spill._recon->_tof_event->_tof_space_points._tof0._time >> tof01_mc");


  // DATA
  TTree *T = f1.Get("Spill");   // Pull out the TTree                                                                                                                                                                                      
  MAUS::Data *data_ptr; // A variable to store the Data from each spill                                                                                                                                                                     
  T->SetBranchAddress("data", &data_ptr);  // Set the address of data_ptr                                                                                                                                                                    
  TH1F *tof01_data = new TH1F("tof01_data", "TOF0-1 #Deltat", 200, 25., 35.);
  tof01_data->GetXaxis()->SetTitle("#Deltat TOF0-1 (ns)");

  T->Draw("data._spill._recon->_tof_event->_tof_space_points._tof1._time - data._spill._recon->_tof_event->_tof_space_points._tof0._time >> tof01_data");
  

  Double_t norm;


  tof01_mc->SetNormFactor();
  tof01_mc->SetLineColor(kRed);
  tof01_mc->Draw();

  tof01_data->SetNormFactor();
  tof01_data->SetMarkerStyle(2);
  tof01_data->Draw("Psame");

  char label[10];
  sprintf(label, "Data: %.f", tof01_data->GetEntries());
  leg->AddEntry(tof01_data, label, "lep");

  sprintf(label, "Reco MC: %.f", tof01_mc->GetEntries());
  leg->AddEntry(tof01_mc, label, "lep");
 
  
  leg->Draw();
  c1->SaveAs("deltat_260_mc-data.png");
  
  
}
