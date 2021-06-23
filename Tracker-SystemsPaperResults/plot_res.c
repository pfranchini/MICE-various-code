{

  TFile *_file0 = TFile::Open("test9962.root");

  /////// UPSTREAM //////////

  _file0->cd("upstream");

  TCanvas *c0 = new TCanvas("c0","Track reconstruction upstream",1200,600);
  c0->Divide(2,1);

  c0->cd(1);
  seed_p_residual_p->Draw("colz");


  c0->cd(2);
  seed_pt_residual_p->Draw("colz");


  TCanvas *c1 = new TCanvas("c1","Track reconstruction upstream",1200,900);
  c1->Divide(2,2);
  
  c1->cd(1);
  seed_p_residual_p_bias->SetTitle("p bias");
  seed_p_residual_p_bias->GetYaxis()->SetTitle("#Deltap");
  seed_p_residual_p_bias->GetXaxis()->SetTitle("p [MeV/c]");
  seed_p_residual_p_bias->Draw();

  c1->cd(2);
  seed_p_residual_p_resolution->SetTitle("p resolution");
  seed_p_residual_p_resolution->GetYaxis()->SetTitle("#sigma_{p}");
  seed_p_residual_p_resolution->GetXaxis()->SetTitle("p [MeV/c]");
  seed_p_residual_p_resolution->Draw();

  c1->cd(3);
  seed_pt_residual_p_bias->SetTitle("p_{t} bias");
  seed_pt_residual_p_bias->GetYaxis()->SetTitle("#Deltap_{t}");
  seed_pt_residual_p_bias->GetXaxis()->SetTitle("p [MeV/c]");
  seed_pt_residual_p_bias->Draw();

  c1->cd(4);
  seed_pt_residual_p_resolution->SetTitle("p_{t} resolution");
  seed_pt_residual_p_resolution->GetYaxis()->SetTitle("#sigma_{p_{t}}");
  seed_pt_residual_p_resolution->GetXaxis()->SetTitle("p [MeV/c]");
  seed_pt_residual_p_resolution->Draw();

  c1->SaveAs("tracker_reconstruction_upstream.png");
  c1->SaveAs("tracker_reconstruction_upstream.pdf");

  //  return

  /////// DOWNSTREAM //////////

  _file0->cd("downstream");

  TCanvas *c2 = new TCanvas("c2","Track reconstruction downstream",1200,600);
  c2->Divide(2,1);

  c2->cd(1);
  seed_p_residual_p->Draw("colz");

  c2->cd(2);
  seed_pt_residual_p->Draw("colz");


  TCanvas *c3 = new TCanvas("c3","Track reconstruction downstream",1200,900);
  c3->Divide(2,2);
  
  c3->cd(1);
  seed_p_residual_p_bias->SetTitle("p bias");
  seed_p_residual_p_bias->GetYaxis()->SetTitle("#Deltap");
  seed_p_residual_p_bias->GetXaxis()->SetTitle("p [MeV/c]");
  seed_p_residual_p_bias->Draw();

  c3->cd(2);
  seed_p_residual_p_resolution->SetTitle("p resolution");
  seed_p_residual_p_resolution->GetYaxis()->SetTitle("#sigma_{p}");
  seed_p_residual_p_resolution->GetXaxis()->SetTitle("p [MeV/c]");
  seed_p_residual_p_resolution->Draw();

  c3->cd(3);
  seed_pt_residual_p_bias->SetTitle("p_{t} bias");
  seed_pt_residual_p_bias->GetYaxis()->SetTitle("#Deltap_{t}");
  seed_pt_residual_p_bias->GetXaxis()->SetTitle("p [MeV/c]");
  seed_pt_residual_p_bias->Draw();

  c3->cd(4);
  seed_pt_residual_p_resolution->SetTitle("p_{t} resolution");
  seed_pt_residual_p_resolution->GetYaxis()->SetTitle("#sigma_{p_{t}}");
  seed_pt_residual_p_resolution->GetXaxis()->SetTitle("p [MeV/c]");
  seed_pt_residual_p_resolution->Draw();

  c3->SaveAs("tracker_reconstruction_downstream.png");
  c3->SaveAs("tracker_reconstruction_downstream.pdf");



}
