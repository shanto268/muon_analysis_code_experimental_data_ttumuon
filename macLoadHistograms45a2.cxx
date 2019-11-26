{

   std::map<std::string, TH1D*> histo1D;
   std::map<std::string, TH1D*>::iterator histo1Diter;

   std::map<std::string, TH2D*> histo2D;
   std::map<std::string, TH2D*>::iterator histo2Diter;

   string fname[500];  double duration[500]; double events8[500]; 
   // fname[235]="histWTP_435_45_90_1_2_10000000_c60.root";  duration[435]=1.; events8[435]=1;

   TFile *f1 = new TFile("zHist_AWAY45b1.root","READ");
   TFile *f2 = new TFile("zHist_TANK45b1.root","READ");
   TFile *f3 = new TFile("zHist_AWAY45c2_441_449.root","READ");
   TFile *f4 = new TFile("zHist_TANK45c2_440_451.root","READ");

   //    R415  (AWAY)
   histo1D["ntotal_1"]=(TH1D*)f1->Get("test/nNodesAll");
   double n1total=histo1D["ntotal_1"]->GetEntries();
   cout<<"  n1total="<<n1total<<endl;

   // TH1D* x1away=(TH1D*)f1->Get("watertower/muXproj");
   // TH1D* y1away=(TH1D*)f1->Get("watertower/muYproj");
   TH1D* x1away=(TH1D*)f1->Get("watertower/muXpr2");
   TH1D* y1away=(TH1D*)f1->Get("watertower/muYpr2");
   double nx1a=x1away->GetEntries();
   double ny1a=y1away->GetEntries();
   cout<<"n1total  "<<n1total<<"   nx1a "<<nx1a<<"  ny1a "<<ny1a<<endl;
   x1away->Scale(1.0/n1total);
   y1away->Scale(1.0/n1total);

   TH2D* xy1away=(TH2D*)f1->Get("watertower/muXYproj");
   xy1away->Scale(1.0/n1total);

   //    R416  (TANK)
   histo1D["ntotal_2"]=(TH1D*)f2->Get("test/nNodesAll");
   double n2total=histo1D["ntotal_2"]->GetEntries();
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

   TH1D* xr12 = x2tank;
   xr12->Divide(x1away);

   TH1D* yr12 = y2tank;
   yr12->Divide(y1away);

   TH2D* xyr12=xy2tank;
   xy2tank->Divide(xy1away);

   //    2 hit cluster  (AWAY)
   histo1D["ntotal_3"]=(TH1D*)f3->Get("test/nNodesAll");
   double n3total=histo1D["ntotal_3"]->GetEntries();
   cout<<"  n3total="<<n3total<<endl;

   TH1D* x3away=(TH1D*)f3->Get("watertower/muXproj");
   TH1D* y3away=(TH1D*)f3->Get("watertower/muYproj");
   // TH1D* x3away=(TH1D*)f3->Get("watertower/muXpr2");
   // TH1D* y3away=(TH1D*)f3->Get("watertower/muYpr2");
   double nx3a=x3away->GetEntries();
   double ny3a=y3away->GetEntries();
   cout<<"n3total  "<<n3total<<"   nx3a "<<nx3a<<"  ny3a "<<ny3a<<endl;
   x3away->Scale(1.0/n3total);
   y3away->Scale(1.0/n3total);



   //    2 hit cluster  (TANK)
   histo1D["ntotal_4"]=(TH1D*)f4->Get("test/nNodesAll");
   double n4total=histo1D["ntotal_4"]->GetEntries();
   cout<<"  n4total="<<n4total<<endl;

   TH1D* x4tank=(TH1D*)f4->Get("watertower/muXproj");
   TH1D* y4tank=(TH1D*)f4->Get("watertower/muYproj");
   // TH1D* x4tank=(TH1D*)f4->Get("watertower/muXpr2");
   // TH1D* y4tank=(TH1D*)f4->Get("watertower/muYpr2");
   double nx4a=x4tank->GetEntries();
   double ny4a=y4tank->GetEntries();
   cout<<"n1tota4  "<<n4total<<"   nx4a "<<nx4a<<"  ny4a "<<ny4a<<endl;
   x4tank->Scale(1.0/n4total);
   y4tank->Scale(1.0/n4total);

   TH1D* xr34 = x4tank;
   xr34->Divide(x3away);

   TH1D* yr34 = y4tank;
   yr34->Divide(y3away);

   TH2D* xy3away=(TH2D*)f3->Get("watertower/muXYproj");
   xy3away->Scale(1.0/n3total);

   TH2D* xy4tank=(TH2D*)f4->Get("watertower/muXYproj");
   xy4tank->Scale(1.0/n4total);

   TH2D* xyr34=xy4tank;
   xy4tank->Divide(xy3away);

   TH2D* plotA=new TH2D("plotA","Water Tank at Reese",12,-15.0,15.0,12,-15.0,15.0);
   for (int ix=1; ix<20; ix++) {
      for(int iy=1; iy<20; iy++) {
         double wt=xy4tank->GetBinContent(ix,iy);
         double x=-25.0+2.5*ix-2.5/2.0;
         double y=-25.0+2.5*iy-2.5/2.0;
         plotA->Fill(x,y,wt);
      }
   }

  plotA->GetXaxis()->SetTitle("X (m)");
  plotA->GetYaxis()->SetTitle("Y (m)");
/*
   cout<<"   Integral....   "<<endl;
   cout<<"theta 50    Tank "<<x7tank->Integral();
   cout<<"  Away "<<x5away->Integral();
   cout<<"   R "<<x7tank->Integral()/x5away->Integral();
   cout<<endl;

   cout<<"   Integral....   "<<endl;
   cout<<"theta 60    Tank "<<x8tank->Integral();
   cout<<"  Away "<<x9away->Integral();
   cout<<"   R "<<x8tank->Integral()/x9away->Integral();
   cout<<endl;

   cout<<"   Integral....   "<<endl;
   cout<<"  Away60  "<<x5away->Integral();
   cout<<"  Away50  "<<x9away->Integral();
   cout<<"   R "<<x9away->Integral()/x5away->Integral();
   cout<<endl;
*/



   cout<<"histograms are-"<<endl;
   cout<<"(415) x1away,y1away, (416)  x2tank,y2tank"<<endl;
	   cout<<"(425) x3tank,y3tank, (426)  x4away,y4away"<<endl;
   cout<<"(429) x5away,y5away for full,  x5away2, y5away2 in band"<<endl;
   cout<<"(430) x6tank,y6tank for full,  x6tank2, y6tank2 in band"<<endl;
   cout<<"(431) x7tank,y7tank for full,  x7tank2, y7tank2 in band"<<endl;
   cout<<"(434) tankup  x8tank,y8tank for full,  x8tank2, y8tank2 in band"<<endl;
   cout<<"(435) awayup  x9away,y9away for full,  x9away2, y9away2 in band"<<endl;
}
