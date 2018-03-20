//#define __DEBUG__
#include <iomanip>
//#include "TFile.h"
//#include "TH1D.h"
//#include "TROOT.h"
//#include "TCanvas.h"
//#include "TNtupleD.h"
//#include "TH2D.h"
//#include "JCluster.h"
//#include "TGraph.h"
//#include "JREvent.h"
//#include "TSystem.h"
//#include <sstream> 
//#include "T.h"
//#include <cstdlib>

const int kNlayers = 56;
const float kTimeBin = 50; //ns
const int kMaxWidth  = 50;
const int kMinWidth  =  4;
const int kThreshold = 3;

const int maxevent = 20000;
//const int maxevent = 20;

void deadpad_20000(const Char_t *fname = "root/readout-19985_0.root")
{
  string fileNameStr(fname);
  int n_prefix = fileNameStr.find("readout-") + 8; // 8: "readout-"
  int n_postfix = fileNameStr.find(".root") - 2;   // 2: "_0"
  int n_runnumber = n_postfix - n_prefix;
  //cerr << n_prefix << " " << n_postfix << " " << n_runnumber <<endl;
  int runid = atoi(fileNameStr.substr(n_prefix,n_runnumber).c_str());
  //cerr << runid << endl;

  gSystem->Load("libPhysics");
  gSystem->Load("libGraf3d");
  gSystem->Load("libJREvent");

  //gstyle->SetPadGridX(kTRUE);
  //gstyle->SetPadGridY(kTRUE);


  TFile *fp = new TFile(fname);
  TFile &f  = *fp;
  TTree *T  = static_cast<TTree *>(f.Get("Event"));
  JREvent *event = 0;
  //JREvent *event = static_cast<JREvent *>(gROOT->FindObject("Event"));
  //JREvent *event = JREvent::GetInstance();
  T->SetBranchAddress("Event", &event);
#if 0
  const Int_t nentries = (const Int_t)T->GetEntries();
  cerr << "nentries = " << nentries << endl;
  if (nentries>maxevent) {
    cerr << "Set higher maxevent because now we're going to process more than that!" << endl;
    cerr << "Quit." << endl;
    return;
  }
#else
   const Int_t nentries = maxevent;
#endif

  JCluster::SetSortBy(JCluster::kDet); // set sorting policy

  stringstream outfilename;
  outfilename << "root/quality.Run" << runid << ".root" << ends;
  TFile* outFile = new TFile(outfilename.str().data(),"RECREATE");
  const int ndata = 100;
  double data_val[ndata];
  TNtupleD* tup = new TNtupleD("pulse","","evt:module:row:pad:charge:length:height:time:x:y");
  TH2D* hTrackPos = new TH2D("hTrackPos","Charge-weighted hit position;x;y",180,0,180,30,0,30);
  hTrackPos -> SetMaximum(150000);
  TGraph* gChargeSumByEvent;
  int chargeSum[maxevent];
  int eventAt[maxevent];

  for (Int_t ev=0; ev<nentries; ev++) {
      T->GetEntry(ev);
 // cerr << "test : " << event->GetEntries() << endl;
      if (!(ev%100)) {
        cerr << "ev: " << ev << " evno: " << event->GetEventNumber() << " #cls: " << event->GetEntries() << endl;
      }
      event->Sort(); // sort clusters in ascending order of layers

      int npads     = 0;
      int chargeSumByEvent = 0;
      int layer     = 0;
      JCluster *cp;
      TIter next(event);
      while ((cp = static_cast<JCluster *>(next()))) {
	//cp->DebugPrint("detail");
#if 1 
        int phmax = cp->GetMaximum();
	int width = cp->GetLength();
        if (phmax < kThreshold || width < kMinWidth || width > kMaxWidth) continue;
#endif
        int module = cp->GetModuleID();
        int llayer = cp->GetLayerID();
        switch (module) {
          case 0:
            layer = llayer;
	    break;
          case 1:
            layer = llayer;
	    break;
          case 2:
            layer = llayer + 28;
	    break;
          case 3:
            layer = llayer + 28;
	    break;
          case 4:
            layer = llayer + 28;
	    break;
          case 5:
            layer = llayer + 56;
	    break;
          case 6:
            layer = llayer + 56;
	    break;
	  default:
	    continue;
	}

	
	//double x = cp->GetPosition().X(); 
	//double y = cp->GetPosition().Y(); 
	int x = cp->GetPadID(); 
	//int x = cp->GetChannelID(); 
	int y = cp->GetLayerID(); 
	double charge = double(cp->GetCharge());
	
	if(module==0){
	chargeSumByEvent += charge;
	}
	//data_val[0] = double(event->GetEventNumber()); // = ev + 1 and this is inconvenient for loops.
	data_val[0] = double(ev);
	data_val[1] = double(module);
	data_val[2] = double(layer);
	data_val[3] = double(cp->GetPadID());
	data_val[4] = charge;
	data_val[5] = double(width);
	data_val[6] = double(phmax);
	data_val[7] = cp->GetTime() * kTimeBin; // ns
	data_val[8] = x; 
	data_val[9] = y; 
        tup->Fill(data_val);
	
	if(module==0){
		hTrackPos->Fill(x,y,charge);
	}
      //if(charge==0){
	//	cerr << "Pad ID=" << x << "Layer ID=" << y << "  Deadpad" << endl;      
	//}
      	
      }

      chargeSum[ev] = chargeSumByEvent;
      eventAt[ev] = event->GetEventNumber(); 
     
  }
  gChargeSumByEvent = new TGraph(nentries,eventAt,chargeSum);
  gChargeSumByEvent->SetTitle("Total charges by event;event;ADC Sum");
  TCanvas* c_trk_pos = new TCanvas("c_trk_pos","");

  hTrackPos->GetXaxis()->SetNdivisions(190);
  hTrackPos->Draw("COLZ");

  TCanvas* c_chargeSumByEvent = new TCanvas("c_chargeSumByEvent","");
  gChargeSumByEvent->Draw("al");
#if 0
  TCanvas* c = new TCanvas("c","");
  //tup->Draw("charge","module==3&&row==1");
  double _charge;
  double _evt;
  double _row;
  tup->SetBranchAddress("charge",&_charge);
  tup->SetBranchAddress("evt"   ,&_evt);
  tup->SetBranchAddress("row"   ,&_row);
  Int_t ntupentries = tup->GetEntries();
  double hitcharges[maxevent][kNlayers];
  for (int i=0; i < maxevent; i++ ) {
    for (int j=0; j < kNlayers; j++ ) {
       hitcharges[i][j] = 0.;
    }
  }
  for (int i = 0; i < ntupentries; i++) {
    tup->GetEntry(i);
    hitcharges[int(_evt)][int(_row)] += _charge;
  }
  TH1D* hhitcharges[kNlayers];
  TCanvas* chitcharges[kNlayers];
  for (int i = 0; i < kNlayers; i++) {
    stringstream hname;
    hname << "hitcharge_row" << i << ends;
    hhitcharges[i] = new TH1D(hname.str().data(),"",100,0,10000);
    chitcharges[i] = new TCanvas(hname.str().data(),"");
    
    for (int j =0; j < maxevent; j++) {
      if (hitcharges[j][i]>0) hhitcharges[i]->Fill(hitcharges[j][i]); 
      //if (hitcharges[j][i]>100) hhitcharges[i]->Fill(hitcharges[j][i]); 
    }
    
    //hhitcharges[i]->Draw();
    hhitcharges[i]->Fit("landau");
  }

  stringstream outpdf, outpdf_open, outpdf_close;
  outpdf << "Run" << runid << ".pdf" << ends;
  outpdf_open  << "Run" << runid << ".pdf(" << ends;
  outpdf_close << "Run" << runid << ".pdf)" << ends;
  c_trk_pos->Print(outpdf_open.str().data(),"pdf");
  for (int i = 0; i < kNlayers; i++ ) {
    chitcharges[i]->Print(outpdf.str().data(),"pdf");
    if (i==kNlayers-1) chitcharges[i]->Print(outpdf_close.str().data(),"pdf");
  }
  //c_chargeSumByEvent->Print(outpdf_close.str().data(),"pdf");
#endif
}
