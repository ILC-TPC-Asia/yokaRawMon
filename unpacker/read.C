//#define __DEBUG__
#include <iomanip>

const int kMaxWidth  = 50;
const int kMinWidth  =  7;
const int kThreshold = 10;

void read(const Char_t *fname = "root/readout-19673_0.root")
{
  gSystem->Load("libPhysics");
  gSystem->Load("libJREvent");

  TFile *fp = new TFile(fname);
  TFile &f  = *fp;
  TTree *T  = static_cast<TTree *>(f.Get("Event"));
  JREvent *event = 0;
  T->SetBranchAddress("Event", &event);
  Int_t nentries = (Int_t)T->GetEntries();
  cerr << "nentries = " << nentries << endl;

  JCluster::SetSortBy(JCluster::kDet); // set sorting policy

  const int kNlayers = 85;
  TH1F *hSum[kNlayers];
  stringstream ostr;
  for (int l=0; l<kNlayers; l++) {
    ostr << "hSum_" << setw(2) << setfill('0') << l << ends;
    hSum[l] = new TH1F(ostr.str().data(),"",1000,1.,20000.);
    ostr.str("");
    ostr.clear();
  }

  for (Int_t ev=0; ev<nentries; ev++) {
      T->GetEntry(ev);
      if (!(ev%100)) {
        cerr << "ev: " << ev << " evno: " << event->GetEventNumber() << " #cls: " << event->GetEntries() << endl;
      }
      event->Sort(); // sort clusters in ascending order of layers

      int npads     = 0;
      int sum       = 0;
      int layer     = 0;
      int layerlast = 0;
      JCluster *cp;
      TIter next(event);
      while ((cp = static_cast<JCluster *>(next()))) {
#if 1 
        int phmax = cp->GetMaximum();
	int width = cp->GetLength();
        if (phmax < kThreshold || width < kMinWidth || width > kMaxWidth) continue;
#endif
        int module = cp->GetModuleID();
        int llayer = cp->GetLayerID();
        switch (module) {
          case 1:
            layer = llayer;
	    break;
          case 3:
            layer = llayer + 28;
	    break;
          case 6:
            layer = llayer + 56;
	    break;
	  default:
	    continue;
	}
	if (layer != layerlast && npads) {
	  if (layer < kNlayers) {
#ifdef __DEBUG__
            cerr << "layer = " << layerlast << " sum = " << sum << endl;
#endif
            hSum[layer]->Fill(sum);
          }
          sum   = 0;
	  npads = 0;
	}
	//cerr << cp->GetCharge() << endl;
        sum += cp->GetCharge();
	layerlast = layer;
	npads++;
#ifdef __DEBUG__
	//cp->DebugPrint("detail");
#endif
      }
     if (npads && layer < kNlayers) {
#ifdef __DEBUG__
       cerr << "layer = " << layer << " sum = " << sum << endl;
#endif
       hSum[layer]->Fill(sum);
     }
  }
  for (int l=0; l<kNlayers; l++) {
    hSum[l]->Draw();
  } 
}
