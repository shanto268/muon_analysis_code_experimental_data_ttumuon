{

   std::map<std::string, TH1D*> histo1D;
   std::map<std::string, TH1D*>::iterator histo1Diter;

   std::map<std::string, TH2D*> histo2D;
   std::map<std::string, TH2D*>::iterator histo2Diter;

   string fname[500];  double duration[500]; double events8[500]; 
   fname[415]="histWTP_415_50_90_1_2_100000000_.root";  duration[415]=415.; events8[415]=1029289;
   fname[416]="histWTP_416_50_0_1_2_10000000_a.root";  duration[416]=415.; events8[416]=1911937;

   fname[425]="histWTP_425_50_0_1_2_10000000_b.root";  duration[425]=415.; events8[425]=1659256;
   fname[426]="histWTP_426_50_90_1_2_10000000_a.root";  duration[426]=415.; events8[426]=1060551;
   fname[429]="histWTP_429_50_90_1_2_10000000_a.root";  duration[429]=1.; events8[429]=1;
   fname[430]="histWTP_430_50_0_1_2_10000000_a.root";  duration[430]=1.; events8[430]=1;
   fname[431]="histWTP_431_50_0_1_2_10000000_a.root";  duration[431]=1.; events8[431]=1;
   fname[434]="histWTP_434_60_0_1_2_10000000_a.root";  duration[434]=1.; events8[434]=1;
   fname[435]="histWTP_435_60_90_1_2_10000000_a.root";  duration[435]=1.; events8[435]=1;

   TFile *f1 = new TFile(fname[415].c_str(),"READ");
   TFile *f2 = new TFile(fname[416].c_str(),"READ");
   TFile *f3 = new TFile(fname[425].c_str(),"READ");
   TFile *f4 = new TFile(fname[426].c_str(),"READ");
   TFile *f5 = new TFile(fname[429].c_str(),"READ");
   TFile *f6 = new TFile(fname[430].c_str(),"READ");
   TFile *f7 = new TFile(fname[431].c_str(),"READ");
   TFile *f8 = new TFile(fname[434].c_str(),"READ");
   TFile *f9 = new TFile(fname[435].c_str(),"READ");

   //    R415  (AWAY)
   histo1D["ntotal1"]=(TH1D*)f1->Get("test/nNodesAll");
   double n1total=histo1D["ntotal1"]->GetEntries();
   cout<<"  n1total="<<n1total<<endl;

   TH1D* x1away=(TH1D*)f1->Get("watertower/muXproj");
   TH1D* y1away=(TH1D*)f1->Get("watertower/muYproj");
   double nx1a=x1away->GetEntries();
   double ny1a=y1away->GetEntries();
   cout<<"n1total  "<<n1total<<"   nx1a "<<nx1a<<"  ny1a "<<ny1a<<endl;
   x1away->Scale(1.0/n1total);
   y1away->Scale(1.0/n1total);

   //    R416  (TANK)
   histo1D["ntotal2"]=(TH1D*)f2->Get("test/nNodesAll");
   double n2total=histo1D["ntotal2"]->GetEntries();
   cout<<"  n1total="<<n2total<<endl;

   TH1D* x2tank=(TH1D*)f2->Get("watertower/muXproj");
   TH1D* y2tank=(TH1D*)f2->Get("watertower/muYproj");
   double nx2a=x2tank->GetEntries();
   double ny2a=y2tank->GetEntries();
   cout<<"n1tota2  "<<n2total<<"   nx2a "<<nx2a<<"  ny2a "<<ny2a<<endl;
   x2tank->Scale(1.0/n2total);
   y2tank->Scale(1.0/n2total);

   //    R425  (TANK)
   histo1D["ntotal3"]=(TH1D*)f3->Get("test/nNodesAll");
   double n3total=histo1D["ntotal3"]->GetEntries();
   cout<<"  n3total="<<n3total<<endl;

   TH1D* x3tank=(TH1D*)f3->Get("watertower/muXproj");
   TH1D* y3tank=(TH1D*)f3->Get("watertower/muYproj");
   double nx3a=x3tank->GetEntries();
   double ny3a=y3tank->GetEntries();
   cout<<"n3total  "<<n3total<<"   nx3a "<<nx3a<<"  ny3a "<<ny3a<<endl;
   x3tank->Scale(1.0/n3total);
   y3tank->Scale(1.0/n3total);

   //    R426  (AWAY)
   histo1D["ntotal4"]=(TH1D*)f4->Get("test/nNodesAll");
   double n4total=histo1D["ntotal4"]->GetEntries();
   cout<<"  n4total="<<n4total<<endl;

   TH1D* x4away=(TH1D*)f4->Get("watertower/muXproj");
   TH1D* y4away=(TH1D*)f4->Get("watertower/muYproj");
   double nx4a=x4away->GetEntries();
   double ny4a=y4away->GetEntries();
   cout<<"n4total  "<<n4total<<"   nx4a "<<nx4a<<"  ny4a "<<ny4a<<endl;
   x4away->Scale(1.0/n4total);
   y4away->Scale(1.0/n4total);



   //    R429   (AWAY)
   histo1D["ntotal5"]=(TH1D*)f5->Get("test/nNodesAll");
   double n5total=histo1D["ntotal5"]->GetEntries();
   cout<<"  n5total="<<n5total<<endl;

   TH1D* x5away=(TH1D*)f5->Get("watertower/muXproj");
   TH1D* y5away=(TH1D*)f5->Get("watertower/muYproj");
   double nx5a=x5away->GetEntries();
   double ny5a=y5away->GetEntries();
   cout<<"n5total  "<<n5total<<"   nx5a "<<nx5a<<"  ny5a "<<ny5a<<endl;
   x5away->Scale(1.0/n5total);
   y5away->Scale(1.0/n5total);
 
   TH1D* x5away2=(TH1D*)f5->Get("watertower/muXpr2");
   TH1D* y5away2=(TH1D*)f5->Get("watertower/muYpr2");
   x5away2->Scale(1.0/n5total);
   y5away2->Scale(1.0/n5total);

   //    R430  (TANK)
   histo1D["ntotal6"]=(TH1D*)f6->Get("test/nNodesAll");
   double n6total=histo1D["ntotal6"]->GetEntries();
   cout<<"  n6total="<<n6total<<endl;

   TH1D* x6tank=(TH1D*)f6->Get("watertower/muXproj");
   TH1D* y6tank=(TH1D*)f6->Get("watertower/muYproj");
   double nx6a=x6tank->GetEntries();
   double ny6a=y6tank->GetEntries();
   cout<<"n6total  "<<n6total<<"   nx6a "<<nx6a<<"  ny6a "<<ny6a<<endl;
   x6tank->Scale(1.0/n6total);
   y6tank->Scale(1.0/n6total);

   TH1D* x6tank2=(TH1D*)f6->Get("watertower/muXpr2");
   TH1D* y6tank2=(TH1D*)f6->Get("watertower/muYpr2");
   x6tank2->Scale(1.0/n6total);
   y6tank2->Scale(1.0/n6total);

   //    R431  (TANK)
   histo1D["ntotal7"]=(TH1D*)f7->Get("test/nNodesAll");
   double n7total=histo1D["ntotal7"]->GetEntries();
   cout<<"  n7total="<<n7total<<endl;

   TH1D* x7tank=(TH1D*)f7->Get("watertower/muXproj");
   TH1D* y7tank=(TH1D*)f7->Get("watertower/muYproj");
   double nx7a=x7tank->GetEntries();
   double ny7a=y7tank->GetEntries();
   cout<<"n7total  "<<n7total<<"   nx7a "<<nx7a<<"  ny7a "<<ny7a<<endl;
   x7tank->Scale(1.0/n7total);
   y7tank->Scale(1.0/n7total);

   TH1D* x7tank2=(TH1D*)f7->Get("watertower/muXpr2");
   TH1D* y7tank2=(TH1D*)f7->Get("watertower/muYpr2");
   x7tank2->Scale(1.0/n7total);
   y7tank2->Scale(1.0/n7total);

   //    R434  (TANKup)
   histo1D["ntotal8"]=(TH1D*)f8->Get("test/nNodesAll");
   double n8total=histo1D["ntotal8"]->GetEntries();
   cout<<"  n8total="<<n8total<<endl;

   TH1D* x8tank=(TH1D*)f8->Get("watertower/muXproj");
   TH1D* y8tank=(TH1D*)f8->Get("watertower/muYproj");
   double nx8a=x8tank->GetEntries();
   double ny8a=y8tank->GetEntries();
   cout<<"n8total  "<<n8total<<"   nx8a "<<nx8a<<"  ny8a "<<ny8a<<endl;
   x8tank->Scale(1.0/n8total);
   y8tank->Scale(1.0/n8total);

   TH1D* x8tank2=(TH1D*)f8->Get("watertower/muXpr2");
   TH1D* y8tank2=(TH1D*)f8->Get("watertower/muYpr2");
   x8tank2->Scale(1.0/n8total);
   y8tank2->Scale(1.0/n8total);

   //    R435  (AWAY60))
   histo1D["ntotal9"]=(TH1D*)f9->Get("test/nNodesAll");
   double n9total=histo1D["ntotal9"]->GetEntries();
   cout<<"  n9total="<<n9total<<endl;

   TH1D* x9away=(TH1D*)f9->Get("watertower/muXproj");
   TH1D* y9away=(TH1D*)f9->Get("watertower/muYproj");
   double nx9a=x9away->GetEntries();
   double ny9a=y9away->GetEntries();
   cout<<"n9total  "<<n9total<<"   nx9a "<<nx9a<<"  ny9a "<<ny9a<<endl;
   x9away->Scale(1.0/n9total);
   y9away->Scale(1.0/n9total);

   TH1D* x9away2=(TH1D*)f9->Get("watertower/muXpr2");
   TH1D* y9away2=(TH1D*)f9->Get("watertower/muYpr2");
   x9away2->Scale(1.0/n9total);
   y9away2->Scale(1.0/n9total);

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




   cout<<"histograms are-"<<endl;
   cout<<"(415) x1away,y1away, (416)  x2tank,y2tank"<<endl;
	   cout<<"(425) x3tank,y3tank, (426)  x4away,y4away"<<endl;
   cout<<"(429) x5away,y5away for full,  x5away2, y5away2 in band"<<endl;
   cout<<"(430) x6tank,y6tank for full,  x6tank2, y6tank2 in band"<<endl;
   cout<<"(431) x7tank,y7tank for full,  x7tank2, y7tank2 in band"<<endl;
   cout<<"(434) tankup  x8tank,y8tank for full,  x8tank2, y8tank2 in band"<<endl;
   cout<<"(435) awayup  x9away,y9away for full,  x9away2, y9away2 in band"<<endl;
}
