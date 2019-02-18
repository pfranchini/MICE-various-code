
{

  //TFile file("/data/mice/phumhf/TargetSim1/rootFiles/LM/700MeVParticlesLM11-5-18-circlecut.root");
  //TFile file("/data/mice/phumhf/TargetSim1/rootFiles/LM/800MeVParticlesLM5-5-18-circlecut.root");
  //TFile file("/data/mice/phumhf/TargetSim1/rootFiles/LM/800MeVParticlesLM26-6-18-circlecut-r<30cm-4.991E10protons.root");
  //TFile file("/data/mice/phumhf/TargetSim1/rootFiles/LM/700MeVParticlesLM26-6-18-circlecut-r<30cm-1E10protons.root");
  //TFile file("/data/mice/phumhf/TargetSim1/rootFiles/LM/LM-4.85mcut/800MeVParticlesLM26-6-18-circlecut-r<30cm-4.85mdisp-5E9protons.root");
  //TFile file("/data/mice/phumhf/TargetSim1/rootFiles/LM/800MeVParticlesLM-BLACKHOLE-27-6-18-circlecut-r<30cm-2.3E10protons.root");
  TFile file("/data/mice/phumhf/TargetSim1/rootFiles/LM/ParticlesLM26-6-18-circlecut-r<13cm-4.991E10protons.root");
  
  ofstream myfile;
  //myfile.open ("700MeVParticlesLM11-5-18-circlecut.txt");
  //myfile.open ("800MeVParticlesLM5-5-18-circlecut.txt");
  //myfile.open ("800MeVParticlesLM26-6-18-circlecut-r30cm-4.991E10protons.txt");
  //myfile.open ("700MeVParticlesLM26-6-18-circlecut-r30cm-1E10protons.root");
  //myfile.open ("800MeVParticlesLM26-6-18-circlecut-r30cm-4.85mdisp-5E9protons.txt");
  //myfile.open ("800MeVParticlesLM-BLACKHOLE-27-6-18-circlecut-r30cm-2.3E10protons.txt");
  myfile.open ("ParticlesLM26-6-18-circlecut-r13cm-4.991E10protons.txt");
  

  TTree* tree = (TTree*) file.Get("Data");
  Double_t x,y,z,px,py,pz;
  Int_t PDGId,event, dummy;

  /*
 nGen            = 0
 event           = 0
 marsId          = 0
 PDGId           = 0
 x               = 0
 y               = 0
 z               = 0
 rxy             = 0
 px              = 0
 py              = 0
 pt              = 0
 pz              = 0
 p               = 0
 totE            = 0
 ctFlight        = 0
 weight          = 0

  */

  // x y z Px Py Pz t PDGid EventID TrackID ParentID Weight Bx By Bz Ex Ey Ez ProperTime PathLength PolX PolY PolZ InitX InitY InitZ InitT InitKE
  
  tree.SetBranchAddress("x", &x);  // are cm
  tree.SetBranchAddress("y", &y);
  tree.SetBranchAddress("z", &z);
  tree.SetBranchAddress("px", &px);  // GeV
  tree.SetBranchAddress("py", &py);
  tree.SetBranchAddress("pz", &pz);
  tree.SetBranchAddress("PDGId", &PDGId);
  tree.SetBranchAddress("event", &event);

  int time = 0; // Simulate the time, otherwise would be 0 for all the particles
  
  for (int i = 0, N = tree->GetEntries(); i < N; ++i) {
    
    tree->GetEntry(i);     
    time = 0;

    myfile << " " << x*10 << " " << y*10 << " " << z*10 << " " << px*1000 << " " << py*1000 << " " << pz*1000 << " " << time << " " << PDGId << " " << event << " " << dummy << " " << dummy << " " << dummy << " " << dummy << " " << dummy << " " << dummy << " " << dummy << " " << dummy << " " << dummy << " " << dummy << " " << dummy << " " << dummy << " " << dummy << " " << dummy << " " << dummy << " " << dummy << " " << dummy << " " << dummy << " " << dummy <<"\n";
      
  }

  myfile.close();
}
