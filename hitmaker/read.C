//#define __DEBUG__
#include <iomanip>

void read(const Char_t *fname = "../unpacker/root/readout-7052_0.root")
{
  gSystem->Load("libPhysics");
  gSystem->Load("../unpacker/libJREvent");
  gSystem->Load("libJHits");

  TFile *fp = new TFile(fname);
  TFile &f  = *fp;
  TTree *T  = static_cast<TTree *>(f.Get("Event"));
  JREvent *event = 0;
  T->SetBranchAddress("Event", &event);
  Int_t nentries = (Int_t)T->GetEntries();
  cerr << "nentries = " << nentries << endl;

  JHitMaker hitmaker;

  for (Int_t ev=0; ev<nentries; ev++) {
      T->GetEntry(ev);
      if (!(ev%100)) {
        cerr << "ev: " << ev << " evno: " << event->GetEventNumber() << " #cls: " << event->GetEntries() << endl;
      }
      cerr << " -------<Event " << ev << " >----------------" << endl;
      JCluster::SetSortBy(JCluster::kDet); // set sorting policy
      event->Sort(); // sort clusters in ascending order of layers
      hitmaker.SetClusters(event);
      hitmaker.DebugPrint("detailed");
  } 
}
