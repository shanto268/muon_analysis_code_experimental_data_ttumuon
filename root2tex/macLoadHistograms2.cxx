{

   std::map<std::string, TH1D*> histo1D;
   std::map<std::string, TH1D*>::iterator histo1Diter;

   std::map<std::string, TH2D*> histo2D;
   std::map<std::string, TH2D*>::iterator histo2Diter;

   string fname[500];  double duration[500]; double events8[500]; 
   
   fname[440]="histWTP_440_45_2_1_2_10000000_a.root";  duration[440]=1.; events8[440]=1.;
   fname[441]="histWTP_441_45_-90_1_2_1000000000_a.root";  duration[441]=1.; events8[441]=1.;

   
   TFile *f9 = new TFile(fname[440].c_str(),"READ");
   TFile *f10 = new TFile(fname[441].c_str(),"READ");
   

   //    R440  (TANK45)
   histo1D["ntotal9"]=(TH1D*)f9->Get("test/nNodesAll");
   double n9total=histo1D["ntotal9"]->GetEntries();
   cout<<"  n9total="<<n9total<<endl;

   TH1D* x9tank=(TH1D*)f9->Get("watertower/muXproj");
   TH1D* y9tank=(TH1D*)f9->Get("watertower/muYproj");
   double nx9a=x9tank->GetEntries();
   double ny9a=y9tank->GetEntries();
   cout<<"n9total  "<<n9total<<"   nx9a "<<nx9a<<"  ny9a "<<ny9a<<endl;
   x9tank->Scale(1.0/n9total);
   y9tank->Scale(1.0/n9total);

   TH1D* x9tank2=(TH1D*)f9->Get("watertower/muXpr2");
   TH1D* y9tank2=(TH1D*)f9->Get("watertower/muYpr2");
   x9tank2->Scale(1.0/n9total);
   y9tank2->Scale(1.0/n9total);
 
   //    R441  (WT41)
   histo1D["ntotal10"]=(TH1D*)f10->Get("test/nNodesAll");
   double n10total=histo1D["ntotal10"]->GetEntries();
   cout<<"  n10total="<<n10total<<endl;

   TH1D* x10away=(TH1D*)f10->Get("watertower/muXproj");
   TH1D* y10away=(TH1D*)f10->Get("watertower/muYproj");
   double nx10a=x10away->GetEntries();
   double ny10a=y10away->GetEntries();
   cout<<"n10total  "<<n10total<<"   nx10a "<<nx10a<<"  ny10a "<<ny10a<<endl;
   x10away->Scale(1.0/n10total);
   y10away->Scale(1.0/n10total);
 
   TH1D* x10away2=(TH1D*)f10->Get("watertower/muXpr2");
   TH1D* y10away2=(TH1D*)f10->Get("watertower/muYpr2");
   x10away2->Scale(1.0/n10total);
   y10away2->Scale(1.0/n10total);


   cout<<"histograms are-"<<endl;
   cout<<"(440) tank45  x9tank,y9tank for full,  x9tank2, y9tank2 in band"<<endl;
   cout<<"(441) x10away,y10away,x10away2,y10away2 (441)"<<endl;
}
