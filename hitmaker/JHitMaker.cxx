#define SEP_2010

#include "JHitMaker.h"
#include "JHitLayer.h"

#include "TPad.h"
#include "TString.h"
#include "TColor.h"

#include <cstdlib>
#include <iostream>
#include <iomanip>
using namespace std;

ClassImp(JHitMaker)

JHitMaker::JHitMaker()
        : fClustersPtr(0),     // pointer to clusters
	  fDone       (kFALSE) // update flag
{
  SetOwner();
}

void JHitMaker::DebugPrint(const Char_t *opt)
{
  if (!fDone) Update();
  cerr << "Number of hits =  " << GetEntries() << endl;
  TString opts(opt);
  opts.ToLower();
  if (opts.Contains("detail")) {
    cerr << "------" << endl;
    Int_t nhitls = 0;
    TIter next(this);
    JHitLayer *hlp;
    while ((hlp = static_cast<JHitLayer *>(next()))) {
      cerr << "---<hit layer " << nhitls++ << ">-----" << endl;
      hlp->DebugPrint(opt);
    }
  }
}

void JHitMaker::SetClusters(TObjArray *jclsptr)
{
  Clear();
  fClustersPtr = jclsptr;
  fDone        = kFALSE;
}

void JHitMaker::MakeHits(TObjArray *jclsptr)
{
  SetClusters(jclsptr);
  Update();
}

void JHitMaker::Update()
{
  if (fDone) return;
  if (!fClustersPtr) {
    cerr << ":::::: ERROR in JHitMaker::Update ::::::" << endl
	 << " Clusters not yet set! abort."            << endl;   
    ::abort();
  }	  
  if (!fClustersPtr->GetEntries()) {
    cerr << ":::::: WARNING in JHitMaker::Update ::::" << endl
	 << " No clusters in array! skip."             << endl;   
    return;
  }	  
  fDone = kTRUE;

  JCluster::SetSortBy(JCluster::kDet); // set sorting policy
  fClustersPtr->Sort(); // sort clusters in ascending order of layers
  //////// VERY TEMPORARY TREATMENT : SHOULD BE FIXED ////////
  static const Int_t kNmodules = 3;
  static const Int_t kNlayers  = 28;
  TObjArray clusters[kNmodules][kNlayers];
  ////////////////////////////////////////////////////////////
  int modser    = 0;
  JCluster *cp;
  TIter next(fClustersPtr);
  while ((cp = static_cast<JCluster *>(next()))) {
#if 1
    //////// VERY TEMPORARY TREATMENT : SHOULD BE FIXED ////////
#if 1
    static const int kMaxWidth  = 200;
    //static const int kMinWidth  =  7;
    static const int kMinWidth  =  3;
    //static const int kThreshold = 10;
    static const int kThreshold = 5;
#else
    static const int kMaxWidth  = 50;
    static const int kMinWidth  =  7;
    static const int kThreshold =  6;
#endif
    ////////////////////////////////////////////////////////////
    int phmax = cp->GetMaximum();
    int width = cp->GetLength();
    if (phmax < kThreshold || width < kMinWidth || width > kMaxWidth) continue;
    int module = cp->GetModuleID();
    int layer  = cp->GetLayerID();
    switch (module) {
#ifdef SEP_2010
      case 0:
#else
      case 1:
#endif
        modser = 0;
        break;
      case 3:
        modser = 1;
        break;
#ifdef SEP_2010
      case 5:
#else
      case 6:
#endif
        modser = 2;
        break;
      default:
        continue;
    }
    clusters[modser][layer].Add(cp);
    ////////////////////////////////////////////////////////////
  }
#endif
  for (Int_t modser=0; modser<kNmodules; modser++) {
    for (Int_t layer=0; layer<kNlayers; layer++) {
      if (!clusters[modser][layer].GetEntries()) continue;
      JHitLayer *hitlayerPtr = new JHitLayer;
      TObjArray *clsp = &clusters[modser][layer];
      hitlayerPtr->MakeHits(clsp);
      Add(hitlayerPtr);
    }
  }
}

void JHitMaker::Draw(const Char_t *opt)
{
  Int_t color = 10; 
  Draw(color, opt);
}

void JHitMaker::Draw(Int_t color, const Char_t *opt)
{
  if (!gPad)   return;
  if (!fDone) Update();
  TIter next(this);
  JHitLayer *hlp;
  while ((hlp = static_cast<JHitLayer *>(next()))) {
    hlp->Draw(color, opt);  
  }
}
