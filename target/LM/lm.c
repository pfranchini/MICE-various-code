// Produce plot from the G4BL output root file of the LM.in

{

  TFile f("g4beamline.root");
  Detector->cd();

  TH1F* electron1 = new TH1F("electron1", "LM12", 100, 0, 800);
  TH1F* muon1     = new TH1F("muon1", "LM12", 100, 0, 800);
  TH1F* pion1     = new TH1F("pion1", "LM12", 100, 0, 800);
  TH1F* neutron1  = new TH1F("neutron1","LM12", 100, 0, 800);
  TH1F* proton1   = new TH1F("proton1", "LM12", 100, 0, 800);

  TH1F* electron2 = new TH1F("electron2", "LM34", 100, 0, 800);
  TH1F* muon2     = new TH1F("muon2", "LM34", 100, 0, 800);
  TH1F* pion2     = new TH1F("pion2", "LM34", 100, 0, 800);
  TH1F* neutron2  = new TH1F("neutron2","LM34", 100, 0, 800);
  TH1F* proton2   = new TH1F("proton2", "LM34", 100, 0, 800);

  TH1F* electron3 = new TH1F("electron3", "LM34 (if LM1234)", 100, 0, 800);
  TH1F* muon3     = new TH1F("muon3", "LM34 (if LM1234)", 100, 0, 800);
  TH1F* pion3     = new TH1F("pion3", "LM34 (if LM1234)", 100, 0, 800);
  TH1F* neutron3  = new TH1F("neutron3","LM34 (if LM1234)", 100, 0, 800);
  TH1F* proton3   = new TH1F("proton3", "LM34 (if LM1234)", 100, 0, 800);

  TH1F* electron4 = new TH1F("electron4", "LM34 (if #Deltat)", 100, 0, 800);
  TH1F* muon4     = new TH1F("muon4", "LM34 (if #Deltat)", 100, 0, 800);
  TH1F* pion4     = new TH1F("pion4", "LM34 (if #Deltat)", 100, 0, 800);
  TH1F* neutron4  = new TH1F("neutron4","LM34 (if #Deltat)", 100, 0, 800);
  TH1F* proton4   = new TH1F("proton4", "LM34 (if #Deltat)", 100, 0, 800);

  TCanvas *c1 = new TCanvas("c1", "Luminosity monitor", 1200 , 1080);
  c1->Divide(2,2);
  gStyle->SetOptStat(0);

  neutron1.SetFillColor(kRed);
  proton1.SetFillColor(kBlue);
  pion1.SetFillColor(kYellow);
  muon1.SetFillColor(kGreen);
  electron1.SetFillColor(kMagenta);

  neutron2.SetFillColor(kRed);
  proton2.SetFillColor(kBlue);
  pion2.SetFillColor(kYellow);
  muon2.SetFillColor(kGreen);
  electron2.SetFillColor(kMagenta);

  neutron3.SetFillColor(kRed);
  proton3.SetFillColor(kBlue);
  pion3.SetFillColor(kYellow);
  muon3.SetFillColor(kGreen);
  electron3.SetFillColor(kMagenta);

  neutron1.SetFillStyle(3001);
  proton1.SetFillStyle(3001);
  pion1.SetFillStyle(3001);
  muon1.SetFillStyle(3001);
  electron1.SetFillStyle(3001);

  neutron2.SetFillStyle(3001);
  proton2.SetFillStyle(3001);
  pion2.SetFillStyle(3001);
  muon2.SetFillStyle(3001);
  electron2.SetFillStyle(3001);

  neutron3.SetFillStyle(3001);
  proton3.SetFillStyle(3001);
  pion3.SetFillStyle(3001);
  muon3.SetFillStyle(3001);
  electron3.SetFillStyle(3001);

  neutron4.SetFillStyle(3001);
  proton4.SetFillStyle(3001);
  pion4.SetFillStyle(3001);
  muon4.SetFillStyle(3001);
  electron4.SetFillStyle(3001);

  neutron1->GetXaxis()->SetTitle("P [MeV]");
  neutron2->GetXaxis()->SetTitle("P [MeV]");
  neutron3->GetXaxis()->SetTitle("P [MeV]");
  neutron4->GetXaxis()->SetTitle("P [MeV]");

  c1.cd(1);

  LMSC1.Draw("sqrt(Px**2+Py**2+Pz**2)>>electron1","abs(PDGid)==11");
  LMSC1.Draw("sqrt(Px**2+Py**2+Pz**2)>>muon1","abs(PDGid)==13");
  LMSC1.Draw("sqrt(Px**2+Py**2+Pz**2)>>pion1","abs(PDGid)==211");
  LMSC1.Draw("sqrt(Px**2+Py**2+Pz**2)>>neutron1","abs(PDGid)==2112");
  LMSC1.Draw("sqrt(Px**2+Py**2+Pz**2)>>proton1","abs(PDGid)==2212");
  
  neutron1.Draw();
  proton1.Draw("sames");
  electron1.Draw("sames");
  muon1.Draw("sames");
  pion1.Draw("sames");

  leg1 = new TLegend(0.52,0.66,0.87,0.87);
  leg1->AddEntry(neutron1,Form("Neutrons - %u",(int)neutron1.GetEntries()),"f");
  leg1->AddEntry(proton1,Form("Protons - %u",(int)proton1.GetEntries()),"f");
  leg1->AddEntry(electron1,Form("Electrons - %u",(int)electron1.GetEntries()),"f");
  leg1->AddEntry(muon1,Form("Muons - %u",(int)muon1.GetEntries()),"f");
  leg1->AddEntry(pion1,Form("Pions - %u",(int)pion1.GetEntries()),"f");
  leg1->Draw();

  c1.cd(2);

  LMSC2.Draw("sqrt(Px**2+Py**2+Pz**2)>>electron2","abs(PDGid)==11");
  LMSC2.Draw("sqrt(Px**2+Py**2+Pz**2)>>muon2","abs(PDGid)==13");
  LMSC2.Draw("sqrt(Px**2+Py**2+Pz**2)>>pion2","abs(PDGid)==211");
  LMSC2.Draw("sqrt(Px**2+Py**2+Pz**2)>>neutron2","abs(PDGid)==2112");
  LMSC2.Draw("sqrt(Px**2+Py**2+Pz**2)>>proton2","abs(PDGid)==2212");

  neutron2.Draw();
  proton2.Draw("sames");  
  electron2.Draw("sames");
  muon2.Draw("sames");
  pion2.Draw("sames");

  leg2 = new TLegend(0.52,0.66,0.87,0.87);
  leg2->AddEntry(neutron2,Form("Neutrons - %u",(int)neutron2.GetEntries()),"f");
  leg2->AddEntry(proton2,Form("Protons - %u",(int)proton2.GetEntries()),"f");
  leg2->AddEntry(electron2,Form("Electrons - %u",(int)electron2.GetEntries()),"f");
  leg2->AddEntry(muon2,Form("Muons - %u",(int)muon2.GetEntries()),"f");
  leg2->AddEntry(pion2,Form("Pions - %u",(int)pion2.GetEntries()),"f");
  leg2->Draw();

  cout << "          " << "\t" << "LM12" << "\t" << "LM34" << endl;
  cout << "Neutrons: " << "\t" << neutron1.GetEntries() << "\t" << neutron2.GetEntries() << endl;
  cout << "Protons:  " << "\t" << proton1.GetEntries() << "\t" << proton2.GetEntries() << endl;
  cout << "Electrons:" << "\t" << electron1.GetEntries() << "\t" << electron2.GetEntries() << endl;
  cout << "Muons:    " << "\t" << muon1.GetEntries() << "\t" << muon2.GetEntries() << endl;
  cout << "Pions:    " << "\t" << pion1.GetEntries() << "\t" << pion2.GetEntries() << endl;

  /////////////////////////////////////////////

  // Four-fold coincidence rates:

  TNtuple *nf1 = (TNtuple*)f->Get("Detector/LMSC1");
  TNtuple *nf2 = (TNtuple*)f->Get("Detector/LMSC2");
  
  cout << "          " << "\t" << "=================" << endl;
  cout << "Tot:      " << "\t" << nf1->GetEntries()  << "\t" << nf2->GetEntries() << endl;

  Float_t Px,Py,Pz,PDGid,EventID1,EventID2, t1,t2, PDGid_mother;
  Int_t n;
  Int_t time_window = 2; // ns (the distance between LM12 and LM24 can be travelled in 0.75 ns)
  TH1F* time = new TH1F("time", "time difference", 100, 0, 4);

  nf1->SetBranchAddress("EventID",&EventID1);
  nf1->SetBranchAddress("t",&t1);
  nf1->SetBranchAddress("PDGid",&PDGid_mother);
  nf2->SetBranchAddress("EventID",&EventID2);
  nf2->SetBranchAddress("Px",&Px);
  nf2->SetBranchAddress("Py",&Px);
  nf2->SetBranchAddress("Pz",&Px);
  nf2->SetBranchAddress("PDGid",&PDGid);
  nf2->SetBranchAddress("t",&t2);
  
  std::cout << endl << "Entry: " << endl;
  for (Int_t i=0; i<(Int_t)nf2->GetEntries(); i++) {
    nf2->GetEntry(i);
    if ( i % 10 == 0 ) std::cout << "\b\b\b\b\b\b\b\b\b\b" <<i;
    for (Int_t j=0; j<(Int_t)nf1->GetEntries(); j++) {
      nf1->GetEntry(j);
      if (EventID1 == EventID2) {  // LMSC12 and LMSC34 coincidence (same EventIDs means same track even if different particles after a decay)
	time->Fill(t2-t1);
	if ((t2-t1)<time_window) {  // LMSC12 and LMSC34 time coincidence
	  if (abs(PDGid)==11)   electron3->Fill(sqrt(Px**2+Py**2+Pz**2),1);
	  if (abs(PDGid)==13)   muon3->Fill(sqrt(Px**2+Py**2+Pz**2),1);
	  if (abs(PDGid)==211)  pion3->Fill(sqrt(Px**2+Py**2+Pz**2),1);
	  if (abs(PDGid)==2112) neutron3->Fill(sqrt(Px**2+Py**2+Pz**2),1);
	  if (abs(PDGid)==2212) proton3->Fill(sqrt(Px**2+Py**2+Pz**2),1);
	  break;
	  n++;
	}
      }
    }
  }
  
  c1.cd(3);
  
  neutron3.Draw();
  proton3.Draw("sames");
  electron3.Draw("sames");
  muon3.Draw("sames");
  pion3.Draw("sames");

  leg3 = new TLegend(0.52,0.66,0.87,0.87);
  leg3->AddEntry(neutron3,Form("Neutrons - %u",(int)neutron3.GetEntries()),"f");
  leg3->AddEntry(proton3,Form("Protons - %u",(int)proton3.GetEntries()),"f");
  leg3->AddEntry(electron3,Form("Electrons - %u",(int)electron3.GetEntries()),"f");
  leg3->AddEntry(muon3,Form("Muons - %u",(int)muon3.GetEntries()),"f");
  leg3->AddEntry(pion3,Form("Pions - %u",(int)pion3.GetEntries()),"f");
  leg3->Draw();

  c1.cd(4);
  time.Draw();


  /*  /////////////////////////////////////////////

  // Time coincidence between LM12 and LM34:

  Int_t n2=0;
  
  for (Int_t i=0; i<(Int_t)nf1->GetEntries(); i++) {
    nf1->GetEntry(i);
    //    cout << t1 << endl;    
    for (Int_t j=0; j<(Int_t)nf2->GetEntries(); j++) {
      nf2->GetEntry(j);
          
      if ((t2-t1)<time_window) {  // LMSC12 and LMSC34 time coincidence
	if (PDGid==11)   electron4->Fill(sqrt(Px**2+Py**2+Pz**2),1);
	if (PDGid==13)   muon4->Fill(sqrt(Px**2+Py**2+Pz**2),1);
	if (PDGid==211)  pion4->Fill(sqrt(Px**2+Py**2+Pz**2),1);
	if (PDGid==2112) neutron4->Fill(sqrt(Px**2+Py**2+Pz**2),1);
	if (PDGid==2212) proton4->Fill(sqrt(Px**2+Py**2+Pz**2),1);
	n2++;
      }
    }
  }  
  
  c1.cd(4);
  //  time.Draw();
  
  neutron4.Draw();
  proton4.Draw("sames");
  electron4.Draw("sames");
  muon4.Draw("sames");
  pion4.Draw("sames");
  
  leg4 = new TLegend(0.52,0.66,0.87,0.87);
  leg4->AddEntry(neutron4,Form("Neutrons - %u",(int)neutron4.GetEntries()),"f");
  leg4->AddEntry(proton4,Form("Protons - %u",(int)proton4.GetEntries()),"f");
  leg4->AddEntry(electron4,Form("Electrons - %u",(int)electron4.GetEntries()),"f");
  leg4->AddEntry(muon4,Form("Muons - %u",(int)muon4.GetEntries()),"f");
  leg4->AddEntry(pion4,Form("Pions - %u",(int)pion4.GetEntries()),"f");
  leg4->Draw();
  
  /////////////////////////////////////////////*/

  cout << endl;
  
  cout << "ID coincidence" << endl;
  cout << "LM1234:       " << n << endl;
  cout << "LM1234/LM12:  " << (Float_t)n/nf1->GetEntries() << endl;
  cout << "LM1234/LM34:  " << (Float_t)n/nf2->GetEntries() << endl;
  //  cout << "Time coincidence, " << time_window << "ns" << endl;
  //  cout << "LM1234     :  " << n2<< endl;
  //  cout << "LM1234/LM12:  " << (Float_t)n2/nf1->GetEntries() << endl;
  //  cout << "LM1234/LM34:  " << (Float_t)n2/nf2->GetEntries() << endl;
  cout << "LM34/LM12:    " << (Float_t)nf2->GetEntries()/nf1->GetEntries() << endl;


  c1->SaveAs("LM.png"); 


  
}


