//  Fit of Tom's simulation
//  Plot the fit together with the G4BL generation from Tom's simulation (1-target.txt)


{


  TH1F *pion_ptot = new TH1F("pion_ptot", "G4BL", 200, 0., 800.);
  //  TH1F *pion_nocut = new TH1F("pion_nocut", "MARS", 200, 0., 0.8);
  TH1F *muon_thpz1 = new TH1F("muon_thpz1", "G4BL", 200, 0., 800.);
  TH1F *positron_thpz1 = new TH1F("positron_thpz1", "G4BL", 200, 0., 800.);

  //  TH1F *pion_thpz2 = new TH1F("pion_thpz2", "Ao", 200, 0., .800.);
  //  TH1F *muon_thpz2 = new TH1F("muon_thpz2", "Ao", 200, 0., 800.);
  //  TH1F *positron_thpz2 = new TH1F("positron_thpz2", "G4BL", 100, 0., 800.);

  TNtuple *ntuple1 = new TNtuple("ntuple1","data from ascii file","x:y:z:Px:Py:Pz:PDGid:t");
  TNtuple *ntuple2 = new TNtuple("ntuple2","data from ascii file","x:y:z:Px:Py:Pz:PDGid");

  Float_t x, y,z, Px, Py, Pz, t, TrackID, ParentID, Weight, Bx, By, Bz, Ex, Ey, Ez, ProperTime, PathLength, PolX, PolY, PolZ, InitX, InitY, InitZ, InitT, InitKE;
  Int_t PDGid, EventID;
  string dummy;
  
  ifstream is;

  // G4BL
  is.open("1-target.txt");
  getline(is,dummy);
  getline(is,dummy);
  while (is.good())
    {
      is >> x >> y >> z >> Px >> Py >> Pz >> t >> PDGid >> EventID >> TrackID >> ParentID >> Weight >> Bx >> By >> Bz >> Ex >> Ey >> Ez >> ProperTime >> PathLength >> PolX >> PolY >> PolZ >> InitX >> InitY >> InitZ >> InitT >> InitKE;

      ntuple1->Fill(x,y,z,Px,Py,Pz,PDGid,ProperTime);
      
    }
  is.close();


  TCanvas *c1 = new TCanvas("c1", "G4BL simulation", 1400 , 500);
  c1->Divide(3,1);

  //  gStyle->SetPalette(1,0);
  //  gStyle->SetOptStat(0);

  Float_t theMax1;
  Float_t theMax2;

  Float_t muons;
  Float_t pions;

  char label[10];


  // NEW target from G4BL
  ///////////////////////

  c1->cd(3);

  ntuple1->Draw("Pz >> pion_ptot","PDGid==211");
  ntuple1->Draw("Pz >> muon_thpz1","PDGid==-13");
  ntuple1->Draw("Pz >> positron_thpz1","PDGid==-11");

  pion_ptot->GetXaxis()->SetTitle("P [MeV/c]");
  muon_thpz1->GetXaxis()->SetTitle("P [MeV/c]");

  cout << "Pions" << "\t" << "Muons" << endl;
  cout << "--------------" << endl;
  cout << pion_ptot->GetEntries() << "\t" << muon_thpz1->GetEntries() << "\t - D1 Muons peak at " << muon_thpz1->GetMaximumBin()*2.0+100 << " MeV/c" << endl ;


  //  TLegend* leg1 = new TLegend(0.1,0.7,0.48,0.9);
  TLegend* leg1 = new TLegend(0.5,0.7,0.9,0.9);
  sprintf(label, "Muons: %.f", muon_thpz1->GetEntries());
  leg1->AddEntry(muon_thpz1, label, "l");
  //sprintf(label, "Muons peak at %.1f MeV/c", muon_thpz1->GetMaximumBin()*2.0+100);
  //leg1->AddEntry(muon_thpz1, label, "l");
  sprintf(label, "Pions: %.f", pion_ptot->GetEntries());
  leg1->AddEntry(pion_ptot, label, "l");
  //sprintf(label, "Positrons: %.f", positron_thpz1->GetEntries());
  //leg1->AddEntry(positron_thpz1, label, "l");

  sprintf(label, "pions/muons: %.2f", pion_ptot->GetEntries()/muon_thpz1->GetEntries());
  leg1->AddEntry((TObject*)0, label,"");

  positron_thpz1->SetLineColor(kBlue);
  positron_thpz1->Draw();

  theMax1  = pion_ptot->GetMaximum();
  theMax2  = muon_thpz1->GetMaximum();

  if (theMax1 > theMax2)  {
    pion_ptot->Draw();
    muon_thpz1->SetLineColor(kRed);
    muon_thpz1->Draw("sames");
  }
  else  {
    muon_thpz1->SetLineColor(kRed);
    muon_thpz1->Draw();
    pion_ptot->Draw("sames");
  }
    
  //leg1->Draw();
    
  //////////////////////////////////////////////////

  c1->cd(1);
  // Tom's simulation
  ////////////////////
    
  // Tom simulation from MARS
  // TFile f("/data/mice/phumhf/TargetSim1/rootFiles/23-4-18-MARSv4-rootoutput/Piplus-650MeV-23-4-18-25deg-5E10protons.root");
  // TFile f("/data/mice/phumhf/TargetSim1/rootFiles/23-4-18-MARSv4-rootoutput/Piplus-700MeV-23-4-18-25deg-5E10protons.root");
  // TFile f("/data/mice/phumhf/TargetSim1/rootFiles/23-4-18-MARSv4-rootoutput/Piplus-800MeV-22-4-18-25deg-4.1E10protons.root");
  // TFile f("/data/mice/phumhf/TargetSim1/rootFiles/27-4-18-MARSv4-rootoutput/Piplus-750MeV-27-4-18-25deg-5E10protons.root");
  // TFile f("/data/mice/phumhf/TargetSim1/rootFiles/MARSv6/Piplus-22-6-18-100cmZprimecut-20cmMICEBPgap-3.5404E12protons.root");
  //TFile f("/data/mice/phumhf/RootFilesForPaolo/Piplus-650MeV-21-7-18-100cmZprimecut-20cmMICEBPgap-2.016E12protons.root");
  //TFile f("/data/mice/phumhf/RootFilesForPaolo/Piplus-700MeV-23-7-18-100cmZprimecut-20cmMICEBPgap-2.3631E12protons.root");
  //TFile f("/data/mice/phumhf/RootFilesForPaolo/Piplus-750MeV-23-7-18-100cmZprimecut-20cmMICEBPgap-2.5936E12protons.root");
  TFile f("/data/mice/phumhf/RootFilesForPaolo/Piplus-800MeV-21-6-18-100cmZprimecut-20cmMICEBPgap-3.5397E12protons.root");
  
  f.cd();
  
  Data.Draw("sqrt(px**2+py**2+pz**2) >> pion_nocut");
  Data.Draw("sqrt(px**2+py**2+pz**2) >> pion_thpz2","PDGId>0");
  
 	      
  /*ntuple2->Draw("Pz >> pion_thpz2","PDGid==211");
    ntuple2->Draw("Pz >> muon_thpz2","PDGid==-13");
    ntuple2->Draw("Pz >> positron_thpz2","PDGid==-11");
  */

  pion_thpz2->GetXaxis()->SetTitle("P [MeV/c]");
  //muon_thpz2->GetXaxis()->SetTitle("P [MeV/c]");

  cout << pion_thpz2->GetEntries() << endl; // << "\t" << muon_thpz2->GetEntries() << endl;


  TLegend* leg2 = new TLegend(0.5,0.7,0.8,0.8);
  //sprintf(label, "Muons: %.f", muon_thpz2->GetEntries());
  //leg2->AddEntry(muon_thpz2, label, "l");
  //sprintf(label, "Muons peak at %.1f MeV/c", muon_thpz2->GetMaximumBin()*2.0+100);
  //leg2->AddEntry(muon_thpz2, label, "l");
  sprintf(label, "Pions: %.f", pion_thpz2->GetEntries());
  leg2->AddEntry(pion_thpz2, label,"l");
  //sprintf(label, "Positrons: %.f", positron_thpz2->GetEntries());
  //leg2->AddEntry(positron_thpz2, label, "l");

  //sprintf(label, "pions/muons: %.2f", pion_thpz2->GetEntries()/muon_thpz2->GetEntries());
  //leg2->AddEntry((TObject*)0, label,"");

  //  positron_thpz2->SetLineColor(kBlue);
  //positron_thpz2->Draw();

  theMax1  = pion_thpz2->GetMaximum();
  //theMax2  = muon_thpz2->GetMaximum();

  c1->cd(1);
  //  pion_nocut->SetLineStyle(9);
  pion_nocut->Draw();

  //  c1->cd(2);
  //  pion_thpz2->Draw("same");   


  /*
  if (theMax1 > theMax2)  {
    pion_thpz2->Draw();
    muon_thpz2->SetLineColor(kRed);
    muon_thpz2->Draw("sames");
  }
  else  {
    muon_thpz2->SetLineColor(kRed);
    muon_thpz2->Draw();
    pion_thpz2->Draw("sames");
  }
  */


  leg2->Draw();

  // Fit on Tom: 3 Gaussian

  cout << endl << "Fit..." << endl;
  cout << "------" << endl;

  TFitResultPtr r1 = pion_thpz2->Fit("gaus","S","",0.,.8);   // first fit
  TFitResultPtr r2 = pion_thpz2->Fit("gaus","S","",0.,.8);   // second fit
  TFitResultPtr r3 = pion_thpz2->Fit("gaus","S","",0.,.8);   // third fit
  // TFitResultPtr r4 = pion_thpz2->Fit("gaus","S","",0.,.8);   // fourth fit
  
  //  TF1 * f1 = new TF1("fitFunc","gaus(0)+gaus(3)+gaus(6)+gaus(9)");
  TF1 * f1 = new TF1("fitFunc","gaus(0)+gaus(3)+gaus(6)");

  // parameters of first gaussian
  f1->SetParameter(0,r1->Parameter(0));
  f1->SetParameter(1,r1->Parameter(1));
  f1->SetParameter(2,r1->Parameter(2));
  // parameters of second gaussian
  f1->SetParameter(3,r2->Parameter(0));
  f1->SetParameter(4,r2->Parameter(1));
  f1->SetParameter(5,r2->Parameter(2));
  // parameters of third gaussian
  f1->SetParameter(6,r3->Parameter(0));
  f1->SetParameter(7,r3->Parameter(1));
  f1->SetParameter(8,r3->Parameter(2));
  // parameters of fourth gaussian
  //  f1->SetParameter(9,r4->Parameter(0));
  //  f1->SetParameter(10,r4->Parameter(1));
  //  f1->SetParameter(11,r4->Parameter(2));

  // Fit limits   
  f1->SetParLimits(0,0,1E12);
  f1->SetParLimits(3,0,1E12);
  f1->SetParLimits(6,0,1E12);
  //  f1->SetParLimits(9,0,1E12);

  f1->SetParLimits(1, 0.1, 0.4);
  f1->SetParLimits(4, 0.2, 0.5);
  f1->SetParLimits(7, 0.4, 0.5);
  //  f1->SetParLimits(10,0.0, 0.3);

  f1->SetParLimits(2, 0.0, 30.);
  f1->SetParLimits(5, 0.0, 30.);
  f1->SetParLimits(8, 0.0, 30.);

  pion_thpz2->Fit(f1); 


  // Draw the gaussians     

  c1->cd(2);

  TF1 gaussian1("gauss", "[0] / sqrt(2.0 * TMath::Pi()) / [2] * exp(-(x-[1])*(x-[1])/2./[2]/[2])", 0, .800);
  gaussian1.SetParameters(f1->GetParameter(0),f1->GetParameter(1),f1->GetParameter(2));
  gaussian1.Draw();

  TF1 gaussian2("gauss", "[0] / sqrt(2.0 * TMath::Pi()) / [2] * exp(-(x-[1])*(x-[1])/2./[2]/[2])", 0, .800);
  gaussian2.SetParameters(f1->GetParameter(3),f1->GetParameter(4),f1->GetParameter(5));
  gaussian2.Draw("same");

  TF1 gaussian3("gauss", "[0] / sqrt(2.0 * TMath::Pi()) / [2] * exp(-(x-[1])*(x-[1])/2./[2]/[2])", 0, .800);
  gaussian3.SetParameters(f1->GetParameter(6),f1->GetParameter(7),f1->GetParameter(8));
  gaussian3.Draw("same");

  //  TF1 gaussian4("gauss", "[0] / sqrt(2.0 * TMath::Pi()) / [2] * exp(-(x-[1])*(x-[1])/2./[2]/[2])", 0, .800);
  //  gaussian4.SetParameters(f1->GetParameter(9),f1->GetParameter(10),f1->GetParameter(11));
  //  gaussian4.Draw("same");

  cout << "Yields: " << sqrt(2*3.14159265359)*(f1->GetParameter(0)*f1->GetParameter(2) + f1->GetParameter(3)*f1->GetParameter(5) + f1->GetParameter(6)*f1->GetParameter(8)) << endl; // + f1->GetParameter(9)*f1->GetParameter(11)) << endl;
  
  cout << "TOT yields: " << f1->Integral(0,.900) << endl << endl;
  cout << "1st:  " << sqrt(2*3.14159265359)*f1->GetParameter(0)*f1->GetParameter(2) << endl;
  cout << "2nd:  " << sqrt(2*3.14159265359)*f1->GetParameter(3)*f1->GetParameter(5) << endl;
  cout << "3rd:  " << sqrt(2*3.14159265359)*f1->GetParameter(6)*f1->GetParameter(8) << endl;
  //  cout << "4th:  " << sqrt(2*3.14159265359)*f1->GetParameter(9)*f1->GetParameter(11) << endl;

  cout << endl;

  cout <<  "meanMomentum=" << f1->GetParameter(1)*1E3 << " sigmaP=" <<  f1->GetParameter(2)*1E3 << endl; 
  cout <<  "meanMomentum=" << f1->GetParameter(4)*1E3 << " sigmaP=" <<  f1->GetParameter(5)*1E3 << endl; 
  cout <<  "meanMomentum=" << f1->GetParameter(7)*1E3 << " sigmaP=" <<  f1->GetParameter(8)*1E3 << endl; 
  //  cout <<  "meanMomentum=" << f1->GetParameter(10)*1E3 << " sigmaP=" <<  f1->GetParameter(11)*1E3 << endl; 


  c1.SaveAs("Target_800MeV.png");
  
}

