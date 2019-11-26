{

   std::map<std::string, TH1D*> histo1D;
   std::map<std::string, TH1D*>::iterator histo1Diter;

   std::map<std::string, TH2D*> histo2D;
   std::map<std::string, TH2D*>::iterator histo2Diter;

   string fname[500];  double duration[500]; double events8[500]; 
   // fname[235]="histWTP_435_45_90_1_2_10000000_c60.root";  duration[435]=1.; events8[435]=1;

   TFile *f1 = new TFile("histWTP_480_45_90_1_2_10000000_f2_1.root","READ");
   TFile *f2 = new TFile("histWTP_481_45_0_1_2_10000000_f2_1.root","READ");
   TFile *f3 = new TFile("histWTP_476_45_90_1_2_10000000_f2_1.root","READ");
   TFile *f4 = new TFile("histWTP_475_45_0_1_2_10000000_f2_1.root","READ");
   // TFile *f3 = new TFile("zHist_AWAY45c2_441_449.root","READ");
   //T File *f4 = new TFile("zHist_TANK45c2_440_451.root","READ");

   //    R415  (AWAY)
   histo1D["ntotal_1"]=(TH1D*)f1->Get("watertower/lastEvent");
   double n1total=histo1D["ntotal_1"]->GetBinContent(2);
   cout<<"  n1total="<<n1total<<endl;

   TH1D* x1away=(TH1D*)f1->Get("watertower/muXproj");
   TH1D* y1away=(TH1D*)f1->Get("watertower/muYproj");
   // TH1D* x1away=(TH1D*)f1->Get("watertower/muXpr2");
   // TH1D* y1away=(TH1D*)f1->Get("watertower/muYpr2");
   double nx1a=x1away->GetEntries();
   double ny1a=y1away->GetEntries();
   cout<<"n1total  "<<n1total<<"   nx1a "<<nx1a<<"  ny1a "<<ny1a<<endl;
   x1away->Scale(1.0/n1total);
   y1away->Scale(1.0/n1total);

   TH2D* xy1away=(TH2D*)f1->Get("watertower/muXYproj");
   xy1away->Scale(1.0/n1total);

   //    R416  (TANK)
   histo1D["ntotal_2"]=(TH1D*)f2->Get("watertower/lastEvent");
   double n2total=histo1D["ntotal_2"]->GetBinContent(2);
   cout<<"  n2total="<<n2total<<endl;

   TH1D* x2tank=(TH1D*)f2->Get("watertower/muXproj");
   TH1D* y2tank=(TH1D*)f2->Get("watertower/muYproj");
   // TH1D* x2tank=(TH1D*)f2->Get("watertower/muXpr2");
   // TH1D* y2tank=(TH1D*)f2->Get("watertower/muYpr2");
   double nx2a=x2tank->GetEntries();
   double ny2a=y2tank->GetEntries();
   cout<<"n2total  "<<n2total<<"   nx2a "<<nx2a<<"  ny2a "<<ny2a<<endl;
   x2tank->Scale(1.0/n2total);
   y2tank->Scale(1.0/n2total);

   TH2D* xy2tank=(TH2D*)f2->Get("watertower/muXYproj");
   xy2tank->Scale(1.0/n2total);

   TH1D* xr12 = (TH1D*) x2tank->Clone();
   xr12->Divide(x1away);

   TH1D* yr12 = (TH1D*) y2tank->Clone();
   yr12->Divide(y1away);

   TH2D* xyr12= (TH2D*) xy2tank->Clone();
   xy2tank->Divide(xy1away);

}
