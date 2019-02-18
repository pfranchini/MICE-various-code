// Fit the D2 measurement and compare the field with the magic spreadsheet one

void fit_d1(double current) {

  Double_t I,B;
  Double_t x[100];  
  Double_t y[100];  

  string dummy;
  Int_t i = 0;

  ifstream is;
  //is.open("measurement/center_down.txt");
  is.open("measurement/d1_up.txt");

  while (is.good())
    {
      is >> I >> B;
      y[i]=I;
      x[i]=B;

      i++;

    }
  is.close();

  cout << "New fit: " << endl;
  TGraph* graph = new TGraph(i,x,y);
  graph->Fit("pol3");
  graph->Draw("A*");

  TGraph* graph = new TGraph(i,y,x);
  graph->GetXaxis()->SetTitle("Current [A]");
  graph->GetYaxis()->SetTitle("B [T]");
  graph->SetTitle("D2");
  graph->Fit("pol3");
  graph->Draw("A*");
  c1->SaveAs("B_vs_I.png");

  // Magic Spreadsheet
  TF1 *func1=new TF1("func1","[0]+[1]*x+[2]*x^2+[3]*x^3",0,1.6);
  func1->SetParameters(0,253.91,-55.998,39.59);
  func1->SetLineColor(1);  //black
  func1->Draw();

  // Fit
  TF1 *func2=new TF1("func2","[0]+[1]*x+[2]*x^2+[3]*x^3",0,1.6);
  func2->SetParameters(-16.4174,337.503,-160.296,78.0383);
  func2->SetLineColor(2);  //red
  func2->Draw("same");

  fit(100);
  fit(142.66);
  fit(180.7);
  fit(200);
  fit(210.61);
  fit(240);
  fit(350);
  fit(142.66);  
  fit(153);
  
  cout << "Input current:" << endl;
  fit(current);

  cout << "B=1T, DeltaI=" << func1(1,0,2)-func2(1,0,2) << endl;
  cout << "B=1T, DeltaI=" << func1(1.1,0,2)-func2(1.1,0,2) << endl;
  cout << "B=1T, DeltaI=" << func1(1.2,0,2)-func2(1.2,0,2) << endl;
  cout << "B=1T, DeltaI=" << func1(1.3,0,2)-func2(1.3,0,2) << endl;

}


void fit(double current) {

  double correction;

  cout << "Magic: I= " << current << "A, B=" << func1->GetX(current,0,2) << endl;
  cout << "Fit:   I= " << current << "A, B=" << func2->GetX(current,0,2) << endl;
  
  correction= (func2->GetX(current,0,2)-func1->GetX(current,0,2))/func2->GetX(current,0,2);
  cout << correction*100 << "%" << endl;
  cout << "New current: " << current-current*correction << endl << endl;

} 
