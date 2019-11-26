// root2tex program
//    This program takes histograms in a root file and insert them in
//    a latex file as "figure".  
//
//    If the number of entries of a histogram is zero (empty), then the
//    histogram will be skipped.
//
// - preparation(1): my_texfile.tex (example)
//     make sure you have a template file for latex. 
//
// - preparation(2): merve_hlist_full.txt (example)
//     create a histogram list file,
// 
//  > .ls
//  > _file0->GetDirectory("directory-name-in-root")->ls()
//
//  for example,
//  > _file0->GetDirectory("analyzeHCALSplash")->ls()
//  or (in case no directory)
//  _file0->ls()
//  
//  then add the following lines in the file.  (at least)
//
// tex_template_file   tex_template1.tex
// tex_replace  DUMMYTITLE auto
// tex_replace  DUMMYDATE  auto
// tex_replace  DUMMYAUTHOR  Merve Ince
//
//  You may comment out un-wanted histogram by //
//  of by /*  */ for range of lines as in C++ 
//
// - preparation(3):  create a subdirectory "plotsdir", i.e
//  mkdir plotsdir
//
//  all pdf files for individual histogram will be save in the subdirectory.
//
// - compile and run...
// g++ `root-config --cflags` -o root2tex_v7 root2tex_v7.cc `root-config --glibs`
// ./root2tex_v7  hlist_met_full.txt
// pdflatex plots_ntpl_HFnoise_HLTPhys17B_297723recoL1THLT_test.tex
// open plots_ntpl_HFnoise_HLTPhys17B_297723recoL1THLT_test.pdf
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <string>
#include <cassert>
#include <vector>
#include <time.h>

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "TStyle.h"

using namespace std;

// ====================================================================
class MakePlots {

public:

  MakePlots();
  ~MakePlots();
 
  void analyze(string);
  void doTFileFromArgs(string);

private:

  void readHListFile(string);
  int  processLines();
  int  doTexTemplate(int, vector<string>);
  int  doTexReplace(int, vector<string>);
  int  doTexFootnote(int, vector<string>);
  int  doComparisonMode(int, vector<string>);
  int  doTFile(int, vector<string>);
  int  doInFileChange(int, vector<string>);
  int  doTDirectoryFile(int, vector<string>);
  int  doKEY(int, vector<string>);

  void writeLatexHeader();
  void writeLatexPDFs();
  int  doLatexBlock(int, vector<string>);
  int  doLatexLine(int, vector<string>);
  void closeLatex();

  int  getHistOptions(int, vector<string> &);

  // string createTexLine(string);
  // int  doTexTemplate(int, vector<string>);

  // utility functions...
  string fix_file_name(string);
  string fix_tex_line(string);
  void parse_line(string&, vector<string> &);
  void str_replace( string &, const string &, const string & );
  string  currentDateTime();

  
  ifstream infile;  //  input tex template file.
  ofstream ofile;   //  output tex file.

  string outTexFileName;

  bool rootFileNamesFromInputArgs;
  string inTemplateFileName;
  string inHListFileName;
  string inRootFileName;
  vector<string> lines;
  bool comparisonMode;

  vector<TFile*> inRootFileList;
  int currentInFileSeq;
  TFile *inRootFile;
  string hDirectory;

  vector<string>  pdfnames;
  vector<string>  pdftitles;
  int  pdfindex;    // index of the last processed entry in pdfnames vector.

  map<string,string>  texreplace;
  string texfootnote_comment;

  bool latexHeaderDone;  // a flag to write the header only once.
};
// ====================================================================
MakePlots::MakePlots() { 
   latexHeaderDone=false;
   rootFileNamesFromInputArgs=false;
   comparisonMode=false;
}


MakePlots::~MakePlots() { }

// ====================================================================
void MakePlots::analyze(string intextfile) {

   inHListFileName=intextfile;
   cout<<"  analyze:  inText "<<inHListFileName<<endl;;

   readHListFile(inHListFileName);
   cout<<"  after readH..."<<endl;
   int status=processLines();

   cout<<"  "<<endl;
   cout<<"  "<<endl;
   cout<<"root2tex job completed ***"<<endl;
   cout<<"     next"<<endl;
   cout<<"pdflatex "<<outTexFileName<<endl;
   cout<<"     then "<<endl;
   string pdffile=outTexFileName;
   str_replace(pdffile,"tex","pdf");
   cout<<"open "<<pdffile<<endl;
   cout<<"  "<<endl;
   cout<<"  "<<endl;
};

// ====================================================================
void MakePlots::readHListFile(string){
  //  read the input text file and save lines in a vector string "lines".
  ifstream infile (inHListFileName.c_str());
  int counts=0;
  if(infile.is_open()){
     string temp;
     string hdirectory_temp;
     int skipline=0;
     int n=0;
     while (infile.good()) {
        getline(infile,temp);
        counts++;
        if(temp.size()==0) continue;
        vector<string> tokens;
        parse_line(temp, tokens);

        // cout<<"tokens size "<<tokens.size()<<endl;
        // for(int i=0; i<tokens.size();i++) {
        //   cout<<"   i="<<i<<"   ==="<<tokens[i]<<"==="<<endl;
        //}

        if(tokens[0].compare(0,2,"//")==0) skipline=1;
        if(tokens[0].compare(0,2,"/*")==0) skipline=20;
        if(tokens[0].compare(0,2,"*/")==0) skipline=21;
        
        if(skipline==0) {
           lines.push_back(temp);  // save input line...
        }

        if(skipline==1 || skipline==21) skipline=0;
     }
  }
  cout<<" "<<counts<<" lines read from file "<<inHListFileName<<endl;
  cout<<" "<<lines.size()<<" linse saved in vector<string> lines."<<endl;
};

// ====================================================================
int MakePlots::processLines(){
  
   if(lines.size()==0) { 
      cout<<"Errorr:  processLines- no line to process in input hlist file"<<endl;
      return -999;
   }

   pdfindex=-1;   // index of the last processed entry in pdfnames vector.

   int nskip=0;
   for(int it=0; it<lines.size(); it++) {
      if(nskip>0) {nskip--; continue;}
      vector<string> tokens;
      parse_line(lines[it], tokens);
      if(tokens.size()==0) continue;
      nskip=-1000;
      // cout<<" it="<<it<<"  "<<lines[it]<<endl;
      if(tokens[0].compare(0,1,"*")==0) {nskip=0;}  // a cooment line. do nothing
      if(tokens[0].compare(0,3,"OBJ")==0) {nskip=0;}  // OBJ line. do nothing
      if(tokens[0].compare(0,17,"tex_template_file")==0) {nskip=doTexTemplate(it,tokens);}
      if(tokens[0].compare(0,11,"tex_replace")==0) {nskip=doTexReplace(it,tokens);}
      if(tokens[0].compare(0,12,"tex_footnote")==0) {nskip=doTexFootnote(it,tokens);}
      if(tokens[0].compare(0,15,"comparison_mode")==0) {nskip=doComparisonMode(it,tokens);}
      if(tokens[0].compare(0,6,"TFile*")==0) {nskip=doTFile(it,tokens);}
      if(tokens[0].compare(0,12,"InFileChange")==0) {nskip=doInFileChange(it,tokens);}
      if(tokens[0].compare(0,15,"TDirectoryFile*")==0) {nskip=doTDirectoryFile(it,tokens);}
      // if(tokens[0].compare(0,3,"KEY")==0) {nskip=doKEY(it,tokens);}
      if(tokens[0].compare(0,3,"KEY")==0) {
         if(comparisonMode) {
            for (int n=0; n<2; n++) {
               inRootFile=inRootFileList[n];
               currentInFileSeq=n;  
               nskip=doKEY(it,tokens);             
            }
         }
         else { 
            nskip=doKEY(it,tokens);
         }
      }
      //  a block of Latex lines which ends with LatexEnd
      if(tokens[0].compare(0,15,"LatexBlockBegin")==0) {nskip=doLatexBlock(it,tokens);}
      //  single Latex line (inline).
      if(tokens[0].compare(0,9,"LatexLine")==0) {nskip=doLatexLine(it,tokens);}

      if(nskip<-900) {
         cout<<"  "<<endl;
         cout<<"Error in the input histrogram list file.  stop the program"<<endl;
         cout<<" Line Number="<<it<<endl;
         cout<<" "<<lines[it]<<endl;
         return nskip;
      }
   }

   closeLatex();

   return 0;
};

// ====================================================================
int MakePlots::doTexFootnote(int it, vector<string> tokens) {

   string words;
   for(int i=1; i<tokens.size(); i++) {
      words=words+tokens[i]+" ";
   }

   texfootnote_comment=words;

   return 0;
};
// ====================================================================
int MakePlots::doTexReplace(int it, vector<string> tokens) {
   if(tokens.size()<3) { 
      cout<<"Define a tex line replacement pair:  key word and words to replace."<<endl;
      return -999;
   }

   string words;
   for(int i=2; i<tokens.size(); i++) {
      words=words+tokens[i]+" ";
   }
   texreplace[tokens[1]]=words;
   // cout<<"  tex replace (key)="<<tokens[1]<<endl;
   // cout<<"  tex replace (words)"<<words<<endl;

   return 0;
};

// ====================================================================
int MakePlots::doTexTemplate(int it, vector<string> tokens) {
   if(tokens.size()<2) {
      cout<<"Define tex template file name for a key word- tex_template_file"<<endl;
      return -999;
   }
   inTemplateFileName=tokens[1];
   return 0;
}

// ====================================================================
int MakePlots::doComparisonMode(int it, vector<string> tokens) {

   comparisonMode=false;
   if(tokens[1].compare(0,4,"true")==0) {comparisonMode=true;} 

   return 0;
};

// ====================================================================
int MakePlots::doTFile(int it, vector<string> tokens) {
      // if(inRootFileName.size()==0) inRootFileName=tokens[1];
      if(rootFileNamesFromInputArgs) return 0;
      inRootFileName=tokens[1];
      inRootFile=new TFile(inRootFileName.c_str());
      inRootFileList.push_back(inRootFile);
      currentInFileSeq=inRootFileList.size()-1;
      cout<<"open root file("<<inRootFileList.size()<<")="<<inRootFileName<<"   address="<<inRootFile<<endl;
   return 0;
};

// ====================================================================
void MakePlots::doTFileFromArgs(string inName) {
      rootFileNamesFromInputArgs=true;
      inRootFileName=inName;
      inRootFile=new TFile(inRootFileName.c_str());
      inRootFileList.push_back(inRootFile);
      currentInFileSeq=inRootFileList.size()-1;
      cout<<"open root file("<<inRootFileList.size()<<")="<<inRootFileName<<"   address="<<inRootFile<<endl;
   return ;
};

// ====================================================================
int MakePlots::doInFileChange(int it, vector<string> tokens) {
      if(inRootFileName.size()==0) inRootFileName=tokens[1];
      int n=std::stoi(tokens[1]);
      inRootFile=inRootFileList[n];
      currentInFileSeq=n;
      std::cout<<"InFileChange:  n="<<n<<"    "<<inRootFile<<std::endl;
   return 0;
};

// ====================================================================
int MakePlots::doTDirectoryFile(int it, vector<string> tokens) {
      hDirectory=tokens[1];
      cout<<"hDirectory="<<hDirectory<<endl;
   return 0;
};

// ====================================================================
int MakePlots::doKEY(int it, vector<string> tokens) {
      // cout<<"doKey:  begin"<<endl;

      if(tokens.size()<3) return 0;

      // get a histogram name and create a pdf file name...
      string slash="/";  if(hDirectory.size()==0) slash="";
      string hname=hDirectory+slash+tokens[2];
      // string hname=hDirectory+"/"+tokens[2];
      //string pdfname_temp=tokens[2]+".pdf";

      //cout<<"doKEY: hname="<<hname<<endl;
      //cout<<"hDirectory aize="<<hDirectory.size()<<endl;

      string pdfname_temp=hname+".pdf";
      str_replace(pdfname_temp,"/","-");
      str_replace(pdfname_temp,";","-");
      str_replace(pdfname_temp,"#","N");

      // check next lines for histogram options...
      vector<string> hoptions;
      int nskip=getHistOptions(it,hoptions);

      if(hoptions.size()>0) {
         cout<<"   doKEY:  nskip="<<nskip<<" noptions "<<hoptions.size()<<endl;
         for(int k=0; k<hoptions.size(); k++) {
            cout<<" "<<hoptions[k]<<endl;
         }
      }

      // get the histogram
      string cname=tokens[2];
      str_replace(cname,";","-");
      TCanvas* c1 = new TCanvas(cname.c_str(), cname.c_str() , 900, 600);
       // gStyle->SetOptStat(0);
       // gPad->SetLogz();

      int found=0;
      int nentries=0;
      if(tokens[1].compare(0,3,"TH1")==0) {
         found=1;
         int optstat=111111;
         if(hoptions.size()>0) {
            for(int k=0; k<hoptions.size(); k++) {
               if(hoptions[k].compare(0,4,"logy")==0) {c1->SetLogy(1);}
               if(hoptions[k].compare(0,4,"logx")==0) {c1->SetLogx(1);}
               if(hoptions[k].compare(0,7,"optstat")==0) {
                   if ( ! (istringstream(hoptions[k+1]) >> optstat) ) optstat = 0;}
            }
         }
         gStyle->SetOptStat(optstat);
         inRootFile->cd();
         gDirectory->pwd();
         TH1 *h1 = (TH1*) inRootFile->Get(hname.c_str());
         if(h1!=NULL) {
            h1->Draw();
            nentries=h1->GetEntries();
         }
      }
      
      if(tokens[1].compare(0,3,"TH2")==0) {
         found=1;
         string drawoption;
         int optstat=0;
         if(hoptions.size()>0) {
            for(int k=0; k<hoptions.size(); k++) {
               if(hoptions[k].compare(0,3,"box")==0) {drawoption="box";}
               if(hoptions[k].compare(0,4,"colz")==0) {drawoption="colz";}
               if(hoptions[k].compare(0,7,"optstat")==0) {
                   if ( ! (istringstream(hoptions[k+1]) >> optstat) ) optstat = 0;}
            }
         }
         gStyle->SetOptStat(optstat);
         TH2 *h2 = (TH2*) inRootFile->Get(hname.c_str());
         if(h2!=NULL) {
            if(drawoption.size()>0) {h2->Draw(drawoption.c_str());}
            else {h2->Draw("colz");}
            nentries=h2->GetEntries();
         }
      }

      if(tokens[1].compare(0,8,"TProfile")==0) {
         found=1;
         TProfile *hp = (TProfile*) inRootFile->Get(hname.c_str());
         if(hp!=NULL) {
            hp->Draw();
            nentries=hp->GetEntries();
         }
      }

      if(found==0) return 0;
      if(nentries==0) return 0;

      // **** add sub-directory name ****

      string n=std::to_string(currentInFileSeq);
      pdfname_temp="plotsdir/file"+n+"_"+pdfname_temp;

      // ********************************


      c1->SaveAs(pdfname_temp.c_str());
      delete c1;

      
      string htitle_temp;
      if(tokens.size()<4) {
          htitle_temp=" ";
       } else {
          for(int k=3; k<tokens.size(); k++) {
              htitle_temp=htitle_temp+tokens[k]+" ";
          }
       }
       

       // string htitle_temp=tokens[2]; 
       // string htitle_temp=tokens[3]; 

       pdfnames.push_back(pdfname_temp);
       pdftitles.push_back(htitle_temp);

       cout<<"pdfname_temp==="<<pdfname_temp<<"====="<<endl;
       cout<<"htitle_temp==="<<htitle_temp<<"====="<<endl;

   return nskip;
};

// ====================================================================
void MakePlots::writeLatexHeader() {

  if(latexHeaderDone) return;
     latexHeaderDone=true;

  if(pdfindex != -1) return;

  infile.open(inTemplateFileName.c_str());
  cout<<"tex template file= "<<inTemplateFileName<<endl;
  if(infile.is_open()==0) {
    cout<<"   Error:  The tex template file name may be wrong. Please check"<<endl;
    cout<<"   Error:  The tex template file name may be wrong. Please check"<<endl;
    cout<<"   Error:  The tex template file name may be wrong. Please check"<<endl;
    cout<<"   Error:  The tex template file name may be wrong. Please check"<<endl;
    cout<<"   Error:  The tex template file name may be wrong. Please check"<<endl;
    cout<<"   Error:  The tex template file name may be wrong. Please check"<<endl;
  }
  string  newTexFile="plots_"+inRootFileName;
  str_replace(newTexFile,".root",".tex");
  cout<<"new TexFile="<<newTexFile;
  outTexFileName=newTexFile;
  ofile.open(newTexFile);

  string title_temp=inRootFileName;
  string date_temp=currentDateTime();
  string footnote_temp=title_temp+" / "+date_temp;;
  if(texfootnote_comment.size()>0) footnote_temp=footnote_temp+" / "+texfootnote_comment;

  title_temp=fix_tex_line(title_temp);
  date_temp=fix_tex_line(date_temp);
  footnote_temp=fix_tex_line(footnote_temp);

  if(infile.is_open()){
     cout<<"   tex_template is open"<<endl;
     string temp;
     while (infile.good()) {
        getline(infile,temp);
        // cout<<temp<<endl;
        if(texreplace.size()>0) {
          for(map<string,string>::iterator ii=texreplace.begin();
           ii!=texreplace.end(); ii++) {
             string key=(*ii).first;
             string words=(*ii).second;
             if(words.compare(0,4,"auto")==0) continue;
             str_replace(temp,key,words);
          }
        }
        str_replace(temp,"DUMMYTITLE",title_temp);
        str_replace(temp,"DUMMYDATE",date_temp);
        str_replace(temp,"DUMMYFOOTNOTE",footnote_temp);

        string temp2=temp+"\n";
        ofile<<temp2;
     }
  }
};

// ====================================================================
void MakePlots::writeLatexPDFs() {

  int ntotal=pdfnames.size();
  if((ntotal-pdfindex)<2) return;   //  all pdf files have been processed

  int nmax=3;    // max number of plots in a raw
  int npageclear=15;
  int row_counts=0;
  int ixtitles[7];
  string label[]={"(a)","~~~(b) ","~~~(c)","~~~(d)","~~~(e)","~~~(f) ","~~~(h)"};

  // string begin_figure="\\begin{figure}[h] \n";
  string begin_figure="\\begin{figure}[H] \n";
  string vspace_figure="\\vspace*{-0.3cm} \n";
  string include_graph;
  if(nmax==2) {
     include_graph="\\includegraphics[width=0.42\\textwidth,scale=0.5,trim=0 0 0 0,clip]{";
  }
  if(nmax==3) {
     include_graph="\\includegraphics[width=0.33\\textwidth,scale=0.5,trim=0 0 0 0,clip]{";
  }
  string end_figure="\\end{figure} \n";
  string clear_page="\\clearpage \n";

  // ofile<<new_page;
  int nplots=0;;
  int icounts=-1;
  int ibegin=pdfindex+1;
  for(int i=ibegin; i<ntotal; i++) {
    pdfindex=i;    // update the pdfindex..
    icounts++;
    if((icounts%nmax)==0) {
        ofile<<begin_figure;
        ofile<<vspace_figure;
        nplots=0;
    }

    string  graph=include_graph+pdfnames[i]+"} \n";
    ofile<<graph;

    ixtitles[nplots]=i;
    nplots++;

    if((((icounts+1)%nmax)==0) || (i+1)==ntotal) {
       string caps;
       for (int k=0; k<nplots; k++) {
           int kx=ixtitles[k];
           caps=caps+label[k]+pdftitles[kx];
           // caps=caps+label[k];
       }
       //   do not put fugure captions for now...  They look agree for our input.
       string newcaps=fix_tex_line(caps);
       // cout<<"    in makePDF  newcaps="<<newcaps<<endl;
       ofile<<"\\caption{"<<newcaps<<"} \n";
       ofile<<end_figure;
       // clear page to process "floats"...
       row_counts++;
       if((row_counts%npageclear)==0) ofile<<clear_page;
    }
  }  // end of loop over plots...

};

// ====================================================================
int MakePlots::doLatexLine(int it, vector<string> tokens) {

   //cout<<"MakePlots::doLatexLine is called..."<<endl;
   if(tokens.size()<1) return 0;

    //  write Header and PDF(plots) first...
    writeLatexHeader();
    writeLatexPDFs();

    string buf;
    for(int i=1; i<tokens.size(); i++) {
      buf=buf+tokens[i]+" ";
    }    

    buf=fix_tex_line(buf);
    ofile<<buf+" \n";

    // cout<<"  doLatexLine: =="<<(buf+" \n")<<endl;

    return 0;
};

// ====================================================================
int MakePlots::doLatexBlock(int it, vector<string> tokens) {

   // cout<<"   MakePlots::doLatexBlock is clled "<<endl;
   if(tokens.size()<1) return 0;

    //  write Header and PDF(plots) first...
    writeLatexHeader();
    writeLatexPDFs();
   
    int nskips=0; 
    for (int kt=it+1; kt<lines.size(); kt++) {
       nskips++;
       vector<string> newtokens;
       parse_line(lines[kt], newtokens);
       if(newtokens.size()==0) continue;
       if(newtokens[0].compare(0,13,"LatexBlockEnd")==0) { break;}
       // cout<<"MakePlots::doLatexBlock:"<<lines[kt]<<endl;
       ofile<<lines[kt]+" \n";
    } 

    return nskips;
};

// ====================================================================
void MakePlots::closeLatex() {

  writeLatexHeader();
  writeLatexPDFs();

  string end_doc="\\end{document} \n";
  ofile<<end_doc;
  ofile.close();

};

// ====================================================================
int  MakePlots::getHistOptions(int it, vector<string> & hoptions){
      int nskip=0;      
      // cout<<"  getHistOptions begins "<<endl;
      if(lines.size()>it) {
         for(int jt=it+1; jt<lines.size(); jt++) {
            vector<string> nexttokens;
            parse_line(lines[jt], nexttokens);
            // cout<<"next line="<<lines[jt]<<endl;
            // cout<<"  next tokens size="<<nexttokens.size()<<endl;
            if(nexttokens.size()==0) {
               // skip a blank line...
               nskip++; 
               continue;
            }
            if(nexttokens[0].compare(0,3,"opt")==0) {
               nskip++;
               if(nexttokens.size()>1) {
                  for(int k=1; k<nexttokens.size(); k++) {
                     hoptions.push_back(nexttokens[k]);
                  }
               }
            } 
            else { break;}   // break if key word is not "opt"...
         }
       }

   return nskip;
};

// ====================================================================
// ====================================================================
// ====================================================================

// ====================================================================
string MakePlots::fix_file_name(string aLine) {
   string newLine;

   //  step through characters in the string...
   if(aLine.size()>0) {
      for(int i=0; i<aLine.size(); i++) {
         string x=aLine.substr(i,1);
         string y;
         if (x.compare(0,1,"#")==0) { y="N";}
         else { y=x;}
         newLine=newLine+y;
      }   // end of for-loop through string.
   }

   // cout<<"   aLine="<<aLine<<endl;
   // cout<<" newLine="<<newLine<<endl;

   return newLine;
}

// ====================================================================
string MakePlots::fix_tex_line(string aLine) {
   string newLine;

   //  step through characters in the string...
   if(aLine.size()>0) {
      for(int i=0; i<aLine.size(); i++) {
         string x=aLine.substr(i,1);
         string y;
         if (x.compare(0,1,"#")==0) { y="$\\varhash$";}
         else if(x.compare(0,1,"\\")==0) { y="\\";}
         else if(x.compare(0,1,"_")==0) { y="\\_";}
         else { y=x;}
         newLine=newLine+y;
      }   // end of for-loop through string.
   }

   // cout<<"   aLine="<<aLine<<endl;
   // cout<<" newLine="<<newLine<<endl;

   return newLine;
}

// ====================================================================
void MakePlots::parse_line(string & line, vector<string> & tokens){

    string buf; // Have a buffer string
    stringstream ss(line); // Insert the string into a stream

    if(line.size()==0) return;

    // vector<string> tokens; // Create vector to hold our words

    while (ss >> buf)
        tokens.push_back(buf);

    return ;
};

// ===================================================================
void MakePlots::str_replace( string &s, const string &search, const string &replace )
{
    for( size_t pos = 0; ; pos += replace.length() )
        {
        pos = s.find( search, pos );
        if( pos == string::npos ) break;

        s.erase( pos, search.length() );
        s.insert( pos, replace );
    }
}


// ====================================================================
// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
string MakePlots::currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

// ====================================================================
// ====================================================================
// ====================================================================
int main( int argc, char **argv ) {

  MakePlots makeplots;

  std::cout<<"  starting myplots...  "<<std::endl;

  if ( argc < 2 ) {
    std::cerr << "You must provide an input text file" << std::endl;
    // std::cerr << "prog input.lhe output.root" << std::endl;
    return 0;
  }

  // std::string inputHListFile="inputHistogramList.txt";  
  // std::string outName="myplots01.root";;

  std::string inHListFileName(argv[1]);
  std::cout<<"  main: input text file  ="<<inHListFileName<<std::endl;

  std::cout<<"  argc="<<argc<<std::endl;
  if(argc>2) {
     for(int i=2; i<argc; i++) {
        string inRootFileName=argv[i];
        std::cout<<"  main: input root file  ="<<inRootFileName<<std::endl;
        makeplots.doTFileFromArgs(inRootFileName);
     }
  }

  makeplots.analyze(inHListFileName);

}
