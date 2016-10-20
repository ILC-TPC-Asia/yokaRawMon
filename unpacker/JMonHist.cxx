#include "TDirectory.h"
#include "TTree.h"
#include "TFile.h"

#include "JMonEvent.h"
#include "JMonHist.h"
#include "JPed.h"
#include "JChMapper.h"

#include "JREvent.h"
#include "JCluster.h"

#include "TH1.h"

#include <cstdlib>
#include <fstream>
#include <sstream>

static const int kNrcu     = 4;
static const int kNfecrcu  = 32;
static const int kNchipfec = 8;
static const int kNchchip  = 16;
static const int kNchrcu   = kNfecrcu*kNchipfec*kNchchip;
//static const int kNtbin    = 1024;

//static const int kNMaxLayer = 16;
//static const int kNMaxPad   = 16;

static int peddata[kNrcu][kNchrcu];
static int nped    = 0;

JMonHist::JMonHist()
{
}

JMonHist::JMonHist(const string &outs)
{
  fFile = new TFile(outs.data(),"RECREATE");
  fTree = new TTree("Event","Event");
  fREvent = new JREvent;
  fTree->Branch("Event","JREvent",&fREvent,64000,2);
}

void JMonHist::SetPedestals(int nrcu, JPed **jped)
{
  //--
  //  Read pedestals
  //--
  if (!nped) {
    int nped = 0;
    for (int j=0; j<kNrcu; j++) {
      for (int ch=0; ch<kNchrcu; ch++) {
	peddata[j][ch] = jped[j]->GetPedestal(ch);
        // cout << " nped = " << nped << " ped = " << peddata[j][ch] << endl;
        nped++;
      }
    }
    cout << " nped = " << nped << endl;
  }
}

JMonHist::~JMonHist()
{
}

void JMonHist::ClearAll()
{
  fREvent->Clear(); // clear the event buffer
}


// Event by event histograms
void JMonHist::FillEvent(CLUSTER *ad)
{
#if 1
  for (int i = 0; i<ad->length; i++) { // pedestal subtraction
      ad->data[i] -= peddata[ad->rcuid][ad->channel];
  }
#endif
  TObjArray acs;
  if (ad->length > 0) fREvent->Add(new JCluster(ad, &acs));
  TIter nextacl(&acs,kIterBackward);
  JCluster* cp;
  while ((cp = static_cast<JCluster *>(nextacl()))) fREvent->Add(cp);
}

void JMonHist::WriteAll(int ev)
{
  fREvent->SetEventNumber(ev);
  //fREvent->Sort();
#ifdef __DEBUG__
  cerr << "----" << endl;
  TIter next(fREvent);
  JCluster *cp;
  while ((cp = static_cast<JCluster *>(next()))) {
    cp->DebugPrint("detail");
  }
#endif
  fTree->Fill();
}

void JMonHist::CloseFile()
{
  fTree->Write();
  fFile->Close();
}
