#define SEP_2010
#include "GEMHist.h"
#include "TDirectory.h"
#include "JREvent.h"
#include "JCluster.h"

#include "TH1.h"

#include <cstdlib>
#include <fstream>
#include <sstream>

static const int kMaxHist  = 100000;
static const int kNtbin    = 1024;
#ifdef SEP_2010
//-- 2010/09/13 DESY Beam Test ----------------------------------------
static const int kNmodules   = 3;
static const int kModuleNo[] = {0, 3, 5};  // (0, 3, 5) for the moment
static const int kOffset  [] = {0, 128, 48};  // (0, 3, 5) for the moment
static const int kNMaxLayer  = 28;         // (0-27)
static const int kNMaxPad    = 48;         // (0-47)
//---------------------------------------------------------------------
#else
//-- 2009/03/03 DESY Beam Test ----------------------------------------
static const int kNmodules   = 3;
static const int kModuleNo[] = {1, 3, 6};  // (1, 3, 6) for the moment
static const int kOffset  [] = {0, 128, 48};  // (1, 3, 6) for the moment
static const int kNMaxLayer  = 28;         // (0-27)
static const int kNMaxPad    = 48;         // (0-47)
//---------------------------------------------------------------------
#endif

GEMHist *GEMHist::fgInstancePtr = 0;

ClassImp(GEMHist)

GEMHist::GEMHist()
{
  htime = new TH1F*[kMaxHist];
  for (int h=0; h<kMaxHist; h++) htime[h] = 0;
  //--
  //  Define histograms
  //--
  for (int m=0; m<kNmodules; m++) {
    int module = kModuleNo[m];
    for (int layer=0; layer<kNMaxLayer; layer++) {
      for (int p=0; p<kNMaxPad; p++) {
        int pad = p + kOffset[m];
        stringstream hname;
        hname << "HTIME_" << module << "_" << layer << "_" << pad << ends;
        int hid = kNMaxLayer*kNMaxPad*module + kNMaxPad*layer + pad;
        if (hid >= kMaxHist) {
          cerr << " hid = " << hid << " > " << kMaxHist << endl
               << " abort!" << endl;
          ::abort();
        }
        htime[hid] = new TH1F(hname.str().data(),"",kNtbin, -0.5, kNtbin-0.5);
        Add(htime[hid]);
      }
    }
  }
}

GEMHist::~GEMHist()
{
  delete [] htime;
}

GEMHist *GEMHist::GetInstance()
{
  if (!fgInstancePtr) fgInstancePtr = new GEMHist;
  return fgInstancePtr;
}

void GEMHist::ClearAll()
{
  TIter next(this);
  TH1* hp;
  while ((hp = static_cast<TH1 *>(next()))) hp->Reset();
}


// Event by event histograms
void GEMHist::FillEvent(JREvent *event)
{
  TIter next(event);
  JCluster *cp;
  while ((cp = static_cast<JCluster *>(next()))) {
    int module = cp->GetModuleID();
    if (module < 0) continue;
    int layer  = cp->GetLayerID();
    int pad    = cp->GetPadID();
    int hid    = kNMaxLayer*kNMaxPad*module + kNMaxPad*layer + pad;
    if (hid >= kMaxHist) {
      cerr << " hid = " << hid << " > " << kMaxHist << endl
           << " abort!" << endl;
      ::abort();
    } else if (!htime[hid]) {
      stringstream hname;
      hname << "HTIME_" << module << "_" << layer << "_" << pad << ends;
      htime[hid] = new TH1F(hname.str().data(),"",kNtbin, -0.5, kNtbin-0.5);
      Add(htime[hid]);
    }
    int length = cp->GetLength();
    int adtime = cp->GetStartTime();
    for (int t=0; t<length; t++) {
      int adc = (*cp)[t];
      htime[hid]->Fill(adtime+t,adc);
    }
  }
}
