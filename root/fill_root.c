// Fill from ASCII files into a bunch of TH1F

{

  TH1F *pion_thpz1 = new TH1F("pion_thpz1", "After D1", 100, 0., 500.);
  TH1F *muon_thpz1 = new TH1F("muon_thpz1", "After D1", 100, 0., 500.);
  TH1F *positron_thpz1 = new TH1F("positron_thpz1", "After D1", 100, 0., 500.);

  TH1F *pion_thpz2 = new TH1F("pion_thpz2", "After DS", 100, 0., 500.);
  TH1F *muon_thpz2 = new TH1F("muon_thpz2", "After DS", 100, 0., 500.);
  TH1F *positron_thpz2 = new TH1F("positron_thpz2", "After D1", 100, 0., 500.);

  TH1F *pion_thpz3 = new TH1F("pion_thpz3", "After D2", 100, 0., 500.);
  TH1F *muon_thpz3 = new TH1F("muon_thpz3", "After D2", 100, 0., 500.);
  TH1F *positron_thpz3 = new TH1F("positron_thpz3", "After D1", 100, 0., 500.);

  TH1F *pion_thpz4 = new TH1F("pion_thpz4", "TOF0", 100, 0., 500.);
  TH1F *muon_thpz4 = new TH1F("muon_thpz4", "TOF0", 100, 0., 500.);
  TH1F *positron_thpz4 = new TH1F("positron_thpz4", "After D1", 100, 0., 500.);

  TH1F *pion_thpz5 = new TH1F("pion_thpz5", "TOF1", 100, 0., 500.);
  TH1F *muon_thpz5 = new TH1F("muon_thpz5", "TOF1", 100, 0., 500.);
  TH1F *positron_thpz5 = new TH1F("positron_thpz5", "After D1", 100, 0., 500.);

  //TH1F *pion_thpz6 = new TH1F("pion_thpz6", "After D2", 100, 0., 500.);
  //TH1F *muon_thpz6 = new TH1F("muon_thpz6", "After D2", 100, 0., 500.);

  TNtuple *ntuple1 = new TNtuple("ntuple1","data from ascii file","Px:Py:Pz:PDGid:t");
  TNtuple *ntuple2 = new TNtuple("ntuple2","data from ascii file","Px:Py:Pz:PDGid:t");
  TNtuple *ntuple3 = new TNtuple("ntuple3","data from ascii file","Px:Py:Pz:PDGid:t");
  TNtuple *ntuple4 = new TNtuple("ntuple4","data from ascii file","Px:Py:Pz:PDGid:t");
  TNtuple *ntuple5 = new TNtuple("ntuple5","data from ascii file","Px:Py:Pz:PDGid:t");
  //TNtuple *ntuple6 = new TNtuple("ntuple6","data from ascii file","Px:Py:Pz:PDGid:t");


  Float_t x, y,z, Px, Py, Pz, t, TrackID, ParentID, Weight, Bx, By, Bz, Ex, Ey, Ez, ProperTime, PathLength, PolX, PolY, PolZ, InitX, InitY, InitZ, InitT, InitKE;
  Int_t PDGid, EventID;
  string dummy;
  
  ifstream is;

  is.open("4-d1.txt");
  getline(is,dummy);
  getline(is,dummy);
  while (is.good())
    {

      is >> x >> y >> z >> Px >> Py >> Pz >> t >> PDGid >> EventID >> TrackID >> ParentID >> Weight >> Bx >> By >> Bz >> Ex >> Ey >> Ez >> ProperTime >> PathLength >> PolX >> PolY >> PolZ >> InitX >> InitY >> InitZ >> InitT >> InitKE;

      ntuple1->Fill(Px,Py,Pz,PDGid,ProperTime);

    }
  is.close();
  
  is.open("5-ds.txt");
  getline(is,dummy);
  getline(is,dummy);
  while (is.good())
    {
      is >> x >> y >> z >> Px >> Py >> Pz >> t >> PDGid >> EventID >> TrackID >> ParentID >> Weight >> Bx >> By >> Bz >> Ex >> Ey >> Ez >> ProperTime >> PathLength >> PolX >> PolY >> PolZ >> InitX >> InitY >> InitZ >> InitT >> InitKE;
      
      ntuple2->Fill(Px,Py,Pz,PDGid,ProperTime);
      
    }
  is.close();

  is.open("6-d2.txt");
  getline(is,dummy);
  getline(is,dummy);
  while (is.good())
    {
      is >> x >> y >> z >> Px >> Py >> Pz >> t >> PDGid >> EventID >> TrackID >> ParentID >> Weight >> Bx >> By >> Bz >> Ex >> Ey >> Ez >> ProperTime >> PathLength >> PolX >> PolY >> PolZ >> InitX >> InitY >> InitZ >> InitT >> InitKE;
      
      ntuple3->Fill(Px,Py,Pz,PDGid,ProperTime);

    }
  is.close();
  
  is.open("7-tof0.txt");
  getline(is,dummy);
  getline(is,dummy);
  while (is.good())
    {
      is >> x >> y >> z >> Px >> Py >> Pz >> t >> PDGid >> EventID >> TrackID >> ParentID >> Weight >> Bx >> By >> Bz >> Ex >> Ey >> Ez >> ProperTime >> PathLength >> PolX >> PolY >> PolZ >> InitX >> InitY >> InitZ >> InitT >> InitKE;

      ntuple4->Fill(Px,Py,Pz,PDGid,ProperTime);

    }
  is.close();


  is.open("8-tof1.txt");
  getline(is,dummy);
  getline(is,dummy);
  while (is.good())
    {
      is >> x >> y >> z >> Px >> Py >> Pz >> t >> PDGid >> EventID >> TrackID >> ParentID >> Weight >> Bx >> By >> Bz >> Ex >> Ey >> Ez >> ProperTime >> PathLength >> PolX >> PolY >> PolZ >> InitX >> InitY >> InitZ >> InitT >> InitKE;

      ntuple5->Fill(Px,Py,Pz,PDGid,ProperTime);

    }
  is.close();


  TCanvas *c1 = new TCanvas("c1", "G4BL simulation", 1280 , 1024);
  c1->Divide(3,2);

  //  gStyle->SetPalette(1,0);
  gStyle->SetOptStat(0);

  //TCut cut1 = "sqrt(Px*Px+Py*Py)<0.15*abs(Pz)";
  TCut cut1 = "sqrt(Px*Px)<0.10*abs(Pz)";

  Float_t theMax1;
  Float_t theMax2;

  Float_t muons;
  Float_t pions;

  char label[10];

  // Fill the canvas:

  c1->cd(1);

  ntuple1->Draw("Pz >> pion_thpz1","PDGid==211"&&cut1);
  ntuple1->Draw("Pz >> muon_thpz1","PDGid==-13"&&cut1);
  ntuple1->Draw("Pz >> positron_thpz1","PDGid==-11"&&cut1);

  cout << "Pions  Muons" << endl;
  cout << "============" << endl;
  cout << pion_thpz1->GetEntries() << " " << muon_thpz1->GetEntries() << endl;

  //  TLegend* leg1 = new TLegend(0.1,0.7,0.48,0.9);
  TLegend* leg1 = new TLegend(0.6,0.7,0.9,0.9);
  sprintf(label, "Muons: %.f", muon_thpz1->GetEntries());
  leg1->AddEntry(muon_thpz1, label, "l");
  sprintf(label, "Muons peak at %.1f MeV/c", muon_thpz1->GetMaximumBin()*5);
  leg1->AddEntry(muon_thpz1, label, "l");
  sprintf(label, "Pions: %.f", pion_thpz1->GetEntries());
  leg1->AddEntry(pion_thpz1, label, "l");
  //sprintf(label, "Positrons: %.f", positron_thpz1->GetEntries());
  //leg1->AddEntry(positron_thpz1, label, "l");

  sprintf(label, "pions/muons: %.2f", pion_thpz1->GetEntries()/muon_thpz1->GetEntries());
  leg1->AddEntry((TObject*)0, label,"");

  positron_thpz1->SetLineColor(kBlue);
  positron_thpz1->Draw();

  theMax1  = pion_thpz1->GetMaximum();
  theMax2  = muon_thpz1->GetMaximum();

  if (theMax1 > theMax2)  {
    pion_thpz1->Draw();
    muon_thpz1->SetLineColor(kRed);
    muon_thpz1->Draw("sames");
  }
  else  {
    muon_thpz1->SetLineColor(kRed);
    muon_thpz1->Draw();
    pion_thpz1->Draw("sames");
  }
    
  leg1->Draw();
    
    
  c1->cd(2);

  ntuple2->Draw("Pz >> pion_thpz2","PDGid==211"&&cut1);
  ntuple2->Draw("Pz >> muon_thpz2","PDGid==-13"&&cut1);
  ntuple2->Draw("Pz >> positron_thpz2","PDGid==-11"&&cut1);

  cout << pion_thpz2->GetEntries() << " " << muon_thpz2->GetEntries() << endl;

  TLegend* leg2 = new TLegend(0.6,0.7,0.9,0.9);
  sprintf(label, "Muons: %.f", muon_thpz2->GetEntries());
  leg2->AddEntry(muon_thpz2, label, "l");
  sprintf(label, "Muons peak at %.1f MeV/c", muon_thpz2->GetMaximumBin()*5);
  leg2->AddEntry(muon_thpz2, label, "l");
  sprintf(label, "Pions: %.f", pion_thpz2->GetEntries());
  leg2->AddEntry(pion_thpz2, label,"l");
  sprintf(label, "Positrons: %.f", positron_thpz2->GetEntries());
  leg2->AddEntry(positron_thpz2, label, "l");

  sprintf(label, "pions/muons: %.2f", pion_thpz2->GetEntries()/muon_thpz2->GetEntries());
  leg2->AddEntry((TObject*)0, label,"");

  positron_thpz2->SetLineColor(kBlue);
  positron_thpz2->Draw();

  theMax1  = pion_thpz2->GetMaximum();
  theMax2  = muon_thpz2->GetMaximum();

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

  leg2->Draw();


  c1->cd(3);

  ntuple3->Draw("Pz >> pion_thpz3","PDGid==211"&&cut1);
  ntuple3->Draw("Pz >> muon_thpz3","PDGid==-13"&&cut1);
  ntuple3->Draw("Pz >> positron_thpz3","PDGid==-11"&&cut1);

  cout << pion_thpz3->GetEntries() << " " << muon_thpz3->GetEntries() << endl;

  TLegend* leg3 = new TLegend(0.6,0.7,0.9,0.9);
  sprintf(label, "Muons: %.f", muon_thpz3->GetEntries());
  leg3->AddEntry(muon_thpz3, label, "l");
  sprintf(label, "Muons peak at %.1f MeV/c", muon_thpz3->GetMaximumBin()*5);
  leg3->AddEntry(muon_thpz3, label, "l");
  sprintf(label, "Pions: %.f", pion_thpz3->GetEntries());
  leg3->AddEntry(pion_thpz3, label, "l");
  sprintf(label, "Positrons: %.f", positron_thpz3->GetEntries());
  leg3->AddEntry(positron_thpz3, label, "l");

  sprintf(label, "pions/muons: %.2f", pion_thpz3->GetEntries()/muon_thpz3->GetEntries());
  leg3->AddEntry((TObject*)0, label,"");


  positron_thpz3->SetLineColor(kBlue);
  positron_thpz3->Draw();

  theMax1  = pion_thpz3->GetMaximum();
  theMax2  = muon_thpz3->GetMaximum();

  if (theMax1 > theMax2)  {
    pion_thpz3->Draw();
    muon_thpz3->SetLineColor(kRed);
    muon_thpz3->Draw("sames");
  }
  else  {
    muon_thpz3->SetLineColor(kRed);
    muon_thpz3->Draw();
    pion_thpz3->Draw("sames");
  }

  leg3->Draw();



  c1->cd(4);

  ntuple4->Draw("Pz >> pion_thpz4","PDGid==211"&&cut1);
  ntuple4->Draw("Pz >> muon_thpz4","PDGid==-13"&&cut1);
  ntuple4->Draw("Pz >> positron_thpz4","PDGid==-11"&&cut1);
  
  cout << pion_thpz4->GetEntries() << " " << muon_thpz4->GetEntries() << endl;

  TLegend* leg4 = new TLegend(0.6,0.7,0.9,0.9);
  sprintf(label, "Muons: %.f", muon_thpz4->GetEntries());
  leg4->AddEntry(muon_thpz4, label, "l");
  sprintf(label, "Muons peak at %.1f MeV/c", muon_thpz4->GetMaximumBin()*5);
  leg4->AddEntry(muon_thpz4, label, "l");
  sprintf(label, "Pions: %.f", pion_thpz4->GetEntries());
  leg4->AddEntry(pion_thpz4, label, "l");
  sprintf(label, "Positrons: %.f", positron_thpz4->GetEntries());
  leg4->AddEntry(positron_thpz4, label, "l");

  sprintf(label, "pions/muons: %.2f", pion_thpz4->GetEntries()/muon_thpz4->GetEntries());
  leg4->AddEntry((TObject*)0, label,"");


  positron_thpz4->SetLineColor(kBlue);
  positron_thpz4->Draw();

  theMax1  = pion_thpz4->GetMaximum();
  theMax2  = muon_thpz4->GetMaximum();

  if (theMax1 > theMax2)  {
    pion_thpz4->Draw();
    muon_thpz4->SetLineColor(kRed);
    muon_thpz4->Draw("sames");
  }
  else  {
    muon_thpz4->SetLineColor(kRed);
    muon_thpz4->Draw();
    pion_thpz4->Draw("sames");
  }

  leg4->Draw();

  c1->cd(5);

  ntuple5->Draw("Pz >> pion_thpz5","PDGid==211"&&cut1);
  ntuple5->Draw("Pz >> muon_thpz5","PDGid==-13"&&cut1);
  ntuple5->Draw("Pz >> positron_thpz5","PDGid==-11"&&cut1);

  cout << pion_thpz5->GetEntries() << " " << muon_thpz5->GetEntries() << endl;

  TLegend* leg5 = new TLegend(0.6,0.7,0.9,0.9);
  sprintf(label, "Muons: %.f", muon_thpz5->GetEntries());
  leg5->AddEntry(muon_thpz5, label, "l");
  sprintf(label, "Muons peak at %.1f MeV/c", muon_thpz5->GetMaximumBin()*5);
  leg5->AddEntry(muon_thpz5, label, "l");
  sprintf(label, "Pions: %.f", pion_thpz5->GetEntries());
  leg5->AddEntry(pion_thpz5, label, "l");
  sprintf(label, "Positrons: %.f", positron_thpz5->GetEntries());
  leg5->AddEntry(positron_thpz5, label, "l");

  sprintf(label, "pions/muons: %.2f", pion_thpz5->GetEntries()/muon_thpz5->GetEntries());
  leg5->AddEntry((TObject*)0, label,"");


  positron_thpz5->SetLineColor(kBlue);
  positron_thpz5->Draw();

  theMax1  = pion_thpz5->GetMaximum();
  theMax2  = muon_thpz5->GetMaximum();

  if (theMax1 > theMax2)  {
    pion_thpz5->Draw();
    muon_thpz5->SetLineColor(kRed);
    muon_thpz5->Draw("sames");
  }
  else  {
    muon_thpz5->SetLineColor(kRed);
    muon_thpz5->Draw();
    pion_thpz5->Draw("sames");
  }

  leg5->Draw();

  /////////////
  //  Delta t

  c1->cd(6);

  ifstream is0,is1;

  // INPUT:                                                                                                                                                                                                                                 
  is0.open("7-tof0.txt");  
  is1.open("8-tof1.txt");

  TH1F *pion_deltat = new TH1F("pion_deltat", "TOF0-1 #Deltat", 100, 25., 35.);
  TH1F *muon_deltat = new TH1F("muon_deltat", "TOF0-1 #Deltat", 100, 25., 35.);

  pion_deltat->GetXaxis()->SetTitle("#Deltat TOF0-1 (ns)");
  muon_deltat->GetXaxis()->SetTitle("#Deltat TOF0-1 (ns)");

  // Cycle on EventID                                                                                                                                                                                                                       

  TFile *myfile1=new TFile("tof0.root", "RECREATE");
  TFile *myfile2=new TFile("tof1.root", "RECREATE");
  TTree *ntuple1t = new TTree("ntuple1t","data from ascii file TOF0");
  TTree *ntuple2t = new TTree("ntuple2t","data from ascii file TOF1");

  ntuple1t->Branch("Px",&Px);
  ntuple1t->Branch("Py",&Py);
  ntuple1t->Branch("Pz",&Pz);
  ntuple1t->Branch("PDGid",&PDGid);
  ntuple1t->Branch("t",&t);
  ntuple1t->Branch("EventID",&EventID);

  ntuple2t->Branch("Px",&Px);
  ntuple2t->Branch("Py",&Py);
  ntuple2t->Branch("Pz",&Pz);
  ntuple2t->Branch("PDGid",&PDGid);
  ntuple2t->Branch("t",&t);
  ntuple2t->Branch("EventID",&EventID);

  getline(is0,dummy);
  getline(is0,dummy);
  while (is0.good())
    {
      is0 >> x >> y >> z >> Px >> Py >> Pz >> t >> PDGid >> EventID >> TrackID >> ParentID >> Weight >> Bx >> By >> Bz >> Ex >> Ey >> Ez >> ProperTime >> PathLength >> PolX >> PolY >> PolZ >> InitX >> InitY >> InitZ >> InitT >> InitKE;
      if ( sqrt(Px*Px)<0.10*abs(Pz) )
        ntuple1t->Fill();
    }
  is0.close();


  getline(is1,dummy);
  getline(is1,dummy);
  while (is1.good())
    {
      is1 >> x >> y >> z >> Px >> Py >> Pz >> t >> PDGid >> EventID >> TrackID >> ParentID >> Weight >> Bx >> By >> Bz >> Ex >> Ey >> Ez >> ProperTime >> PathLength >> PolX >> PolY >> PolZ >> InitX >> InitY >> InitZ >> InitT >> InitKE;
      if ( sqrt(Px*Px)<0.10*abs(Pz) )
        ntuple2t->Fill();
    }
  is1.close();


  Int_t event;
  Int_t nentries1 = (Int_t)ntuple1t->GetEntries();
  Int_t nentries2 = (Int_t)ntuple2t->GetEntries();

  cout << endl << "Entries at TOF0: " << nentries1 << endl;
  cout << "Entries at TOF1: " << nentries2 << endl;

  Float_t t_tof1, t_tof2;

  Int_t i,j;
  j = 0;
  
  
  for ( i=0; i<nentries2; i++ ) {
    ntuple2t->GetEntry(i);
    event = EventID;
    t_tof2 = t;
    
    if ( i % 1000 == 0 )
      cout << "Tof1 entry: " << i << " - j: " << j << endl;
    
    if ( sqrt(Px*Px)<0.10*abs(Pz) ) {
      
      for ( j=j; j<nentries1; j++ ) {
        ntuple1t->GetEntry(j);
	
        //if ( EventID > event )    # only if is sorted by eventid
        //  break;
	
        if ( EventID == event ) {
          //cout << "Match! " << endl;
	  
	  if ( sqrt(Px*Px)<0.10*abs(Pz) ) {
	    if ( PDGid == 211 )         // pion
	      pion_deltat.Fill(t_tof2 - t);
            else if ( PDGid == -13 )    // muon 
              muon_deltat.Fill(t_tof2 - t);
	    //            else if ( PDGid == -11 )    // positron  
	    //              electron_deltat.Fill(t_tof2 - t);
          }
          break;
        }
      }
    }
  }
  

  muon_deltat->SetLineColor(kRed);

  theMax1  = pion_deltat->GetMaximum();
  theMax2  = muon_deltat->GetMaximum();

  if (theMax1 > theMax2)  {
    pion_deltat->Draw();
    muon_deltat->Draw("sames");
  }
  else  {
    muon_deltat->Draw();
    pion_deltat->Draw("sames");
  }

  c1->SaveAs("G4BL.png");   


  
}

