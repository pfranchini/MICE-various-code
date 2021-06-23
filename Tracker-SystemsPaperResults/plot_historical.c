
{

  float S_Y_MIN = 98.0-0.1;
  float S_Y_MAX = 100.1;

  float H_Y_MIN = 98.0-0.1;
  float H_Y_MAX = 100.1;
  
  float TIME_MAX = 1514510000; //1514710000;

  float TIME_MIN_H, TIME_MIN_S;
  int nPoints;

  TFile *_file0 = TFile::Open("historical_analysis.root");

  TCanvas *c1 = new TCanvas("c1","Track finding efficiency",1200,900);
  c1->Divide(2,2);

  _file0->cd("helical");

  gStyle->SetTitleFontSize(0.06);

  c1->cd(1);
  gPad->SetGrid(1);
  up_efficiency->SetTitle("Helical - Upstream");
  up_efficiency->GetYaxis()->SetRangeUser(H_Y_MIN,H_Y_MAX);
  up_efficiency->GetXaxis()->SetRangeUser(1419937242,TIME_MAX);
  up_efficiency->GetXaxis()->SetTimeDisplay(1);
  up_efficiency->GetXaxis()->SetNdivisions(-503);
  up_efficiency->GetXaxis()->SetTimeFormat("%d/%m/%y %F 1970-01-01 00:00:00");   
  up_efficiency->GetYaxis()->SetTitle("Track finding efficiency");
  up_efficiency->GetXaxis()->SetTitle("Run date");
  up_efficiency->GetXaxis()->SetLabelSize(0.05);
  up_efficiency->GetYaxis()->SetLabelSize(0.05);
  up_efficiency->GetXaxis()->SetTitleSize(0.05);
  up_efficiency->GetYaxis()->SetTitleSize(0.05);
  up_efficiency->SetMarkerStyle(7);
  up_efficiency->SetMarkerColor(4);
  up_efficiency->Draw("ap");

  c1->cd(2);
  gPad->SetGrid(1);
  down_efficiency->SetTitle("Helical - Downstream");
  down_efficiency->GetYaxis()->SetRangeUser(H_Y_MIN,H_Y_MAX);
  down_efficiency->GetXaxis()->SetRangeUser(1419937242,TIME_MAX);
  down_efficiency->GetXaxis()->SetTimeDisplay(1);
  down_efficiency->GetXaxis()->SetNdivisions(-503);
  down_efficiency->GetXaxis()->SetTimeFormat("%d/%m/%y %F 1970-01-01 00:00:00");   
  down_efficiency->GetYaxis()->SetTitle("Track finding efficiency");
  down_efficiency->GetXaxis()->SetTitle("Run date");
  down_efficiency->GetXaxis()->SetLabelSize(0.05);
  down_efficiency->GetYaxis()->SetLabelSize(0.05);
  down_efficiency->GetXaxis()->SetTitleSize(0.05);
  down_efficiency->GetYaxis()->SetTitleSize(0.05);
  down_efficiency->SetMarkerStyle(7);
  down_efficiency->SetMarkerColor(4);
  down_efficiency->Draw("ap");


  _file0->cd("straight");

  c1->cd(3);
  gPad->SetGrid(1);
  up_efficiency->SetTitle("Straight - Upstream");
  up_efficiency->GetYaxis()->SetRangeUser(S_Y_MIN,S_Y_MAX);
  up_efficiency->GetXaxis()->SetRangeUser(1419937242,TIME_MAX);
  up_efficiency->GetXaxis()->SetTimeDisplay(1);
  up_efficiency->GetXaxis()->SetNdivisions(-503);
  up_efficiency->GetXaxis()->SetTimeFormat("%d/%m/%y %F 1970-01-01 00:00:00");
  up_efficiency->GetYaxis()->SetTitle("Track finding efficiency");
  up_efficiency->GetXaxis()->SetTitle("Run date");
  up_efficiency->GetXaxis()->SetLabelSize(0.05);
  up_efficiency->GetYaxis()->SetLabelSize(0.05);
  up_efficiency->GetXaxis()->SetTitleSize(0.05);
  up_efficiency->GetYaxis()->SetTitleSize(0.05);
  up_efficiency->SetMarkerStyle(7);
  up_efficiency->SetMarkerColor(4);
  up_efficiency->Draw("ap");

  c1->cd(4);
  gPad->SetGrid(1);
  down_efficiency->SetTitle("Straight - Downstream");
  down_efficiency->GetYaxis()->SetRangeUser(S_Y_MIN,S_Y_MAX);
  down_efficiency->GetXaxis()->SetRangeUser(1419937242,TIME_MAX);
  down_efficiency->GetXaxis()->SetTimeDisplay(1);
  down_efficiency->GetXaxis()->SetNdivisions(-503);
  down_efficiency->GetXaxis()->SetTimeFormat("%d/%m/%y %F 1970-01-01 00:00:00");
  down_efficiency->GetYaxis()->SetTitle("Track finding efficiency");
  down_efficiency->GetXaxis()->SetTitle("Run date");
  down_efficiency->GetXaxis()->SetLabelSize(0.05);
  down_efficiency->GetYaxis()->SetLabelSize(0.05);
  down_efficiency->GetXaxis()->SetTitleSize(0.05);
  down_efficiency->GetYaxis()->SetTitleSize(0.05);
  down_efficiency->SetMarkerStyle(7);
  down_efficiency->SetMarkerColor(4);
  down_efficiency->Draw("ap");

  c1.SaveAs("historical_analysis_plot.png");
  c1.SaveAs("historical_analysis_plot.pdf");


  ///////////////////////////////////////////////////////////////////////////

  int x_bins = 50;
  int y_bins = 50;
  TIME_MIN_H=1464134400;
  TIME_MIN_S=1436227200;

  // kBird for ROOT 5                                                                                                                                                       
  Double_t red[9]   = { 0.2082, 0.0592, 0.0780, 0.0232, 0.1802, 0.5301, 0.8186, 0.9956, 0.9764};
  Double_t green[9] = { 0.1664, 0.3599, 0.5041, 0.6419, 0.7178, 0.7492, 0.7328, 0.7862, 0.9832};
  Double_t blue[9]  = { 0.5293, 0.8684, 0.8385, 0.7914, 0.6425, 0.4662, 0.3499, 0.1968, 0.0539};
  Double_t stops[9] = { 0.0000, 0.1250, 0.2500, 0.3750, 0.5000, 0.6250, 0.7500, 0.8750, 1.0000};
  Int_t nb=255;
  TColor::CreateGradientColorTable(9, stops, red, green, blue, nb);

  gStyle->SetOptStat(0);
  TCanvas *c2 = new TCanvas("c2","Track finding efficiency",1200,900);
  c2->Divide(2,2);


  _file0->cd("helical");

  TH2F *up_efficiency_helical   = new TH2F("up_efficiency_helical","Helical - Upstream",x_bins,TIME_MIN_H,TIME_MAX, y_bins, H_Y_MIN, H_Y_MAX);
  auto nPoints = up_efficiency.GetN(); // number of points in your TGraph
  for(int i=0; i < nPoints; ++i) {
    double x,y;
    up_efficiency.GetPoint(i, x, y);
    up_efficiency_helical->Fill(x,y); 
  }

  c2.cd(1);
  up_efficiency_helical->GetXaxis()->SetTimeDisplay(1);
  up_efficiency_helical->GetXaxis()->SetTimeFormat("%d/%m/%y %F 1970-01-01 00:00:00");
  up_efficiency_helical->GetYaxis()->SetTitle("Track finding efficiency");
  up_efficiency_helical->GetXaxis()->SetTitle("Run date");
  up_efficiency_helical->SetMarkerStyle(6);
  up_efficiency_helical->SetMarkerSize(2);
  up_efficiency_helical->Draw("COLZ");


  TH2F *down_efficiency_helical   = new TH2F("down_efficiency_helical","Helical - Downstream",x_bins,TIME_MIN_H,TIME_MAX, y_bins, H_Y_MIN, H_Y_MAX);
  auto nPoints = down_efficiency.GetN(); // number of points in your TGraph
  for(int i=0; i < nPoints; ++i) {
    double x,y;
    down_efficiency.GetPoint(i, x, y);
    down_efficiency_helical->Fill(x,y); 
  }

  c2.cd(2);
  down_efficiency_helical->GetXaxis()->SetTimeDisplay(1);
  down_efficiency_helical->GetXaxis()->SetTimeFormat("%d/%m/%y %F 1970-01-01 00:00:00");
  down_efficiency_helical->GetYaxis()->SetTitle("Track finding efficiency");
  down_efficiency_helical->GetXaxis()->SetTitle("Run date");
  down_efficiency_helical->SetMarkerStyle(6);
  down_efficiency_helical->SetMarkerSize(2);
  down_efficiency_helical->Draw("COLZ");

  ////////////////////////////
  _file0->cd("straight");

  TH2F *up_efficiency_straight   = new TH2F("up_efficiency_straight","Straight - Upstream",x_bins,TIME_MIN_S,TIME_MAX, y_bins, S_Y_MIN, S_Y_MAX);
  auto nPoints = up_efficiency.GetN(); // number of points in your TGraph
  for(int i=0; i < nPoints; ++i) {
    double x,y;
    up_efficiency.GetPoint(i, x, y);
    up_efficiency_straight->Fill(x,y); 
  }

  c2.cd(3);
  up_efficiency_straight->GetXaxis()->SetTimeDisplay(1);
  up_efficiency_straight->GetXaxis()->SetTimeFormat("%d/%m/%y %F 1970-01-01 00:00:00");
  up_efficiency_straight->GetYaxis()->SetTitle("Track finding efficiency");
  up_efficiency_straight->GetXaxis()->SetTitle("Run date");
  up_efficiency_straight->SetMarkerStyle(6);
  up_efficiency_straight->SetMarkerSize(2);
  up_efficiency_straight->Draw("COLZ");


  TH2F *down_efficiency_straight   = new TH2F("down_efficiency_straight","Straight - Downstream",x_bins,TIME_MIN_S,TIME_MAX, y_bins, S_Y_MIN, S_Y_MAX);
  auto nPoints = down_efficiency.GetN(); // number of points in your TGraph
  for(int i=0; i < nPoints; ++i) {
    double x,y;
    down_efficiency.GetPoint(i, x, y);
    down_efficiency_straight->Fill(x,y); 
  }

  c2.cd(4);
  down_efficiency_straight->GetXaxis()->SetTimeDisplay(1);
  down_efficiency_straight->GetXaxis()->SetTimeFormat("%d/%m/%y %F 1970-01-01 00:00:00");
  down_efficiency_straight->GetYaxis()->SetTitle("Track finding efficiency");
  down_efficiency_straight->GetXaxis()->SetTitle("Run date");
  down_efficiency_straight->SetMarkerStyle(6);
  down_efficiency_straight->SetMarkerSize(2);
  down_efficiency_straight->Draw("COLZ");





}
